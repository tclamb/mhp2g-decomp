#include "lifecycle.hpp"

extern "C" Em59Actor *func_em59_09D151B8(Em59Actor *actor, int flag) {
    if (actor != 0) {
        actor->vtable = D_eboot_089BA160;
        func_game_task_09B623B8(actor, 0);
        if ((short)flag > 0) {
            func_game_task_09B62408(actor);
        }
    }
    return actor;
}
