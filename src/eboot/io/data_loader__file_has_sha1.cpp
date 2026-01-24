#include "io.hpp"

int data_loader::file_has_sha1(u32 file_id) {
  int iVar1;
  iVar1 = memcmp(file_sha1_digests[file_id & 0xffff], &null_sha1_digest, 20);
  return iVar1 != 0;
}