#include "common.h"

extern "C" int func_game_sub_09C366B0(void *object, u32 value) {
    int count = *(int *)((u8 *)object + 0x254);
    int index = 0;
    while (index < count) {
        if (value == *(u32 *)((u8 *)object + 0x22C)) {
            return 1;
        }
        ++index;
        object = (u8 *)object + 4;
    }
    return 0;
}
