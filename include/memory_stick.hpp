#pragma once

#include "common.h"

struct MemoryStick {
    u8 padding_0x0[0x970];
    bool flag_0x970;
    bool flag_0x971;
    u8 padding_0x972[0x4A90 - 0x972];
} __attribute__((aligned(0x10)));

extern "C" {
    void func_eboot_088C28C0(MemoryStick *, char *);
}
