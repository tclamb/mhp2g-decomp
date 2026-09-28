#include "vtable_13_lifecycle.hpp"

extern "C" Em75V13Actor *func_em75_09D46238(Em75V13Actor *actor, int flag) {
    if (actor != 0) {
        actor->vtable = D_eboot_089BA480;
        func_game_task_09B623B8(actor, 0);
        if ((short)flag > 0) {
            func_game_task_09B62408(actor);
        }
    }
    return actor;
}
