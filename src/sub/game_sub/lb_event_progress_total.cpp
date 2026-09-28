#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern u8 D_game_sub_09CCA100[];
extern "C" u32 func_eboot_08850464(GameSys *, u8);

extern "C" int func_game_sub_09C15D40(LbEvent *) {
    int total = 0;
    int index = 0;
    u8 *events = D_game_sub_09CCA100;
    do {
        total += func_eboot_08850464(GameSys::objectPtr, *events) & 0xFFFF;
        if (total >= 100) {
            return 1;
        }
        ++index;
        ++events;
    } while (index < 16);
    return 0;
}
