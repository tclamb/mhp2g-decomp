#pragma once

#include "camera.hpp"
#include "cockpit.hpp"
#include "enemy_manager.hpp"
#include "evdemo.hpp"
#include "game_sys.hpp"
#include "game_task.hpp"
#include "light_manager.hpp"
#include "player_manager.hpp"
#include "stage_manager.hpp"

struct BgmServer : Singleton<BgmServer> {};
struct EffectManager : Singleton<EffectManager> {};
struct ShellManager : Singleton<ShellManager> {};

extern "C" void func_game_task_09A5E6C8(GameTask *);
extern "C" s32 func_eboot_088566DC(GameSys *);
extern "C" void func_game_task_09A5DA28(GameTask *);
extern "C" void func_game_sub_09C3CD68();
extern "C" void func_game_task_09A5DB38(GameTask *);
extern "C" void func_eboot_08856488(GameSys *);
extern "C" void func_game_task_09A5DB68(GameTask *);
extern "C" void func_eboot_088DD9B0(PlayerManager *);
extern "C" void func_eboot_088DF6DC(PlayerManager *);
extern "C" void func_game_task_09AAC5A8(EnemyManager *);
extern "C" void func_game_sub_09C32B58();
extern "C" void func_eboot_08885C54(BgmServer *);
extern "C" void func_game_task_09A5E5F8(GameTask *);
extern "C" void func_eboot_088DDC80(PlayerManager *);
extern "C" void func_eboot_088DDCE4(PlayerManager *);
extern "C" void method_08813990__6CameraFv(Camera *);
extern "C" void method_088607F0__12LightManagerFv(LightManager *);
extern "C" void func_game_sub_09C282D0(EffectManager *);
extern "C" void func_game_task_09B5DF38(ShellManager *);
extern "C" void call_prop_list_ptmf__12StageManagerFv(StageManager *);
extern "C" void func_game_task_09AAEB68(EnemyManager *);
extern "C" void func_game_task_09AAEB10(EnemyManager *);
extern "C" void call_stage_ptmf_0x3D8__12StageManagerFv(StageManager *);
extern "C" void func_eboot_0883F28C(Cockpit *);
extern "C" void func_eboot_088564B4(GameSys *);
extern "C" void func_eboot_088DD9E8(PlayerManager *);
extern "C" void func_game_task_09AAC5E8(EnemyManager *);
extern "C" void func_game_sub_09C28408(EffectManager *);
extern "C" void call_prop_list_vtable_0x10__12StageManagerFv(StageManager *);
extern "C" void func_game_task_09B5E048(ShellManager *, s32);
extern "C" void register_drawable__12StageManagerFv(StageManager *);

