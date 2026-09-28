// Original state restore global callback.
#include "game_sys.hpp"

extern "C" void func_em07_09D1C6A8(unsigned char *actor, unsigned char *record) {
    actor[0x790] = record[0x1F];
    signed char value = ((signed char *)record)[0x1E];
    actor[0x791] = value;
    ((unsigned char *)GameSys::objectPtr)[0x38F] = value;
}
