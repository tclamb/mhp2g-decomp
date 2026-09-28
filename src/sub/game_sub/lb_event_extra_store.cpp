#include "common.h"
#include "lb_event.hpp"

extern "C" void func_game_sub_09C16A08(LbEvent *event, int index, u32 value) {
    if (index < 3) {
        u32 offset = index * 12;
        u32 address = offset + (u32)event;
        *(u32 *)(address + 0x44) = value;
    }
}
