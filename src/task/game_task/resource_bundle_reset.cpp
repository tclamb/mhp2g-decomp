#include "resource_bundle.hpp"

extern "C" void func_game_task_09A5E3D8(GameTaskResourceState *task) {
    ResourceManager::objectPtr->free_all(4);
    task->resource_load_state = 0;
    task->resource_file_id = 0xFFFF;
}

