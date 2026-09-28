#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern u16 *D_eboot_089AEF80[6];
extern "C" u8 func_eboot_0884FDC4(GameSys *, u16);

extern "C" int func_game_sub_09C15B80(LbEvent *) {
    u16 **groups;
    int index = 0;
    groups = D_eboot_089AEF80;
    do {
        u16 *events = *groups;
        u16 event = *events;
        if (event != 0) {
            do {
                if (event != 0x290E && func_eboot_0884FDC4(GameSys::objectPtr, event) == 0) {
                    return 0;
                }
                event = *++events;
            } while (event != 0);
        }
        ++index;
        ++groups;
    } while (index < 6);
    return 1;
}
