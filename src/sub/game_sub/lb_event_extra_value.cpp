#include "common.h"
#include "lb_event.hpp"

extern "C" u32 func_game_sub_09C16970(LbEvent *event, int index) {
    u32 result;
    if (index >= 3) {
        result = 0;
    } else {
        u32 offset = index * 12;
        u32 address = offset + (u32)event;
        result = *(u32 *)(address + 0x40);
    }
    return result;
}
