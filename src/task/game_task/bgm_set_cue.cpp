#include "bgm_set_cue.hpp"

extern "C" void func_game_task_09A5FBE0(BgmServer *self, u16 cue) {
    self->cue_0C = cue;
    self->cue_10 = 0x7F;
}
