#include "io.hpp"

u32 data_loader::file_size(u32 file_id) {
    s32 temp_v1 = file_id & 0xffff;
    s32 var_a3;
    data_loader *var_a2;

    if (temp_v1 == 0xffff) {
        return 0;
    }

    var_a3 = 0;
    var_a2 = this;
loop_5:
    if (temp_v1 == var_a2->file_size_pairs[0].file_id) {
        return file_size_pairs[var_a3].size;
    }
    var_a3++;
    var_a2 = (data_loader*)((u8*)var_a2 + sizeof(file_size_pair));
    if (var_a3 >= 0x32C) {
        return (file_id_to_first_block[temp_v1 + 1] - file_id_to_first_block[(u16)file_id]) * 0x800;
    }
    goto loop_5;
}