#include "io.hpp"

void data_loader::calculate_file_block_offsets() {
  u32 s16;
  s32 iVar1;
  u32 wVar2;
  u32 wVar3;
  u32 uVar5;
  block_offset *pdVar5;
  u32 uVar4;

  wVar3 = 0;
  uVar5 = 0;
  pdVar5 = file_id_to_block_offset;
  for (uVar4 = 0; uVar4 < 0x19ca; uVar4++) {
    s16 = file_has_sha1(uVar4 & 0xffff);
    if (s16 != 0) {
      iVar1 = file_size((u16)uVar4);
      if (uVar5 + iVar1 > 0x370000) {
        wVar3 = wVar3 + 1;
        uVar5 = 0;
        wVar3 = (u16) wVar3;
      }
      wVar2 = (uVar5 >> 0xb);
      pdVar5->block_number = wVar3;
      pdVar5->offset = wVar2;
      uVar5 = uVar5 + iVar1;
    } else {
      pdVar5->block_number = 0xffff;
      pdVar5->offset = 0;
    }
    pdVar5 = pdVar5 + 1;
  }
}
