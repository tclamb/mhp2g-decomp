#include "io.hpp"

void data_loader::movie_read(u8* buf, SceSize len) {
    u32 uVar1;
    u8 *new_var;

    uVar1 = sceUmdGetDriveStat() & 0x20;
    while ((uVar1 > 0) ^ 1)
    {
        sceKernelDelayThreadCB(10000);
        uVar1 = sceUmdGetDriveStat() & 0x20;
    }

    new_var = buf;
loop_2:
    uVar1 = sceIoRead(file_descriptor, new_var, len);
    if (uVar1 != len)
    {
        sceKernelDelayThreadCB(10000);
        goto loop_2;
    }

    movie_open_pos += len;
}