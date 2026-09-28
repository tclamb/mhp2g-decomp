#include "common.h"

extern "C" int func_game_sub_09C36718(void *object, u32 value, int count) {
    int index = 0;
    while (index < count) {
        if (value == *(u32 *)((u8 *)object + 0x258)) {
            return 1;
        }
        ++index;
        object = (u8 *)object + 4;
    }
    return 0;
}
