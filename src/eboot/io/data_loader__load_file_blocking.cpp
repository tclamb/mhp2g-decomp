#include "io.hpp"

inline static u32 inline_fn(u32 arg0) {
    return arg0;
}

u32 data_loader::load_file_blocking(s32 arg1, u8 *arg2, u32 arg3) {
    s32 temp_s0;
    s32 temp_s5;
    u32 var_s1;
    
    u8 *new_var2;
    u32 new_var3;

    sceKernelWaitSema(fake_rofs_semaphore, 1, 0);
    temp_s0 = file_id_to_first_block[arg1 & 0xFFFF] << 0xB;
    temp_s5 = ((u8)temp_s0) >> 0x1F;
loop_1:
    if (sceIoLseek(file_descriptor, temp_s0, temp_s5) != inline_fn(temp_s0)) {
        sceKernelDelayThreadCB(0x2710U);
        goto loop_1;
    }
    var_s1 = file_blocks_size(arg1);
    if (arg3 != -1U) {
        var_s1 = arg3;
    }
    new_var2 = arg2;
    new_var3 = var_s1;
loop_7:
    if (sceIoRead(file_descriptor, new_var2, new_var3) != inline_fn(new_var3)) {
        sceKernelDelayThreadCB(0x2710U);
        goto loop_7;
    }
    var_s1 = temp_s0 >> 0xB;
    if (temp_s0 < 0) {
        var_s1 = (temp_s0 + 0x7FF) >> 0xB;
    }
    set_decryption_key(var_s1);
    decrypt_buffer(new_var2, new_var3, 0);
    sceKernelSignalSema(fake_rofs_semaphore, 1);
    return new_var3;
}