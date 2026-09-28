#include "vtable_14_lifecycle.hpp"

extern "C" Em75V14Actor *func_em75_09D47120(Em75V14Actor *actor) {
    func_game_task_09B62320(actor);
    actor->vtable = D_eboot_089BA4C4;
    EM75_AT32(actor, 0xE0) = -1;
    return actor;
}
