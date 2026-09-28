#include "post_resource.hpp"

extern "C" void func_game_task_09A5E5F8(GameTask *) {
    for (s32 i = 0; i < 4; ++i) {
        Player *player = method_088DF804__13PlayerManagerFi(PlayerManager::objectPtr, i);
        if (player != 0) {
            u8 active = (player->flags & 0x80) != 0;
            if (active) {
                func_game_sub_09C3C6D0(player, 1);
            }
        }
    }
    for (s32 i = 0; i < 20; ++i) {
        Enemy *enemy = EnemyManager::objectPtr->by_index(i);
        if (enemy != 0) {
            u8 active = (enemy->flags & 0x80) != 0;
            if (active) {
                func_game_sub_09C3C6D0(enemy, 1);
            }
        }
    }
}
