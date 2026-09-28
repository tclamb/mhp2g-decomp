#include "bgm_resolve.hpp"

extern "C" u16 func_game_task_09A603D8(u8 *self) {
    u16 original = *(u16 *)(self + 0xC);
    u16 result = original;
    for (s32 index = 0; index < 2; ++index, self += 2) {
        if (original == *(u16 *)(self + 0x1C)) {
            result = D_game_sub_09CDF7B8[StageManager::objectPtr->stage_id].first;
        }
    }
    return result;
}
