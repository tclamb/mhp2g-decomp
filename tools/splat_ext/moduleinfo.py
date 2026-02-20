import struct
from pathlib import Path
from typing import Dict, List, Optional, Union, Tuple
from collections import OrderedDict
import dataclasses
import json

from splat.util import options, log, symbols
from splat.util.symbols import Symbol

from splat.segtypes.common.codesubsegment import CommonSegCodeSubsegment


MODULEINFO_PREAMBLE = (Path(__file__).parent / 'moduleinfo_preamble.h').read_text()

@dataclasses.dataclass
class ResidentBlock:
    size: int
    code: str

@dataclasses.dataclass
class Nids:
    symbol: Symbol
    values: List[int]

@dataclasses.dataclass
class ExportTargets:
    symbol: Symbol
    targets: List[Symbol]

@dataclasses.dataclass
class PspResidentExports:
    nids: Nids
    funcs: ExportTargets
    vars: ExportTargets


@dataclasses.dataclass
class PspModuleExport:
    symbol: Symbol
    name: Optional[Symbol]
    flags: int
    entry_size: int
    var_count: int
    func_count: int
    exports: PspResidentExports


@dataclasses.dataclass
class ImportName:
    version_sym: Symbol
    name_sym: Symbol
    version: int
    name: str


@dataclasses.dataclass
class PspModuleImport:
    symbol: Symbol
    name: ImportName
    flags: int
    entry_size: int
    var_count: int
    func_count: int
    nids: Nids
    funcs: List[Symbol]


@dataclasses.dataclass
class PspModuleInfo:
    symbol: Symbol
    flags: int
    name: str
    gp: Symbol
    exports: List[PspModuleExport]
    imports: List[PspModuleImport]


def aligned_size(size, alignment):
    assert alignment.bit_count() == 1
    return ((size + (alignment - 1)) // alignment) * alignment


class PSPSegModuleinfo(CommonSegCodeSubsegment):
    """Segment that contains PSP module info sections"""

    def __init__(
        self,
        rom_start: Optional[int],
        rom_end: Optional[int],
        type: str,
        name: str,
        vram_start: Optional[int],
        args: list,
        yaml,
    ):
        super().__init__(
            rom_start,
            rom_end,
            type,
            name,
            vram_start,
            args=args,
            yaml=yaml,
        )
        self.root_address = None
        if isinstance(yaml, dict) and "root_offset" in yaml:
            self.root_address = self.rom_to_ram(int(yaml["root_offset"]))
        assert isinstance(self.root_address, int)
        assert self.contains_vram(self.root_address)

        self.resident_size = None
        if isinstance(yaml, dict) and "resident_size" in yaml:
            self.resident_size = int(yaml["resident_size"])
        assert isinstance(self.resident_size, int) and self.resident_size >= 0

        self.nid_size = None
        if isinstance(yaml, dict) and "nid_size" in yaml:
            self.nid_size = int(yaml["nid_size"])
        assert isinstance(self.nid_size, int) and self.nid_size >= 0

        self.file_text: Optional[str] = None
        self.data_only = isinstance(yaml, dict) and yaml.get("data_only", False)

    def get_linker_section(self) -> str:
        # this segment occurs immediately after the .text section, and expands into a few interconnected segments:
        # .sceStub.text
        # .lib.ent.top
        # .lib.ent
        # .lib.ent.btm
        # .lib.stub.top
        # .lib.stub
        # .lib.stub.btm
        # .rodata.sceModuleInfo
        # .rodata.sceResident
        # .rodata.sceNid
        # .rodata.sceVstub
        return ".text"

    def out_path(self) -> Path:
        return options.opts.asset_path / self.dir / f"{self.name}.moduleinfo.cpp"

    def scan(self, rom_bytes: bytes):
        assert not self.data_only
        self.file_text = self.disassemble_data(rom_bytes)

    def create_string(self, addr) -> Optional[Symbol]:
        if addr == 0:
            return None
        return self.create_symbol(
            addr=addr, in_segment=True, type='asciz', define=True,
        )

    def create_nids(self, rom_bytes, addr, count) -> Nids:
        return Nids(
            symbol=self.create_symbol(
                addr=addr, in_segment=True, type='u32', define=True#, size=4*count
            ),
            values=struct.unpack_from(f"<{count}I", rom_bytes, self.ram_to_rom(addr)),
        )

    def create_targets(self, rom_bytes, addr, count, is_func=False) -> ExportTargets:
        target_addrs = struct.unpack_from(f"<{count}I", rom_bytes, self.ram_to_rom(addr))

        type = None
        if is_func:
            type = 'func'


        return ExportTargets(
            symbol=self.create_symbol(
                addr=addr, in_segment=True, type='u32', define=True#, size=4*count,
            ),
            targets=[
                self.create_symbol(addr=target_addr, in_segment=False, define=True, type=type)
                    for target_addr in target_addrs
            ],
        )

    def create_exports(self, rom_bytes, addr, func_count, var_count) -> PspResidentExports:
        assert addr != 0
        # within .rodata.sceResident
        #
        # struct exports
        # {
        #     u32 nid[func_count + var_count];
        #     void *funcs[func_count];
        #     void *vars[var_count];
        # }
        nid_count = func_count + var_count
        nid_addr = addr
        func_addr = nid_addr + 4 * nid_count
        var_addr = func_addr + 4 * func_count
        return PspResidentExports(
            nids=self.create_nids(rom_bytes, addr, nid_count),
            funcs=self.create_targets(rom_bytes, func_addr, func_count, is_func=True),
            vars=self.create_targets(rom_bytes, var_addr, var_count),
        )


    def create_import_funcs(self, addr, func_count) -> List[Symbol]:
        assert addr != 0
        # within .sceStub.text
        #
        # the elf fixup postlinker expects these to have the following form:
        # struct stub
        # {
        #     void *psp_module_import;
        #     u32 nid;
        # }
        return [
            self.create_symbol(addr=func_addr, in_segment=True, type='func', define=True)#, size=8)
            for func_addr in range(addr, addr + 8 * func_count, 8)
        ]

    def parse_export(self, rom_bytes, addr) -> PspModuleExport:
        name_ptr, flags, entry_size, var_count, func_count, exports_ptr = \
            struct.unpack_from('<IIBBHI', rom_bytes, self.ram_to_rom(addr))
        assert name_ptr == 0, "unimplemented"
        assert entry_size == 4, "unexpected export struct size"

        return PspModuleExport(
            symbol=self.create_symbol(addr=addr, in_segment=True, type='PspModuleExport', define=True),#, size=4 * entry_size),
            name=None,
            flags=flags,
            entry_size=entry_size,
            var_count=var_count,
            func_count=func_count,
            exports=self.create_exports(rom_bytes, exports_ptr, func_count, var_count),
        )

    def parse_exports(self, rom_bytes, addr, end) -> List[PspModuleExport]:
        exports = []
        while addr < end:
            entry = self.parse_export(rom_bytes, addr)
            exports.append(entry)
            addr += 4 * entry.entry_size
        assert addr == end, "failed to parse some exports"
        return exports

    def unpack_cstring_from(self, rom_bytes, offset) -> str:
        end = rom_bytes.find(0, offset)
        return rom_bytes[offset:end].decode()


    def create_import_name(self, rom_bytes, addr) -> Optional[Symbol]:
        # within .rodata.sceResident
        #
        # u32  _foo_stub_ver = 0x73; // perhaps SDK_MAJOR_VERSION | SDK_MINOR_VERSION << 4
        # char _foo_stub_str[] = "";
        # }
        version_offset = self.ram_to_rom(addr - 4)
        version, = struct.unpack_from('<I', rom_bytes, version_offset)
        return ImportName(
            version_sym=self.create_symbol(addr=addr - 4, in_segment=True, type='u32', define=True),#, size = 4),
            name_sym=self.create_symbol(addr=addr, in_segment=True, type='asciz', define=True),
            version=version,
            name=self.unpack_cstring_from(rom_bytes, self.ram_to_rom(addr)),
        )

    def parse_import(self, rom_bytes, addr) -> PspModuleImport:
        name_ptr, flags, entry_size, var_count, func_count, nids_ptr, funcs_ptr = \
            struct.unpack_from('<IIBBHII', rom_bytes, self.ram_to_rom(addr))

        if name_ptr == 0:
            return None

        assert entry_size == 5, "unexpected import struct size"
        assert var_count == 0, ".rodata.sceVstub unimplemented; not used by MHP2G"

        return PspModuleImport(
            symbol=self.create_symbol(addr=addr, in_segment=True, type='PspModuleImport', define=True),#, size=4 * entry_size),
            name=self.create_import_name(rom_bytes, name_ptr),
            flags=flags,
            entry_size=entry_size,
            var_count=var_count,
            func_count=func_count,
            nids=self.create_nids(rom_bytes, nids_ptr, func_count + var_count),
            funcs=self.create_import_funcs(funcs_ptr, func_count),
        )

    def parse_imports(self, rom_bytes, addr, end) -> List[PspModuleImport]:
        imports = []
        while addr < end:
            entry = self.parse_import(rom_bytes, addr)
            imports.append(entry)
            addr += 4 * entry.entry_size
        assert addr == end, "failed to parse some imports"
        return imports

    def parse_module_info(self, rom_bytes, addr) -> PspModuleInfo:
        flags, name, gp, exports, exp_end, imports, imp_end = \
            struct.unpack_from("<I28sIIIII", rom_bytes, self.ram_to_rom(addr))

        return PspModuleInfo(
            symbol=self.create_symbol(addr=addr, in_segment=True, type='PspModuleInfo', define=True),#, size=0x34),
            flags=flags,
            name=name[:name.find(0)].decode(),
            gp=self.create_symbol(addr=gp, in_segment=False, define=True),
            exports=self.parse_exports(rom_bytes, exports, exp_end),
            imports=self.parse_imports(rom_bytes, imports, imp_end),
        )

    def disassemble_data(self, rom_bytes) -> str:
        module_info = self.parse_module_info(rom_bytes, self.root_address)

        blocks = []
        blocks.append(MODULEINFO_PREAMBLE)

        assert len(module_info.exports) > 0 and len(module_info.imports) > 0

        first_export_sym_name = module_info.exports[0].symbol.name
        last_export_sym_name = module_info.exports[-1].symbol.name
        export_externs = set([first_export_sym_name, last_export_sym_name])

        first_import_sym_name = module_info.imports[0].symbol.name
        last_import_sym_name = module_info.imports[-1].symbol.name
        import_externs = set([first_import_sym_name, last_import_sym_name])

        blocks.append(f"""
DECLSPEC_EXTERN const psp_module_export {", ".join(export_externs)};
DECLSPEC_EXTERN const psp_module_import {", ".join(import_externs)};
extern void *_gp;
DECLSPEC_SCEMODULEINFO const psp_module_info {module_info.symbol.name} = {{
    0x{module_info.flags:08X}, {json.dumps(module_info.name)}, &_gp, &{first_export_sym_name}, &{last_export_sym_name} + 1, &{first_import_sym_name}, &{last_import_sym_name} + 1
}};
"""
        )

        # mwccpsp.exe emits in order of declaration
        # take special care that import and exports interleave correctly
        # annoyingly, the stub and resident orders may differ
        sceStubText: Dict[int, str] = {}
        sceResident: Dict[int, ResidentBlock] = {}

        for entry in module_info.exports:
            exports = entry.exports

            assert entry.func_count > 0 and entry.var_count > 0, "unimplemented"

            externs = [symbol.name for symbol in exports.funcs.targets + exports.vars.targets
                       if not self.contains_vram(symbol.vram_start)]

            sceResident[exports.nids.symbol.vram_start] = ResidentBlock(
                size=8*len(exports.nids.values),
                code=f"""
DECLSPEC_SCERESIDENT const psp_export_nids {exports.nids.symbol.name} = {{
    {", ".join([f"0x{nid:08X}" for nid in exports.nids.values])}
}};
DECLSPEC_EXTERN psp_export_target {", ".join(externs)};
DECLSPEC_SCERESIDENT const psp_export_funcs {exports.funcs.symbol.name} = {{
    {", ".join([f"&{symbol.name}" for symbol in exports.funcs.targets])}
}};
DECLSPEC_SCERESIDENT const psp_export_vars {exports.vars.symbol.name} = {{
    {", ".join([f"&{symbol.name}" for symbol in exports.vars.targets])}
}};
DECLSPEC_LIB_ENT const psp_module_export {entry.symbol.name} = {{
    0, 0x{entry.flags:08X}, {entry.entry_size}, {entry.var_count}, {entry.func_count}, {exports.nids.symbol.name}
}};
DECLSPEC_SCENID const u8 psp_nid_reserved[{self.nid_size}] = {{}};
""",
            )

        def import_name_resident_size(import_name):
            # u32 + null-terminated string
            return 4 + aligned_size(len(import_name.name) + 1, 0x4)

        for entry in module_info.imports:
            import_name: ImportName = entry.name
            stub_str = import_name.name_sym.name
            header = entry.symbol.name

            sceResident[import_name.version_sym.vram_start] = ResidentBlock(
                size=import_name_resident_size(import_name),
                code=f"""
DECLSPEC_SCERESIDENT const psp_import_version {import_name.version_sym.name} = 0x{import_name.version:X};
DECLSPEC_SCERESIDENT const psp_import_name {stub_str} = {json.dumps(import_name.name)};
DECLSPEC_LIB_STUB const psp_module_import {header} = {{
    {stub_str}, 0x{entry.flags:08X}, {entry.entry_size}, 0, 0, 0, 0
}};
""",
            )

            sceStubText[entry.nids.symbol.vram_start] = (
                "\n".join([
    f"DECLSPEC_SCESTUB_TEXT const psp_stub_text {func.name} = {{ &{header}, 0x{nid:08X} }};"
        for nid, func in zip(entry.nids.values, entry.funcs)
])
            )

        # find orphaned stub data in .rodata.sceResident
        resident_start = self.root_address + 0x34
        resident_end = resident_start + self.resident_size
        address = resident_start
        while address < resident_end:
            if address not in sceResident:
                # assume these are orphaned import names
                import_name = self.create_import_name(rom_bytes, address + 4)
                resident_size = import_name_resident_size(import_name)
                sceResident[import_name.version_sym.vram_start] = ResidentBlock(
                    size=resident_size,
                    code=f"""
DECLSPEC_SCERESIDENT const psp_import_version {import_name.version_sym.name} = 0x{import_name.version:X}; // orphaned
DECLSPEC_SCERESIDENT const psp_import_name {import_name.name_sym.name} = {json.dumps(import_name.name)}; // orphaned
""",
                )
            address += sceResident[address].size
        assert address == resident_end

        blocks.extend([resident.code for resident in OrderedDict(sorted(sceResident.items())).values()])
        blocks.extend(OrderedDict(sorted(sceStubText.items())).values())

        return "\n".join(blocks)

    def split(self, rom_bytes: bytes):
        if self.file_text and self.out_path():
            self.out_path().parent.mkdir(parents=True, exist_ok=True)

            with open(self.out_path(), "w", newline="\n") as f:
                f.write(self.file_text)

    def should_scan(self) -> bool:
        return options.opts.is_mode_active("moduleinfo")

    def should_split(self) -> bool:
        return self.extract and options.opts.is_mode_active("moduleinfo")

    @staticmethod
    def estimate_size(yaml: Union[Dict, List]) -> Optional[int]:
        if isinstance(yaml, dict) and "length" in yaml:
            return yaml["length"]
        return None
