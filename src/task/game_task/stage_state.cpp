#include "stage_state.hpp"

extern "C" void func_game_task_09A5E920(GameTask *task) {
    u16 stage = *(u16 *)((u8 *)GameSys::objectPtr + 0x6AF0E);
    if (stage == 0x39) goto set_flag;
    u16 mapped;
    if (stage == 0xFFFF) mapped = stage;
    else mapped = select_stage(stage);
    if ((u16)mapped != 0x100) return;
set_flag:
    task->flag_6C = 1;
}
