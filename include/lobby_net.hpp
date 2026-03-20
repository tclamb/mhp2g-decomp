#pragma once

#include "common.h"

struct LobbyNet {
    u8 pad_0x0[4];
    s8 unknown_0x4;
    u8 pad_0x5[0x1B63];

    inline LobbyNet() {
        unknown_0x4 = 0;
    }
};
