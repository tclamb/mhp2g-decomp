#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern u16 *D_game_sub_09CCA210[];
extern u8 D_game_sub_09CCA238[];
extern "C" u8 func_eboot_0884FDC4(GameSys *, u16);

extern "C" int func_game_sub_09C15CB0(LbEvent *, u32 group) {
    u16 *events = D_game_sub_09CCA210[group];
    int count = D_game_sub_09CCA238[group];
    int index = 0;
    if (count > 0) {
        do {
            if (func_eboot_0884FDC4(GameSys::objectPtr, *events) == 0) {
                return 0;
            }
            ++index;
            ++events;
        } while (index < count);
    }
    return 1;
}
