#include "stage_manager.hpp"
#include "draw_manager.hpp"
extern "C" base_prop::ptmf ptmf_game_sub_09CF3478;
static inline bool same_state(base_prop *self, base_prop::ptmf target) { return self->ptmf_0x1C == target; }
extern "C" void func_game_sub_09CB6068(base_prop *self) {
    if (same_state(self, ptmf_game_sub_09CF3478) == true) {
        DrawManager::objectPtr->add(7, self, (ScePspFVector4 *)((u8 *)self + 0x40), false);
    }
}
