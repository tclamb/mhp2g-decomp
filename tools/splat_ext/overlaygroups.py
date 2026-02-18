from typing import Optional

from splat.segtypes.common.data import CommonSegData
from splat.disassembler.disassembler_section import DisassemblerSection


class PSPSegOverlaygroups(CommonSegData):
    """Segment that contains addresses for the overlay groups"""

    def get_linker_section(self) -> str:
        # hack: this doesn't have a section, but is inserted before sdata
        return ".sdata"

    def get_section_flags(self) -> Optional[str]:
        return "a"

    def configure_disassembler_section(
        self, disassembler_section: DisassemblerSection
    ) -> None:
        "Allows to configure the section before running the analysis on it"

        super().configure_disassembler_section(disassembler_section)

        section = disassembler_section.get_section()

        # Tell spimdisasm this section only contains floats
        section.enableStringGuessing = False
        section.typeForOwnedSymbols = "s32"
