#include "common.h"
#include "lb_event.hpp"

extern "C" void func_game_sub_09C172C8(LbEvent *event) {
    ++*(u32 *)((u8 *)event + 0x68);
}
