#include "lifecycle.hpp"

extern "C" void func_em59_09D15218(Em59Actor *actor, Em59Data *data, int mode) {
    actor->type = 0x28;
    actor->mode = mode;
    EM59_AT8(actor, 0x15) = data->value_0x1E6;
    EM59_AT8(actor, 0xF) = data->value_0x1E9;
    EM59_AT16(actor, 0xC2) = data->value_0x324;
    EM59_AT32(actor, 0x10) = (u32)data;
    actor->value_0x66 = data->value_0x1F4;
    switch (mode) {
    case 4:
    case 7:
        actor->value_0x66 += 0x8000;
        break;
    default:
        break;
    }
    EM59_AT32(actor, 0x90) = data->value_0x1F0;
    EM59_AT32(actor, 0x94) = data->value_0x1F4;
    EM59_AT32(actor, 0x98) = data->value_0x1F8;
    data->flag_0x33D = 0;
    data->flag_0x3F8 = 0;

    ShellManager *manager = Singleton<ShellManager>::objectPtr;
    if (manager->head == 0) {
        manager->head = actor;
    } else {
        actor->previous = manager->head;
        actor->next = 0;
        manager->head->next = actor;
        manager->head = actor;
    }
    manager->last = actor;
    manager->count++;
}
