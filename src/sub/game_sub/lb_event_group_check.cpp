#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern u16 *D_eboot_089AEFAC[];
extern u16 *D_eboot_089AEFD8[];
extern "C" u8 func_eboot_0884FDC4(GameSys *, u16);

extern "C" int func_game_sub_09C15AA0(LbEvent *, u8 mode, u8 group) {
    u16 *list;
    if (mode == 0) {
        list = D_eboot_089AEFAC[group];
    } else {
        list = D_eboot_089AEFD8[group];
    }

    for (u16 id = *list; id != 0; id = *++list) {
        if (func_eboot_0884FDC4(GameSys::objectPtr, id) == 0) {
            return 0;
        }
    }
    return 1;
}
