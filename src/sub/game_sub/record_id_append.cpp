#include "common.h"

struct RecordIdList {
    u8 unknown[0x22C];
    u32 values[10];
    int count;
};

extern "C" void func_game_sub_09C366E8(void *object, u32 value) {
    RecordIdList *list = (RecordIdList *)object;
    if (list->count < 10) {
        list->values[list->count] = value;
        list->count++;
    }
}
