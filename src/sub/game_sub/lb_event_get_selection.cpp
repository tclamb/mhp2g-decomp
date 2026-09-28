#include "common.h"
#include "lb_event.hpp"

extern "C" u32 func_game_sub_09C17388(LbEvent *event) {
    return *(u32 *)((u8 *)event + 0x6C);
}
