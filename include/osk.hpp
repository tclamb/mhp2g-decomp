#pragma once

#include "common.h"
#include "singleton.hpp"

struct Osk : Singleton<Osk> {
    u32 unknown_0x0;
    u32 unknown_0x4;
    u32 unknown_0x8;
    u32 unknown_0xC;
    u32 unknown_0x10;
    u32 unknown_0x14;
    u32 unknown_0x18;
    u8 pad_0x1C[0x74 - 0x1C];
    u8 unknown_0x74;
    u8 pad_0x75[0x7C - 0x75];

    Osk();
};
