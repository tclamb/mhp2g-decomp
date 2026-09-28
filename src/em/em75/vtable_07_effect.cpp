#include "vtable_07_lifecycle.hpp"

extern "C" void func_em75_09D1CC28(Em75V07Actor *actor) {
    EffectManager *manager = Singleton<EffectManager>::objectPtr;
    Em75EffectObject *effect = func_game_sub_09C2CBA0(manager, EM75_AT8(actor, 0x1E8));
    if (effect != 0) {
        func_game_sub_09C51EC8(effect);
        effect->owner = actor;
        manager = Singleton<EffectManager>::objectPtr;
        effect->next = 0;
        effect->previous = 0;
        if (manager->head != 0) {
            effect->previous = manager->head;
            manager->head->next = effect;
        }
        manager->head = effect;
        manager->count++;
    }
}
