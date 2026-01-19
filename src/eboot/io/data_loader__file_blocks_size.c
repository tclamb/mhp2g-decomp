#include "common.h"

extern "C" u32 data_loader__file_blocks_size(volatile data_loader *self, u32 file_id) {
  u32 uVar1;
  u32 uVar2;
  
  uVar2 = file_id & 0xffff;
  if (uVar2 == 0xffff) {
    uVar1 = 0;
  }
  else {
    uVar1 = (self->file_id_to_first_block[uVar2 + 1] - self->file_id_to_first_block[uVar2]) * 0x800;
  }
  return uVar1;
}