#include "post_resource.hpp"

extern "C" s32 func_game_task_09A5E4F0(GameTask *task) {
    GameSys *game = GameSys::objectPtr;
    bool enabled = (*(u32 *)((u8 *)game + 0x6AF14) & 1) != 0;
    if (enabled == 1) {
        return 0;
    }
    Player *player = method_088DF804__13PlayerManagerFi(
        PlayerManager::objectPtr, *(u8 *)((u8 *)game + 0x28));
    if (player == 0) {
        return 0;
    }
    u8 check;
    if (player->motion_2E4 <= 0) goto common;
    if (player->action_298 == 4) goto common;
    check = func_eboot_0884F9A0(GameSys::objectPtr, 1);
    if (check != 1) goto alternate;
common:
    if (player->flag_2A4 == 1) {
        player->flag_2A4 = 0;
    }
    return 0;
alternate:
    if (player->flag_2A4 != 0) {
        return 0;
    }
    if (player->flag_2E6 != 0) {
        return 0;
    }
    void *exit = intersecting_exit__12StageManagerFP14ScePspFVector4(
        StageManager::objectPtr, (u8 *)player + 0x200);
    *(void **)((u8 *)task + 0x4C) = exit;
    return exit != 0;
}
