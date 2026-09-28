#include "common.h"
#include "lb_event.hpp"

extern "C" void func_game_sub_09C17440(LbEvent *event) {
    *(u8 *)((u8 *)event + 0xC) = 0;
    *(u8 *)((u8 *)event + 0x18) = 0;
    *(u8 *)((u8 *)event + 0x24) = 0;
    *(u8 *)((u8 *)event + 0x30) = 0;
    *(u8 *)((u8 *)event + 0x3C) = 0;
}
