#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern "C" u8 func_eboot_0884FDC4(GameSys *, u16);

extern "C" int func_game_sub_09C15E90(LbEvent *event) {
    if (func_game_sub_09C14800(event, 0x14) == 1) {
        if (func_eboot_0884FDC4(GameSys::objectPtr, 0x2905) == 1) {
            return 1;
        }
    }
    return 0;
}
