#include "vtable_13_lifecycle.hpp"

extern "C" Em75V13Actor *func_em75_09D46200(Em75V13Actor *actor) {
    func_game_task_09B62320(actor);
    actor->vtable = D_eboot_089BA480;
    return actor;
}
