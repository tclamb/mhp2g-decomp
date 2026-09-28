#include "bgm_mode.hpp"

extern "C" void func_game_task_09A60148(BgmServer *self, s32 mode) {
    if (mode == 0) {
        self->mode_4 = 0;
    } else if (mode == 1) {
        self->mode_4 = 9;
        self->value_0 = 0x96;
    } else if (mode == 2) {
        self->mode_4 = 0x14;
    }
}
