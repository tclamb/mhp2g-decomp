#include "common.h"
#include "lb_event.hpp"

extern "C" void func_game_sub_09C17210(LbEvent *event) {
    *(u8 *)((u8 *)event + 0x64) = 0;
}
