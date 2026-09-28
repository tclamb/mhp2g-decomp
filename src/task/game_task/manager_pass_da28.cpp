#include "manager_pass.hpp"

extern "C" void func_game_task_09A5DA28(GameTask *task) {
    if (*(u32 *)((u8 *)Evdemo::objectPtr + 0x88) == 0) {
        func_eboot_088DD9B0(PlayerManager::objectPtr);
    } else {
        func_eboot_088DF6DC(PlayerManager::objectPtr);
    }
    func_game_task_09AAC5A8(EnemyManager::objectPtr);
    if (*(u32 *)((u8 *)Evdemo::objectPtr + 0x88) == 0) {
        func_game_sub_09C32B58();
    }
    func_eboot_08885C54(BgmServer::objectPtr);
    func_game_task_09A5E5F8(task);
    func_eboot_088DDC80(PlayerManager::objectPtr);
    func_eboot_088DDCE4(PlayerManager::objectPtr);
    method_08813990__6CameraFv(Camera::objectPtr);
    method_088607F0__12LightManagerFv(LightManager::objectPtr);
    func_game_sub_09C282D0(EffectManager::objectPtr);
    func_game_task_09B5DF38(ShellManager::objectPtr);
    call_prop_list_ptmf__12StageManagerFv(StageManager::objectPtr);
    func_game_task_09AAEB68(EnemyManager::objectPtr);
    func_game_task_09AAEB10(EnemyManager::objectPtr);
    call_stage_ptmf_0x3D8__12StageManagerFv(StageManager::objectPtr);
    func_eboot_0883F28C(Cockpit::objectPtr);
}
