// EnemyManager per-frame update (called from func_game_task_09A5DA28): runs the manager's current
// state method (pointer-to-member at +0x1214).
#include "common.h"

struct EnemyManagerState {
    typedef void (EnemyManagerState::*ptmf)(void);
    u8 pad_0x0[0x1214];
    ptmf state_1214;

    inline void run_state() {
        if (state_1214 != 0) {
            (this->*state_1214)();
        }
    }
};

extern "C" void func_game_task_09AAC5A8(EnemyManagerState *self) {
    self->run_state();
}
