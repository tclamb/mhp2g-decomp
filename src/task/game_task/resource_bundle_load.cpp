#include "resource_bundle.hpp"

extern "C" u8 func_game_task_09A5E2C0(GameTaskResourceState *task) {
    u8 loaded = 0;
    switch (task->resource_load_state) {
    case 0:
        if (task->resource_file_id != 0x1294) {
            if (ResourceManager::objectPtr->find(4) != 0) {
                ResourceManager::objectPtr->free_all(4);
            }
            u32 size = FileSys::objectPtr->file_size(0x1294);
            u8 *buffer = ResourceManager::objectPtr->alloc(4, size);
            FileSys::objectPtr->load_file_async(0x1294, buffer, (u32)-1, 0, 0, 1);
            task->resource_file_id = 0x1294;
            ++task->resource_load_state;
        } else {
            loaded = 1;
        }
        break;
    case 1:
        if (!FileSys::objectPtr->is_loading()) {
            task->resource_load_state = 0;
            loaded = 1;
        }
        break;
    }
    return loaded;
}

