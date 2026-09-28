#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern "C" int func_eboot_08854B84(GameSys *, u32);
extern "C" int func_eboot_08854B14(GameSys *, u32);

extern "C" int func_game_sub_09C15B38(LbEvent *, u8 mode, u8 index) {
    if (mode == 0) {
        return func_eboot_08854B84(GameSys::objectPtr, index);
    }
    return func_eboot_08854B14(GameSys::objectPtr, index);
}
