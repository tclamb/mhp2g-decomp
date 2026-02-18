from typing import Optional

from splat.segtypes.common.data import CommonSegData
from splat.disassembler.disassembler_section import DisassemblerSection

class PSPSegCtor(CommonSegData):
    """Segment that (at least here) only contains addresses to __sinit_ functions"""

    def get_linker_section(self) -> str:
        return ".ctor"

    def get_section_flags(self) -> Optional[str]:
        return "a"

    def configure_disassembler_section(
        self, disassembler_section: DisassemblerSection
    ) -> None:
        "Allows to configure the section before running the analysis on it"

        super().configure_disassembler_section(disassembler_section)

        section = disassembler_section.get_section()

        # Tell spimdisasm this section only contains words/references to other symbols
        section.enableStringGuessing = False
        section.typeForOwnedSymbols = "s32"
