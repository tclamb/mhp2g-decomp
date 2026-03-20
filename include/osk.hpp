#pragma once

#include "common.h"

struct Osk {
    u8 pad_0x0[0x74];
    u8 unknown_0x74;
    u8 pad_0x75[0x7C - 0x75];

    inline Osk() {
        unknown_0x74 = 0;
    }
};
