#include "common.h"

// Fields used by the five handlers at 0x09D49C90-0x09D49D20.
struct Em75Actor {
    virtual ~Em75Actor();
    virtual void vtable_0x0C();
    virtual void vtable_0x10();
    virtual void vtable_0x14();
    virtual void vtable_0x18();
    virtual void vtable_0x1C();
    virtual void vtable_0x20();
    virtual void vtable_0x24();

    u8 unknown_0x4[0x9];
    u8 mode_0xD;
    u8 unknown_0xE[0xF];
    u8 flag_0x1D;
    u8 unknown_0x1E[0x135];
    u8 flag_0x153;
};

extern "C" void func_game_task_09B625A0(Em75Actor *);

extern "C" void func_em75_09D49C90(Em75Actor *actor) {
    switch (actor->mode_0xD) {
    case 3:
        actor->flag_0x1D = 0;
        actor->flag_0x153 = 1;
        actor->vtable_0x24();
        break;
    default:
        actor->vtable_0x24();
        break;
    }
}

extern "C" void func_em75_09D49CE8(Em75Actor *actor) {
    actor->vtable_0x24();
}

extern "C" void func_em75_09D49CF8(Em75Actor *actor) {
    actor->vtable_0x24();
}

extern "C" void func_em75_09D49D08(Em75Actor *actor) {
    actor->vtable_0x24();
}

extern "C" void func_em75_09D49D18(Em75Actor *actor) {
    func_game_task_09B625A0(actor);
}
