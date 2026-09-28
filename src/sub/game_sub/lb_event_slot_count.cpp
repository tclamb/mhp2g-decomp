#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern "C" int func_game_sub_09C16880(LbEvent *) {
    int count = 0;
    int index = 0;
    u8 *current = (u8 *)GameSys::objectPtr;
    do {
        if (*(s8 *)(current + 0x696FC) != 0) {
            ++count;
        }
        ++index;
        current += 0x70;
    } while (index < 5);
    return count;
}
