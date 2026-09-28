#pragma once

#include "common.h"
#include "file_sys.hpp"
#include "resource_manager.hpp"

struct GameTaskResourceState {
    u8 reserved[0x3C];
    u16 resource_file_id;
    u8 resource_load_state;
};

extern "C" u8 *func_game_task_09A5E418(GameTaskResourceState *, s32);

