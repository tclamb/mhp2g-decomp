#include "common.h"
#include "lb_event.hpp"

extern "C" void func_game_sub_09C17370(LbEvent *event, int index) {
    if (index < 8) {
        *(u32 *)((u8 *)event + 0x6C) = index;
    }
}
