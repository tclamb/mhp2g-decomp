#include "vtable_14_lifecycle.hpp"

extern "C" Em75V14Actor *func_em75_09D47160(Em75V14Actor *actor, int flag) {
    if (actor != 0) {
        actor->vtable = D_eboot_089BA4C4;
        func_game_task_09B623B8(actor, 0);
        if ((short)flag > 0) {
            func_game_task_09B62408(actor);
        }
    }
    return actor;
}
