#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern u8 D_game_sub_09CCA110[];
extern u8 D_game_sub_09CCA118[];
extern "C" u8 func_eboot_0884FF88(GameSys *, u8, u8);

extern "C" int func_game_sub_09C15DB8(LbEvent *) {
    int index = 0;
    u8 *events = D_game_sub_09CCA110;
    do {
        if (func_eboot_0884FF88(GameSys::objectPtr, *events, 3) == 0) {
            return 0;
        }
        ++index;
        ++events;
    } while (index < 3);

    index = 0;
    events = D_game_sub_09CCA118;
    do {
        if (func_eboot_0884FF88(GameSys::objectPtr, *events, 5) == 0) {
            return 0;
        }
        ++index;
        ++events;
    } while (index < 4);
    return 1;
}
