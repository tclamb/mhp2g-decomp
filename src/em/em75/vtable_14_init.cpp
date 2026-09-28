#include "vtable_14_lifecycle.hpp"

extern "C" void func_em75_09D471C0(Em75V14Actor *actor, u8 *data, u8 mode) {
    EM75_AT8(actor, 0xC) = 0x2B;
    EM75_AT8(actor, 0xD) = mode;
    EM75_AT8(actor, 0x15) = EM75_AT16(data, 0x1E6);
    EM75_AT8(actor, 0xF) = EM75_AT8(data, 0x1E9);
    EM75_AT16(actor, 0xC2) = EM75_AT16(data, 0x324);
    EM75_AT32(actor, 0x10) = (u32)data;
    EM75_AT16(actor, 0x66) = EM75_AT32(data, 0x1F4);
    EM75_AT32(actor, 0x90) = EM75_AT32(data, 0x1F0);
    EM75_AT32(actor, 0x94) = EM75_AT32(data, 0x1F4);
    EM75_AT32(actor, 0x98) = EM75_AT32(data, 0x1F8);
    EM75_ATF(actor, 0x40) = EM75_ATF(data, 0x200);
    EM75_ATF(actor, 0x44) = EM75_ATF(data, 0x204);
    EM75_ATF(actor, 0x48) = EM75_ATF(data, 0x208);
    EM75_AT8(data, 0x33D) = 0;
    EM75_AT8(data, 0x3F8) = 0;

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
