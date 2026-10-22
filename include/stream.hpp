#pragma once

#include "common.h"
#include "singleton.hpp"

struct Stream : Singleton<Stream> {
    u32 unknown_0x0;
    u32 unknown_0x4;
    u32 unknown_0x8;
    u16 unknown_0xC;
    u16 unknown_0xE;
    u16 unknown_0x10;
    u16 unknown_0x12;
    u16 unknown_0x14;
    u16 unknown_0x16;
    u32 unknown_0x18;
    u8 pad_0x1C[0x14];
    u32 unknown_0x30;
    u32 unknown_0x34;
    u32 unknown_0x38;
    u16 unknown_0x3C;
};

extern "C" {
    void func_eboot_08886350(Stream *, int);
    void func_eboot_0888633C(Stream *, int, u16, u32);
    u32 func_eboot_08886368(Stream *, int);
}
