from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
from mmap import mmap
from sys import argv


def fixup_pc16_relocs(mm, elf):
    pokes = []

    for section in elf.iter_sections():
        if not isinstance(section, RelocationSection):
            continue

        target_section = elf.get_section(section['sh_info'])
        section_offset = target_section['sh_offset']

        for reloc in section.iter_relocations():
            if not reloc['r_info_type'] == 10: # R_MIPS_PC16
                continue

            assert not reloc.is_RELA()
            offset = reloc['r_offset']
            assert target_section.data()[offset:offset+2] == b'\xff\xff'
            pokes.append(section_offset + offset)

    if len(pokes) == 0:
        return

    for poke in pokes:
        mm.seek(poke)
        mm.write(b'\x00\x00')

def fixup_addraligns(mm, elf):
    header_offset = elf['e_shoff']
    header_size = elf['e_shentsize']

    pokes = []
    for i, section in enumerate(elf.iter_sections()):
        if section.name in ('.text', '.rodata', '.data', '.init', '.ctor', '.vtables', '.bss'):
            pokes.append(header_offset + i * header_size + 32) # sh_addralign offset

    if len(pokes) == 0:
        return

    for poke in pokes:
        mm.seek(poke)
        mm.write(b'\x04\x00\x00\x00')

if __name__ == '__main__':
    assert len(argv) == 2

    with open(argv[1], 'r+b') as f:
        mm = mmap(f.fileno(), 0)
        elf = ELFFile(mm)

        fixup_pc16_relocs(mm, elf)
        fixup_addraligns(mm, elf)
