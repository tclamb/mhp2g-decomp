#include "lifecycle.hpp"

extern "C" void func_em55_09D15218(Em55Actor *actor, u8 *data, u8 mode) {
    actor->type = 0x1F;
    actor->mode = mode;
    EM55_AT8(actor, 0x15) = EM55_AT16(data, 0x1E6);
    EM55_AT8(actor, 0xF) = EM55_AT8(data, 0x1E9);
    EM55_AT16(actor, 0xC2) = EM55_AT16(data, 0x324);
    EM55_AT32(actor, 0x10) = (u32)data;
    EM55_AT16(actor, 0x66) = EM55_AT32(data, 0x1F4);
    EM55_ATF(actor, 0x40) = EM55_ATF(data, 0x200);
    EM55_ATF(actor, 0x44) = EM55_ATF(data, 0x204);
    EM55_ATF(actor, 0x48) = EM55_ATF(data, 0x208);
    EM55_AT32(actor, 0x90) = EM55_AT32(data, 0x1F0);
    EM55_AT32(actor, 0x94) = EM55_AT32(data, 0x1F4);
    EM55_AT32(actor, 0x98) = EM55_AT32(data, 0x1F8);
    EM55_AT8(data, 0x33D) = 0;
    EM55_AT8(data, 0x3F8) = 0;

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
