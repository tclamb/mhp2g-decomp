from typing import Optional

from splat.segtypes.common.data import CommonSegData
from splat.disassembler.disassembler_section import DisassemblerSection


class PSPSegInt2(CommonSegData):
    """Segment that only contains 32-bit ints"""

    def get_linker_section(self) -> str:
        return ".data"

    def get_section_flags(self) -> Optional[str]:
        return "wa"

    def configure_disassembler_section(
        self, disassembler_section: DisassemblerSection
    ) -> None:
        "Allows to configure the section before running the analysis on it"

        super().configure_disassembler_section(disassembler_section)

        section = disassembler_section.get_section()

        # Tell spimdisasm this section only contains floats
        section.enableStringGuessing = False
        section.typeForOwnedSymbols = "i16"
        section.sizeForOwnedSymbols = 2
