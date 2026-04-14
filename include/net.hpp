#pragma once

#include "common.h"
#include "singleton.hpp"

struct Net : Singleton<Net> {
    u32 event;
    u32 error;
    u32 unknown_0x8;
    u8 unknown_0xC;
    u8 powerSuspending;

    Net();
};
