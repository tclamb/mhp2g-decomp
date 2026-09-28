#!/usr/bin/env python3
"""Set st_size = 0 on every '*.NON_MATCHING' symbol, in place.

splat's `nonmatching` macro emits a sized global alias at each function/data
start. mwldpsp 1.1 build 30 (CodeWarrior PSP R1.3 SP9) sums symbol sizes per
section (.text, .rodata, .data, ...) and aborts ("the sum of all symbol sizes
exceed section size") when those aliases double-count. Zeroing the alias size
is the smallest change: section bytes, bindings (incl. Metrowerks' weak binding
13, which `objcopy --strip-symbol` rewrites to LOCAL and thereby lets the
linker dead-strip INCLUDE_ASM copies of inline functions) and relocations stay
untouched.

Side effect: objdiff's *data* match percentage is computed differently once
the aliases have no size, so the local report's data % is not comparable with
decomp.dev (code % is unaffected).

Idempotent. usage: zero_nonmatching.py <obj>...
"""
import struct
import sys

from elftools.elf.elffile import ELFFile


def patch(path):
    with open(path, "r+b") as f:
        elf = ELFFile(f)
        edits = []
        for sec in elf.iter_sections():
            if sec.header.sh_type != "SHT_SYMTAB":
                continue
            entsize = sec.header.sh_entsize
            for i, sym in enumerate(sec.iter_symbols()):
                if sym.name.endswith(".NON_MATCHING") and sym["st_size"] != 0:
                    # Elf32_Sym: name(4) value(4) size(4) info(1) other(1) shndx(2)
                    edits.append(sec.header.sh_offset + i * entsize + 8)
        for off in edits:
            f.seek(off)
            f.write(struct.pack("<I", 0))
        return len(edits)


total = files = 0
for p in sys.argv[1:]:
    n = patch(p)
    total += n
    files += bool(n)
if total:
    print(f"zeroed {total} NON_MATCHING sizes in {files} objects")
