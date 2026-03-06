#include "common.h"

struct pac_block_header {
    u32 offset;
    u32 size;
};

struct pac_header {
    s32 block_count;
    pac_block_header block_headers[0];

    s32 count();
    u8 *data(int index);
    u32 size(int index);
};
