#include "lifecycle.hpp"

extern "C" Em55Actor *func_em55_09D15180(Em55Actor *actor) {
    func_game_task_09B62320(actor);
    actor->vtable = D_eboot_089B9E90;
    return actor;
}
