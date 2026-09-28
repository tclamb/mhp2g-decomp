#include "common.h"
#include "lb_event.hpp"

extern "C" void func_game_sub_09C172A0(LbEvent *event) {
    *(u8 *)((u8 *)event + 0x65) = 1;
    *(u32 *)((u8 *)event + 0x68) = 0;
}
