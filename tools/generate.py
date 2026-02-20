import yaml
from pathlib import Path
from hashlib import sha1
from itertools import repeat
import struct

ROOT = Path(__file__).parent.parent.resolve()
BUILD_DIR = Path(__file__).parent.parent / "build"

class HexInt(int): pass
yaml.add_representer(HexInt, lambda dumper, data: dumper.represent_scalar('tag:yaml.org,2002:int', hex(data)))

class AddressInt(int): pass
yaml.add_representer(AddressInt, lambda dumper, data: dumper.represent_scalar('tag:yaml.org,2002:int', f"0x{data:08x}"))

yaml.add_representer(type(Path('')), lambda dumper, data: dumper.represent_scalar('tag:yaml.org,2002:str', str(data.relative_to(ROOT))))

class FlowSequence(list): pass
yaml.add_representer(FlowSequence, lambda dumper, data: dumper.represent_sequence('tag:yaml.org,2002:seq', data, flow_style=True))

class LiteralString(str): pass
yaml.add_representer(LiteralString, lambda dumper, data: dumper.represent_scalar('tag:yaml.org,2002:str', str(data), style='|'))


def generate_overlay_config(overlay, header, bytes):
    out_file = overlay.module_path()
    sha1_digest = sha1(bytes).hexdigest()

    for p in {overlay.splat_yaml_path(), *overlay.symbol_addrs_paths(), *overlay.reloc_addrs_paths()}:
        p.parent.mkdir(parents=True, exist_ok=True)
        p.touch(exist_ok=True)

    code_subsegments = []
    if header.text_size > 0x40:
        code_subsegments.extend([
            [HexInt(0x40), "pad"],
            [HexInt(0x80), "asm", overlay.stem()],
        ])
    data_offset = 0x40 + header.text_size
    aligned_data_offset = ((data_offset + 0x7f) >> 7) << 7
    if aligned_data_offset > data_offset:
        code_subsegments.append(
            [HexInt(data_offset), "pad"]
        )
    if header.data_size > aligned_data_offset - data_offset:
        code_subsegments.append(
            [HexInt(aligned_data_offset), "data", overlay.stem()]
        )

    if header.static_initializers_start_address != header.static_initializers_end_address:
        ctor_offset = header.static_initializers_start_address - header.load_address
        init_address, = struct.unpack("<I", bytes[ctor_offset:ctor_offset + 4])
        init_offset = init_address - header.load_address
        code_subsegments.extend([
            [HexInt(init_offset), "init", overlay.stem()],
            [HexInt(ctor_offset), "ctor", overlay.stem()],
        ])

    bss_offset = data_offset + header.data_size
    ctor_end_offset = header.static_initializers_end_address - header.load_address
    if ctor_end_offset != bss_offset:
        code_subsegments.append(
            [HexInt(ctor_end_offset), "pad"]
        )

    if header.bss_size > 0:
        code_subsegments.append(
            [HexInt(0x40 + header.text_size + header.data_size), "bss", overlay.stem()]
        )

    segments = [
        [0x0, "bin", "omit_ovl_header"]
    ]
    if len(code_subsegments) > 0:
        segments.append({
            "name": out_file.stem,
            "type": "code",
            "start": HexInt(0x40),
            "vram": AddressInt(header.load_address + 0x40),
            "align": 4,
            "subalign": 4,
            "bss_size": HexInt(header.bss_size),
            "subsegments": [FlowSequence(x) for x in code_subsegments],
        })
    segments.append([HexInt(bss_offset)])

    stem = overlay.stem()
    overlay.splat_yaml_path().write_text(yaml.dump(
    {
        "name": stem,
        "sha1": sha1_digest,
        "options": {
            "platform": "psp",
            "basename": overlay.name,
            "base_path": "/".join(repeat("..", len(overlay.splat_yaml_path().relative_to(ROOT).parents) - 1)),
            "build_path": BUILD_DIR,
            "target_path": out_file,
            "asm_path": overlay.asm_path(),
            "asset_path": overlay.asset_path(),
            "src_path": overlay.src_path(),
            "ld_script_path": overlay.ld_script_path(),
            "extensions_path": "tools/splat_ext",
            "compiler": "MWCCPS2",
            "undefined_funcs_auto_path": overlay.undefined_funcs_auto_path(),
            "undefined_syms_auto_path": overlay.undefined_syms_auto_path(),
            "ld_generate_symbol_per_data_segment": True,
            "gp_value": AddressInt(0x089ceb50),
            "symbol_name_format": f'{stem}_$VRAM',
            "symbol_name_format_no_rom": '$VRAM',
            "create_asm_dependencies": True,
            "create_undefined_funcs_auto": True,
            "create_undefined_syms_auto": True,
            "find_file_boundaries": True,
            "o_as_suffix": True,
            "symbol_addrs_path": overlay.symbol_addrs_paths(),
            "reloc_addrs_path": overlay.reloc_addrs_paths(),
            "string_encoding": "SHIFT_JIS",
            "data_string_encoding": "SHIFT_JIS",
            "rodata_string_guesser_level": 0,
            "data_string_guesser_level": 0,
            "add_set_gp_64": False,
            "pair_rodata_to_text": True,
            "suggestion_rodata_section_start": True,
            "global_vram_start": AddressInt(0x08000000),
            "global_vram_end":   AddressInt(0x09ffffff),
            "named_regs_for_c_funcs": False,
            "migrate_rodata_to_functions": True,
            "ld_bss_is_noload": True,
            "disassemble_all": True,
            "auto_decompile_empty_functions": True,
            "make_full_disasm_for_code": True,
            "asm_inc_header": LiteralString(".set noat\n.set noreorder"),
            "section_order": [
                ".text",
                ".data",
                ".rodata",
                ".init",
                ".ctor",
                ".bss",
            ],
        },
        "segments": segments,
    }, default_flow_style=False))
