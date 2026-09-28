#include "common.h"
#include "lb_event.hpp"

extern "C" int func_game_sub_09C16880(LbEvent *);

extern "C" void func_game_sub_09C173E8(LbEvent *event, int index) {
    if (index < func_game_sub_09C16880(event)) {
        u32 offset = index * 12;
        u32 address = offset + (u32)event;
        *(u8 *)(address + 0xC) = 1;
    }
}
