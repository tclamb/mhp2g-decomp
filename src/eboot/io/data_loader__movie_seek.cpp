#include "io.hpp"

inline static s32 inline_fn(s32 arg0) {
    return arg0;
}

void data_loader::movie_seek(int arg1) {
    s32 new_var;
    s32 temp_s1;
    s32 temp_s2;

    new_var = arg1;
    temp_s1 = file_id_to_first_block[movie_file_id] << 0xB;
    temp_s2 = new_var + temp_s1;
    new_var = temp_s2;
    temp_s1 = (u16)new_var >> 0x1F;
loop_1:
    if (sceIoLseek(file_descriptor, new_var, temp_s1) == inline_fn(new_var)) {
        movie_open_pos = new_var;
        return;
    }

    sceKernelDelayThreadCB(0x2710U);
    goto loop_1;
}