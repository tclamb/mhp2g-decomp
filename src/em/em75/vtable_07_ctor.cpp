#include "vtable_07_lifecycle.hpp"

extern "C" Em75V07Actor *func_em75_09D1CB90(Em75V07Actor *actor) {
    func_game_sub_09C51DF0(actor);
    actor->vtable = D_eboot_089BA390;
    return actor;
}
