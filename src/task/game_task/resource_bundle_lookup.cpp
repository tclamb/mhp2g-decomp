#include "resource_bundle.hpp"

extern "C" void *memcpy(void *destination, const void *source, u32 size);

extern "C" u8 *func_game_task_09A5E460(GameTaskResourceState *task, s32 table_index, u16 string_id) {
    u8 *base = func_game_task_09A5E418(task, table_index);
    if (string_id == 0xFFFF) {
        return 0;
    }
    s32 offset;
    memcpy(&offset, base + string_id * 4, 4);
    if (offset == -1) {
        return 0;
    }
    return base + offset;
}

