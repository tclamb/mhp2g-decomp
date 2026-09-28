#include "stage_manager.hpp"
#include "draw_manager.hpp"
extern "C" base_prop::ptmf ptmf_game_sub_09CF3C40;
static inline bool same_state(base_prop *self, base_prop::ptmf target) { return self->ptmf_0x1C == target; }
extern "C" void func_game_sub_09CBC7A8(base_prop *self) {
    s16 state = StageManager::objectPtr->stage->flash_state;
    if ((state == 1 || state == 2) == true) return;
    if (same_state(self, ptmf_game_sub_09CF3C40) == true) {
        DrawManager::objectPtr->add(8, self, (ScePspFVector4 *)((u8 *)self + 0x40), false);
    }
}
