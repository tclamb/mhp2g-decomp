#! /usr/bin/env python3

import argparse
import os
import shutil
import sys
import json
from pathlib import Path
from typing import Dict, List, Set, Union
import subprocess
import struct
from itertools import count, repeat, chain, starmap
import dataclasses
import pycdlib
import urllib.request
import zipfile

import ninja_syntax
import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry
from splat.util import progress_bar, symbols
import spimdisasm

from tools.generate import generate_overlay_config

ROOT = Path(__file__).parent.resolve()
BIN_DIR = ROOT / "bin"
TOOLS_DIR = ROOT / "tools"
DISKS_DIR = ROOT / "disks"
CONFIG_DIR = ROOT / "config"
MODULES_DIR = ROOT / "modules"
BUILD_DIR = ROOT / "build"
UNDEFINED_SYMS_DIR = BUILD_DIR / "config"
ASM_DIR = ROOT / "asm"
ASSET_DIR = ROOT / "assets"
SRC_DIR = ROOT / "src"

DISK_PATH = DISKS_DIR / "ULJM-05500.iso"
DISK_CHECKSUM_PATH = DISKS_DIR / "ULJM-05500.sha1"
ENCRYPTED_EBOOT_PATH = DISKS_DIR / "ULJM-05500" / "EBOOT.BIN"
DATA_PATH = DISKS_DIR / "ULJM-05500" / "DATA.BIN"
EBOOT_ISO_PATH = '/PSP_GAME/SYSDIR/EBOOT.BIN'
DATA_ISO_PATH = '/PSP_GAME/USRDIR/DATA.BIN'
PSPDECRYPT_URL = 'https://github.com/John-K/pspdecrypt/releases/download/1.0/pspdecrypt-1.0-windows.zip'
PSPDECRYPT_PATH = BIN_DIR / "pspdecrypt.exe"
PSPDECRYPT_CHECKSUM_PATH = BIN_DIR / "pspdecrypt.sha1"
OBJDIFF_CLI_URL = 'https://github.com/encounter/objdiff/releases/download/v3.6.1/objdiff-cli-windows-x86.exe'
OBJDIFF_CLI_PATH = BIN_DIR / "objdiff-cli-windows-x86.exe"
OBJDIFF_CLI_CHECKSUM_PATH = BIN_DIR / "objdiff-cli-windows-x86.sha1"
OBJDIFF_REPORT_PATH = BUILD_DIR / 'report.json'
DATA_EXTRACTOR_PATH = TOOLS_DIR / "data-extractor"
EBOOT_CHECKSUM_PATH = CONFIG_DIR / "eboot.sha1"
OVERLAYS_CHECKSUM_PATH = CONFIG_DIR / "overlays.sha1"
ALL_MODULES_CHECKSUM_PATH = BUILD_DIR / "config" / "modules.sha1"

COMMON_INCLUDES = "-Iinclude -Iinclude/pspsdk"

COMMON_COMPILE_FLAGS = "-Cpp_exceptions off -flag no-opt_unroll_loops -flag explicit_zero_data -O4,p -gccinc -maxerrors 3 -w nocmdline -lang=c++ -RTTI off -sdatathreshold 0"

GAME_GCC_CMD = f"./bin/mwccpsp.exe {COMMON_COMPILE_FLAGS} -c {COMMON_INCLUDES} $in -o $out"
WIBO_GAME_GCC_CMD = f"./bin/wibo {GAME_GCC_CMD}"

OBJDIFF_CLI_CMD = "./bin/objdiff-cli-windows-x86.exe report generate -o $out"
GITHUB_ACTION_OBJDIFF_CLI_CMD = OBJDIFF_CLI_CMD.replace('windows-x86.exe', 'linux-x86_64', 1)

@dataclasses.dataclass
class ModuleInfo:
    name: str
    subfolder: str

    def stem(self):
        return Path(self.name).stem

    def artifact_path(self, parent, suffix = None):
        path = parent / Path(self.subfolder) / self.name
        if suffix != None:
            return path.with_suffix(suffix)
        return path

    def splat_yaml_path(self):
        return self.artifact_path(CONFIG_DIR, ".splat.yaml")

    def module_path(self):
        return self.artifact_path(MODULES_DIR)

    def build_path(self):
        return BUILD_DIR / self.module_path().relative_to(ROOT)

    def symbol_addrs_paths(self):
        return [self.artifact_path(CONFIG_DIR, ".symbol_addrs.txt")]

    def reloc_addrs_paths(self):
        return [self.artifact_path(CONFIG_DIR, ".reloc_addrs.txt")]

    def undefined_funcs_auto_path(self):
        return self.artifact_path(UNDEFINED_SYMS_DIR, ".undefined_funcs_auto.txt")

    def undefined_syms_auto_path(self):
        return self.artifact_path(UNDEFINED_SYMS_DIR, ".undefined_syms_auto.txt")

    def asm_path(self):
        return self.artifact_path(ASM_DIR, "")

    def asset_path(self):
        return self.artifact_path(ASSET_DIR, "")

    def src_path(self):
        return self.artifact_path(SRC_DIR, "")

    def ld_script_path(self):
        return self.build_path().parent / (self.name + ".ld")

@dataclasses.dataclass
class OverlayHeader:
    magic_bytes: bytes
    overlay_id: int
    load_address: int
    text_size: int
    data_size: int
    bss_size: int
    static_initializers_start_address: int
    static_initializers_end_address: int
    name: str

    @staticmethod
    def parse(bytes: bytes):
        header = bytes[:64]
        size = len(bytes)

        magic_bytes, overlay_id, \
        load_address, text_size, \
        data_size, bss_size, \
        static_init_start, static_init_end, \
        name = struct.unpack_from('<4sIIIIIII32s', header)
        name = name[:name.index(b"\0")].decode()

        if magic_bytes != b'MWo3':
            raise ValueError(f"Overlay {name} has malformed header")
        expected_size = 64 + text_size + data_size
        if size != expected_size:
            raise ValueError(f"Overlay {name} has wrong size (expected: {expected_size}, was: {size})")
        if text_size < 0x40:
            raise ValueError(f"Overlay {name} text too small (was: 0x{text_size:x}, which is less than the 0x40 minumum from padding)")
        if any(bytes[0x40:0x80]):
            raise ValueError(f"Overlay {name} text missing the expected 64 bytes of padding")

        return OverlayHeader(magic_bytes, overlay_id, load_address, text_size, data_size, bss_size, static_init_start, static_init_end, name)

@dataclasses.dataclass
class OverlayInfo(ModuleInfo):
    file_id: int

    def extract(self, generate_config=False):
        out_file = self.module_path()
        out_file.parent.mkdir(parents=True, exist_ok=True)
        subprocess.run([
            str(DATA_EXTRACTOR_PATH), str(DATA_PATH), str(self.file_id), str(out_file),
        ]).check_returncode()
        bytes = out_file.read_bytes()
        header = OverlayHeader.parse(bytes)

        if header.name != self.name:
            raise ValueError(f"Wrong overlay (expected: {self.name}, was: {header.name})")
        size = 0x40 + header.text_size + header.data_size
        if size != len(bytes):
            raise ValueError(f"Overlay wrong size (expected: {size}, was: {len(bytes)})")

        if generate_config:
            generate_overlay_config(self, header, bytes)

TASK_OVERLAY_FIRST_ID = 43
TASK_OVERLAY_NAMES = [
    f"{s}_task.ovl" for s in [
        "demo","edit", "cont", "option", "gallery", "download",
        "install", "lobby", "game", "show_select", "arcade", "movie"]
]
SUB_OVERLAY_FIRST_ID = 55
SUB_OVERLAY_NAMES = [f"{s}_sub.ovl" for s in ["demo", "game"]]
EM_OVERLAY_FIRST_ID = 6043
EM_OVERLAY_NAMES = [
    f"em{n:02d}.ovl" for n in [1, 2, 7, 14, 15, 17, 20, 21, 33, 40, 54, 55, 58, 59, 75, 82, 83]
]
STAGE_OVERLAY_FIRST_ID = 5490
STAGE_OVERLAY_NAMES = [
    f"stage{i:03d}.ovl" for i in range(267)
]
all_overlays = list(starmap(OverlayInfo, chain.from_iterable([
    zip(TASK_OVERLAY_NAMES,  repeat("task"),                            count(TASK_OVERLAY_FIRST_ID)),
    zip(SUB_OVERLAY_NAMES,   repeat("sub"),                             count(SUB_OVERLAY_FIRST_ID)),
    zip(EM_OVERLAY_NAMES,    repeat("em"),                              count(EM_OVERLAY_FIRST_ID)),
    zip(STAGE_OVERLAY_NAMES, map(lambda x: x[:6], STAGE_OVERLAY_NAMES), count(STAGE_OVERLAY_FIRST_ID)),
])))
EBOOT_MODULE = ModuleInfo("eboot.elf", ".")
all_modules = [EBOOT_MODULE] + all_overlays

def clean():
    if os.path.exists(".splache"):
        os.remove(".splache")
    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("build", ignore_errors=True)

def sha1sum_check(checksum_path, throw_on_failure=True):
    result = subprocess.run(
        ['sha1sum', '-c', str(checksum_path)],
        check=throw_on_failure, capture_output=not throw_on_failure,
    )
    if throw_on_failure:
        result.check_returncode()
    return result.returncode == 0

def extract_iso():
    if all([sha1sum_check(p.relative_to(ROOT), throw_on_failure=False) for p in [EBOOT_CHECKSUM_PATH, OVERLAYS_CHECKSUM_PATH]]):
        return

    if not DISK_PATH.exists():
        print(f"UMD image not found; copy your backup to '{str(DISK_PATH.relative_to(ROOT))}'")
        exit(-1)

    if ENCRYPTED_EBOOT_PATH.exists() and DATA_PATH.exists():
        if sha1sum_check(DISK_CHECKSUM_PATH, throw_on_failure=False):
            return

    ENCRYPTED_EBOOT_PATH.parent.mkdir(parents=True, exist_ok=True)
    DATA_PATH.parent.mkdir(parents=True, exist_ok=True)

    iso = pycdlib.PyCdlib()
    iso.open(str(DISK_PATH))

    with ENCRYPTED_EBOOT_PATH.open('wb') as eboot:
        iso.get_file_from_iso_fp(eboot, iso_path=EBOOT_ISO_PATH)

    with DATA_PATH.open('wb') as data:
        iso.get_file_from_iso_fp(data, iso_path=DATA_ISO_PATH)

    sha1sum_check(DISK_CHECKSUM_PATH)

def download_pspdecrypt():
    archive_filename, _ = urllib.request.urlretrieve(PSPDECRYPT_URL)
    try:
        with zipfile.ZipFile(archive_filename) as archive:
            archive.extract(PSPDECRYPT_PATH.name, path=PSPDECRYPT_PATH.parent)
        sha1sum_check(PSPDECRYPT_CHECKSUM_PATH)
    finally:
        os.remove(archive_filename)

    PSPDECRYPT_PATH.chmod(0o755)

def download_objdiff_cli():
    urllib.request.urlretrieve(OBJDIFF_CLI_URL, filename = OBJDIFF_CLI_PATH)
    sha1sum_check(OBJDIFF_CLI_CHECKSUM_PATH)
    OBJDIFF_CLI_PATH.chmod(0o755)

def ensure_objdiff_cli():
    if not (OBJDIFF_CLI_PATH.exists() and sha1sum_check(OBJDIFF_CLI_CHECKSUM_PATH, throw_on_failure=False)):
        download_objdiff_cli()

# using wibo with pspdecrypt.exe segfaults; TODO: download & use the macos and linux binaries as appropriate
def decrypt_eboot():
    if EBOOT_MODULE.module_path().exists() and sha1sum_check(EBOOT_CHECKSUM_PATH, throw_on_failure=False):
        return

    if not (PSPDECRYPT_PATH.exists() and sha1sum_check(PSPDECRYPT_CHECKSUM_PATH, throw_on_failure=False)):
        download_pspdecrypt()

    EBOOT_MODULE.module_path().parent.mkdir(parents=True, exist_ok=True)
    # pspdecrypt.exe does not handle whitespace in paths
    subprocess.run(
        [str(PSPDECRYPT_PATH), "-o", str(EBOOT_MODULE.module_path().relative_to(ROOT)), str(ENCRYPTED_EBOOT_PATH.relative_to(ROOT))]
    ).check_returncode()

def extract_overlays(generate_config=False):
    if not generate_config and sha1sum_check(OVERLAYS_CHECKSUM_PATH, throw_on_failure=False):
        return

    subprocess.run(
        ["g++", str(DATA_EXTRACTOR_PATH.with_suffix(".cpp")), "-Wno-int-to-pointer-cast", "-Iinclude", "-Iinclude/pspsdk", "-O3", "-o", str(DATA_EXTRACTOR_PATH)]
    ).check_returncode()

    overlay_bar = progress_bar.get_progress_bar(all_overlays)
    for overlay in overlay_bar:
        overlay_bar.set_description(f"Extracting {overlay.name}")
        overlay.extract(generate_config=generate_config)

def build_stuff(linker_entries: List[LinkerEntry], github_workflow=False):
    built_objects: Dict[String,Set[Path]] = dict()
    built_units: List[Path] = []
    built_categories: Set[String] = set()

    def build(
        object_paths: Union[Path, List[Path]],
        src_paths: List[Path],
        task: str,
        variables: Dict[str, str] = {},
        implicit_outputs: List[str] = [],
    ):
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        for object_path in object_paths:
            if object_path.suffix == ".o":
                module = object_path.parts[2]
                if not module == "eboot":
                    module = object_path.parts[3]
                built_objects.setdefault(module, []).append(str(object_path))

        ninja.build(
            outputs=[str(o) for o in object_paths],
            rule=task,
            inputs=[str(s) for s in src_paths],
            implicit=["./bin/pspas"] if task.startswith("as") else [],
            variables=variables,
            implicit_outputs=implicit_outputs,
        )

    def add_unit(target_path, base_path, src_path = None, is_complete = False):
        if target_path:
            unit_path = target_path.relative_to('build/asm').with_suffix('')
        elif base_path:
            unit_path = base_path.relative_to('build/assets').with_suffix('')
        unit_categories = [str(parent) for parent in unit_path.parents][:-1] # / ["."]
        for c in unit_categories:
            built_categories.add(c)
        built_units.append({
            "name": str(unit_path),
            "target_path": target_path and str(target_path) or str(base_path),
            "base_path": base_path and str(base_path),
            "metadata": {
                "progress_categories": unit_categories,
                "source_path": str(src_path),
                "complete": is_complete,
            }
        })

    ninja = ninja_syntax.Writer(open(str(ROOT / "build.ninja"), "w"), width=9999)

    # Rules
    cross = "mipsel-linux-gnu-"

    ninja.rule(
        "as",
        description="as $in",
        command=f"cat $in | ./bin/pspas -EL -I include/ -G0 -march=allegrex -mabi=eabi -no-pad-sections -o $out",
    )

    ninja.rule(
        "as.target",
        description="as $in",
        command=f"cat include/macro.inc $in | ./bin/pspas -EL -I include/ -G0 -march=allegrex -mabi=eabi -no-pad-sections -o $out",
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=WIBO_GAME_GCC_CMD if github_workflow else GAME_GCC_CMD,
    )

    ninja.rule(
        "objdiff-report",
        description="objdiff-cli report generate",
        command=GITHUB_ACTION_OBJDIFF_CLI_CMD if github_workflow else OBJDIFF_CLI_CMD,
    )

    deinit = "git submodule deinit -f tools/sotn-decomp"
    init = "git submodule update --init tools/sotn-decomp"
    apply = "git apply $in"
    cargobuild = "cargo build --release"
    cpout = "cp ./tools/sotn-decomp/tools/pspas/target/release/pspas $out"
    ninja.rule(
        "pspas",
        command=f"{deinit} && {init} && {apply} && cd ./tools/sotn-decomp/tools/pspas && {cargobuild} && cd ../../../.. && {cpout} && {deinit}"
    )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{cross}ld -EL -Map $map $ldscripts -T $in -o $out",
    )

    ninja.rule(
        "sha1sum",
        description="sha1sum $in",
        command="cd build && fgrep '$module' ../$in | sha1sum -c - && touch ../$out",
    )

    ninja.rule(
        "elf",
        description="elf $out",
        command=f"{cross}objcopy -O binary --gap-fill=0x00 --strip-section-headers $in $out",
    )

    ninja.rule(
        "cppsp",
        description="cppsp $in $out",
        command=f"{cross}ld --oformat elf32-tradlittlemips -r -b binary -o $out $in",
    )

    ninja.rule(
        "concat",
        description="cat $in > $out",
        command="cat $in > $out",
    )

    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if isinstance(seg, splat.segtypes.common.asm.CommonSegAsm) or isinstance(
            seg, splat.segtypes.common.data.CommonSegData
        ):
            build(entry.object_path, entry.src_paths, "as")
            if not isinstance(seg, splat.segtypes.common.bss.CommonSegBss):
                add_unit(entry.object_path, None)
        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            if any(
                str(src_path).startswith("src/lib/") for src_path in entry.src_paths
            ):
                build(entry.object_path, entry.src_paths, "libcc")
            else:
                base_path = entry.object_path
                target_path = 'build/asm' / base_path.relative_to('build/src')

                build(
                    target_path,
                    ['asm' / p.relative_to('src').with_suffix('.s') for p in entry.src_paths],
                    "as")
                build(base_path, entry.src_paths, "cc")

                add_unit(target_path, base_path, src_path=entry.src_paths[0])
        elif isinstance(seg, splat.segtypes.common.databin.CommonSegDatabin):
            build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.bin.CommonSegBin):
            build(entry.object_path, entry.src_paths, "cppsp")
        elif seg.type in ("bytetable", "cstring", "sha1digests"):
            base_path = entry.object_path

            build(base_path, entry.src_paths, "cc")

            add_unit(None, base_path, src_path=entry.src_paths[0])
        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)


    ninja.build(
        str(ALL_MODULES_CHECKSUM_PATH.relative_to(ROOT)),
        "concat",
        [str(p.relative_to(ROOT)) for p in [EBOOT_CHECKSUM_PATH, OVERLAYS_CHECKSUM_PATH]],
    )

    for module in all_modules:
        target = str(module.build_path().relative_to(ROOT))

        ninja.build(
            target + ".elf",
            "ld",
            target + ".ld",
            implicit=built_objects[module.stem()],
            variables={
                "map": target + ".map",
                "ldscripts": "".join(set([
                    f" -T {str(path.relative_to(ROOT))}" for path in
                        [
                            module.undefined_syms_auto_path(),
                            module.undefined_funcs_auto_path(),
                        ]
                ])),
            },
        )
        ninja.build(
            target,
            "elf",
            target + ".elf",
        )
        ninja.build(
            target + ".ok",
            "sha1sum",
            str(ALL_MODULES_CHECKSUM_PATH.relative_to(ROOT)),
            implicit=[target],
            variables={
                "module": str(module.module_path().relative_to(ROOT)),
            }
        )

    ninja.build(
        "bin/pspas",
        "pspas",
        "sotn-decomp-pspas.patch"
    )

    ninja.build(
        str(OBJDIFF_REPORT_PATH.relative_to(ROOT)),
        "objdiff-report",
        "objdiff.json",
        implicit = [str(module.build_path().relative_to(ROOT)) + ".ok" for module in all_modules],
    )

    with open("objdiff.json", "w") as o:
        json.dump({
            "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
            "custom_make": "ninja",
            "build_target": True,
            "build_base": True,
            "units": built_units,
            "progress_categories": [{"id": c, "name": c} for c in built_categories],
            "scratch": {
                "platform": "psp",
                "compiler": "mwccpsp_3.0.1_219",
                "c_flags": COMMON_COMPILE_FLAGS,
            },
            "ignore_patterns": [
                "build/**/*",
                "tools/**/*",
            ],
            "options": {
                "mips.abi": "eabi32",
                "mips.instrCategory": "r4000allegrex",
            },
        }, o)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "-c",
        "--clean",
        help="Clean extraction and build artifacts",
        action="store_true",
    )
    parser.add_argument(
        "-g",
        "--generate-overlay-config",
        help="Generate fresh splat configuration files for overlays",
        action="store_true"
    )
    parser.add_argument(
        "--github-workflow",
        help="Enable hacks to build the GitHub workflow",
        action="store_true"
    )
    args = parser.parse_args()

    if args.clean:
        clean()

    if not args.github_workflow:
        ensure_objdiff_cli()

    all_linker_entries = []
    def split_module(module):
        # ensure splat uses a fresh context
        # otherwise, symbols in previously disassembled modules
        # will not be emitted to undefined_syms/funcs_auto.txt
        symbols.spim_context = spimdisasm.common.Context()
        split.main([module.splat_yaml_path()], modes="all", verbose=False)
        all_linker_entries.extend(split.linker_writer.entries)

    extract_iso()
    decrypt_eboot()
    split_module(EBOOT_MODULE)
    extract_overlays(generate_config=args.generate_overlay_config)
    for overlay in all_overlays:
        split_module(overlay)
    build_stuff(all_linker_entries, github_workflow=args.github_workflow)
