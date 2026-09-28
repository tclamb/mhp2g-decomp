#include "manager_pass.hpp"

extern "C" void func_game_task_09A5DB68(GameTask *) {
    if (*(u32 *)((u8 *)Evdemo::objectPtr + 0x88) == 0) {
        func_eboot_088DD9E8(PlayerManager::objectPtr);
    }
    func_game_task_09AAC5E8(EnemyManager::objectPtr);
    func_game_sub_09C28408(EffectManager::objectPtr);
    call_prop_list_vtable_0x10__12StageManagerFv(StageManager::objectPtr);
    s32 game_state = func_eboot_088566DC(GameSys::objectPtr);
    func_game_task_09B5E048(ShellManager::objectPtr, game_state);
    register_drawable__12StageManagerFv(StageManager::objectPtr);
}
