#include "common.h"
#include "game_sys.hpp"

extern "C" int func_game_sub_09C24680(void *) {
    u8 value = *((u8 *)GameSys::objectPtr + 0x6A23F);
    return value > 0;
}
