#pragma once

#include "common.h"
#include "singleton.hpp"

struct Cockpit : Singleton<Cockpit> {
    u8 pad_0x0[0x1220];

    Cockpit();
    ~Cockpit();
};
