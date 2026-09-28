#include "common.h"
#include "lb_event.hpp"

extern "C" u32 func_game_sub_09C16598(LbEvent *);

extern "C" int func_game_sub_09C172D8(LbEvent *event) {
    u32 expected = func_game_sub_09C16598(event);
    return *(u32 *)((u8 *)event + 0x68) == expected;
}
