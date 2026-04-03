import yaml
from typing import List
from pathlib import Path
import struct
import re
from sys import argv

def _gen_jis2ucs_inc_hpp(symbol, data, offset):
    lines = []

    table = struct.iter_unpack('<H', data[offset:offset + 0x20000])
    for jis, (ucs,) in enumerate(table):
        if ucs != 0:
            lines.append(f"    [0x{jis:X}] = 0x{ucs:X},")

    return f"const u16 {symbol}[0x10000] = {{\n" \
        + "\n".join(lines) \
        + "\n};\n"


def _gen_cameraop_inc_hpp(symbol, data, offset):
    lines = []

    def assertOpsizeEquals(i, opsize, expectsize):
        assert opsize == expectsize, f"Invalid camera opsize at symbol:{symbol} addend:{i}, expected: {expectsize}, was: {opsize}"
    def assertPadBytes(i, size):
        assert not any(data[i:i + size]), f"Expected {size} pad bytes at symbol:{symbol} addend:{i}"

    i = offset
    while True:
        opcode, opsize = struct.unpack_from('<BB', data, i)
        if opcode in (0x2A,):
            # terminate() equivalent?
            assertOpsizeEquals(i, opsize, 0)
            assertPadBytes(i + 2, 2)
            lines.append(f"    0x{opcode:02X},")
            opsize = 1
        elif opcode in (0x28, 0x29, 0x2B, 0x2C, 0x2D):
            # nullary
            assertOpsizeEquals(i, opsize, 1)
            assertPadBytes(i + 2, 2)
            lines.append(f"    0x1{opcode:02X},")
        elif opcode in (0, 7, 0x24, 0x27):
            # unary, s8 arg
            assertOpsizeEquals(i, opsize, 1)
            assertPadBytes(i + 3, 1)
            arg, = struct.unpack_from('<b', data, i + 2)
            lines.append(f"    0x1{opcode:02X} | ((u8){arg} << 16),")
        elif opcode in (1, 4, 0x23):
            # binary, s8 args
            assertOpsizeEquals(i, opsize, 1)
            arg1, arg2 = struct.unpack_from('<bb', data, i + 2)
            lines.append(f"    0x1{opcode:02X} | ((u8){arg1} << 16) | ((u8){arg2} << 24),")
        elif 8 <= opcode <= 0xB or 0xE <= opcode <= 0x22 or opcode in (0x25, 0x26):
            # unary, s16 arg
            assertOpsizeEquals(i, opsize, 1)
            arg, = struct.unpack_from('<h', data, i + 2)
            lines.append(f"    0x1{opcode:02X} | ((u16){arg} << 16),")
        elif opcode in (2, 3, 5, 6):
            # ternary, s32 args
            assertOpsizeEquals(i, opsize, 4)
            assertPadBytes(i + 2, 2)
            arg1, arg2, arg3 = struct.unpack_from('<iii', data, i + 4)
            lines.append(f"    0x4{opcode:02X}, (u32){arg1}, (u32){arg2}, (u32){arg3},")
        elif opcode in (0xC, 0xD):
            # unary, s32 arg
            assertOpsizeEquals(i, opsize, 2)
            assertPadBytes(i + 2, 2)
            arg, = struct.unpack_from('<i', data, i + 4)
            lines.append(f"    0x4{opcode:02X}, (u32){arg},")
        else:
            assert False, f"Invalid camera opcode at symbol:{symbol} addend:{i}, was: 0x{opcode:X}"
        assert opsize > 0
        i += 4 * opsize
        if opcode in (0x29, 0x2A):
            # ret / terminate
            break

    return f"u32 {symbol}[{(i - offset) // 4}] = {{\n" \
        + "\n".join(lines) \
        + "\n};\n"

_extractors = {
    'cameraop': _gen_cameraop_inc_hpp,
    'jis2ucs': _gen_jis2ucs_inc_hpp,
}


def _extractImpl(config, symbolMap):
    assert 'asset_path' in config
    assetPath = Path(config['asset_path'])
    assert 'target_path' in config
    targetPath = Path(config['target_path'])
    assert 'start' in config
    romStart = config['start']
    assert 'vram' in config
    vramStart = config['vram']
    assert 'segments' in config

    target = targetPath.read_bytes()
    for segment in config['segments']:
        address = vramStart + (segment[0] - romStart)
        symbol = symbolMap[address]
        kind = segment[1]
        outPath = (assetPath / symbol).with_suffix(f".{kind}.inc.hpp")
        if kind in _extractors.keys():
            outPath.write_text(_extractors[kind](symbol, target, segment[0]))


def extract(yamlPath: Path, symbolAddrsPaths: List[Path]):
    config = yaml.load(yamlPath.read_text(), yaml.Loader)

    symbolMap = {}
    for path in symbolAddrsPaths:
        for line in path.read_text().splitlines():
            if m := re.match(r'([^\s]+)\s*=\s*(0x[^;\s]+)', line):
                symbol, addressStr = m.groups()
                vramAddress = int(addressStr, 16)
                symbolMap[vramAddress] = symbol

    _extractImpl(config, symbolMap)

if __name__ == '__main__':
    assert len(argv) == 2

    yamlPath = Path(argv[1])
    symbolAddrsPath = yamlPath.with_suffix("").with_suffix(".symbol_addrs.txt")

    extract(yamlPath, [symbolAddrsPath])
