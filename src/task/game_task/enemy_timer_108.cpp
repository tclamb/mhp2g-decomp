#include "enemy_timers.hpp"

// per frame from func_game_task_09A5DA28 (after the camera/light/effect updates)
extern "C" void func_game_task_09AAEB10(EnemyManagerTimers *self) {
    for (int i = 0; i < 32; i++) {
        EnemyTimedObj *obj = self->list_108[i];
        if (obj != 0) {
            if (--obj->timer <= 0) {
                self->list_108[i] = 0;
                self->count_209--;
            }
        }
    }
}
