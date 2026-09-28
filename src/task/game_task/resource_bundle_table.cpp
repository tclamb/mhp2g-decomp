#include "resource_bundle.hpp"

extern "C" u8 *func_game_task_09A5E418(GameTaskResourceState *, s32 table_index) {
    u8 *base = ResourceManager::objectPtr->find(4);
    s32 adjusted_index = table_index + 2;
    u32 byte_offset = adjusted_index << 2;
    u32 item_offset = *(u32 *)(base + byte_offset);
    return base + item_offset;
}

