#include "common.h"
#include "game_sys.hpp"

extern "C" int func_game_sub_09C244B0(void *) {
    u8 value = *((u8 *)GameSys::objectPtr + 0x6A23A);
    if (value == 0) {
        return 0;
    }
    if (value < 3) {
        return 1;
    }
    return 2;
}
