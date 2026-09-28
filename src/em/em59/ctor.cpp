#include "lifecycle.hpp"

extern "C" Em59Actor *func_em59_09D15180(Em59Actor *actor) {
    func_game_task_09B62320(actor);
    actor->vtable = D_eboot_089BA160;
    return actor;
}
