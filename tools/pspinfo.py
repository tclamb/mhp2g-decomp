#! /usr/bin/env python3
from pathlib import Path
from dataclasses import dataclass
import re
import sys

def caesar(s, delta):
    return bytes(map(lambda x: x + delta, bytes(s)))

psp_libinfo_re = re.compile(re.escape(caesar(b'Lib-PSP ', 0x10)) + b'(.{12})(.{4})', re.MULTILINE | re.DOTALL)

@dataclass
class PspLibinfo:
    offset: int
    short_name: str
    sdk_version: bytes

# credit to @Nemoumbra for figuring this out
def find_psp_info(byte_seq):
    result = []
    for match in psp_libinfo_re.finditer(byte_seq):
        result.append(PspLibinfo(
            offset=match.start(),
            short_name=caesar(match.group(1), -0x12).decode().strip(),
            sdk_version=caesar(match.group(2), -0x14),
        ))
    return result

prefix = ""
for filename in sys.argv[1:]:
    if len(sys.argv) > 2:
        prefix = filename + ", "
    for libinfo in find_psp_info(Path(filename).read_bytes()):
        print(f"{prefix}0x{libinfo.offset:x}, {libinfo.short_name}, {libinfo.sdk_version}")
