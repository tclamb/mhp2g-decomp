#pragma once

#include "common.h"
#include "singleton.hpp"

struct LbEvent : Singleton<LbEvent> {
    // TODO

    LbEvent();
};

extern "C" {
    u8 func_game_sub_09C14800(LbEvent *, u32);
}
