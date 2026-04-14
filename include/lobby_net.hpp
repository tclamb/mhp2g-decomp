#pragma once

#include "common.h"
#include "singleton.hpp"

struct LobbyNet : Singleton<LobbyNet> {
    u8 pad_0x0[4];
    s8 unknown_0x4;
    u8 pad_0x5[0x1B63];

    LobbyNet();
};
