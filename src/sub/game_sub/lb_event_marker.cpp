#include "common.h"
#include "lb_event.hpp"

extern "C" u8 func_game_sub_09C17218(LbEvent *event) {
    return *(u8 *)((u8 *)event + 0x64);
}
