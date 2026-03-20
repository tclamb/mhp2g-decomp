#pragma once

#include "common.h"
#include "singleton.hpp"

struct ObjManager : Singleton<ObjManager> {
    u8 pad_0x0[0x80468];

    ObjManager();
    ~ObjManager();
};
