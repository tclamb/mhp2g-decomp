#include "io.hpp"

void data_loader::movie_open(u16 arg1) {
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_v0;
    s32 var_a1;
    s32 *temp_s1;
    s32 temp_a0;
    s32 temp_v1;

    is_playing_movie = 1;
    temp_v0 = arg1 & 0xFFFF;
    movie_file_id = (u16) arg1;
    temp_s1 = temp_v0 + (s32*)&file_id_to_first_block;
    temp_s2 = file_id_to_first_block[temp_v0];
    temp_s3 = temp_s2 << 0xB;
    temp_s2 = 0;
loop_1:
    temp_v0 = sceIoLseek(file_descriptor, (long long)temp_s3, temp_s2);
    temp_v0 = (s32) temp_v0;
    if ((u32)temp_v0 != temp_s3) {
        sceKernelDelayThreadCB(0x2710U);
        goto loop_1;
    }
    temp_a0 = *temp_s1;
    temp_v1 = temp_a0 * 0x800;
    movie_open_pos = temp_v1;
    var_a1 = (temp_a0 << 0xB) >> 0xB;
    if ((s32)(temp_v1) < 0) {
        var_a1 = (temp_v1 + 0x7FF) >> 0xB;
    }
    set_decryption_key(var_a1);
}