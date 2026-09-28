#include "common.h"
#include "lb_event.hpp"

extern "C" int func_game_sub_09C16880(LbEvent *);

extern "C" u32 func_game_sub_09C168C0(LbEvent *event, int index) {
    u32 result;
    if (index >= func_game_sub_09C16880(event)) {
        result = 0;
    } else {
        u32 offset = index * 12;
        u32 address = offset + (u32)event;
        result = *(u32 *)(address + 4);
    }
    return result;
}
