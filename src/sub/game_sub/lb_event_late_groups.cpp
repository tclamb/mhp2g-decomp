#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern u16 *D_eboot_089AEF98[3];
extern "C" u8 func_eboot_0884FDC4(GameSys *, u16);

extern "C" int func_game_sub_09C15C18(LbEvent *) {
    u16 **groups;
    int index = 6;
    groups = D_eboot_089AEF98;
    do {
        u16 *events = *groups;
        u16 event = *events;
        if (event != 0) {
            do {
                if (event != 0x2BDA && func_eboot_0884FDC4(GameSys::objectPtr, event) == 0) {
                    return 0;
                }
                event = *++events;
            } while (event != 0);
        }
        ++index;
        ++groups;
    } while (index < 9);
    return 1;
}
