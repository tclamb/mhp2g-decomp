#include "vtable_07_lifecycle.hpp"

extern "C" Em75V07Actor *func_em75_09D1CBC8(Em75V07Actor *actor, int flag) {
    if (actor != 0) {
        actor->vtable = D_eboot_089BA390;
        func_game_sub_09C51E10(actor, 0);
        if ((short)flag > 0) {
            func_game_sub_09C51EC0(actor);
        }
    }
    return actor;
}
