#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern "C" int func_game_sub_09C16880(LbEvent *);

extern "C" void func_game_sub_09C17100(LbEvent *event, int index) {
    if (index < func_game_sub_09C16880(event)) {
        u32 offset = index * 0x70;
        u8 *address = (u8 *)(offset + (u32)GameSys::objectPtr);
        *(s8 *)((u8 *)event + 2) = *(s8 *)(address + 0x696FC);
        u8 *nextAddress = (u8 *)(offset + (u32)GameSys::objectPtr);
        *(s8 *)(nextAddress + 0x696FC) = 4;
    }
}
