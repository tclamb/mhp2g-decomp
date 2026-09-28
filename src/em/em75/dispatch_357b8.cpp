typedef unsigned int u32;

struct Em75VirtualActor {
    virtual ~Em75VirtualActor();
    virtual void vtable_0x0C();
    virtual void vtable_0x10();
    virtual void vtable_0x14();
    virtual void vtable_0x18();
    virtual void vtable_0x1C();
    virtual void vtable_0x20();
    virtual void vtable_0x24();
};

struct Em75CtorActor {
    u32 *vtable;
};

extern "C" void func_game_task_09B625A0(Em75VirtualActor *);
extern "C" void func_game_task_09B62320(Em75CtorActor *);
extern "C" u32 D_eboot_089BA590[];

extern "C" void func_em75_09D4A8B8(Em75VirtualActor *actor) {
    actor->vtable_0x24();
}

extern "C" void func_em75_09D4A8C8(Em75VirtualActor *actor) {
    actor->vtable_0x24();
}

extern "C" void func_em75_09D4A8D8(Em75VirtualActor *actor) {
    actor->vtable_0x24();
}

extern "C" void func_em75_09D4A8E8(Em75VirtualActor *actor) {
    func_game_task_09B625A0(actor);
}

extern "C" Em75CtorActor *func_em75_09D4A8F0(Em75CtorActor *actor) {
    func_game_task_09B62320(actor);
    actor->vtable = D_eboot_089BA590;
    return actor;
}
