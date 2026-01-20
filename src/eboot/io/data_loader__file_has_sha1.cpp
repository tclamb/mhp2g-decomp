#include "io.hpp"

extern "C" u8 data_loader__file_sha1_digests[][20];
extern "C" u8 data_loader__null_sha1_digest[];
extern "C" int memcmp(void*,void*,u32);

extern "C" int data_loader__file_has_sha1(volatile data_loader *self, u32 file_id) {
  int iVar1;
  iVar1 = memcmp(data_loader__file_sha1_digests[file_id & 0xffff], &data_loader__null_sha1_digest, 20);
  return iVar1 != 0;
}