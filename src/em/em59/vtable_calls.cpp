struct Em59Actor {
    virtual ~Em59Actor();
    virtual void vtable_0x0C();
    virtual void vtable_0x10();
    virtual void vtable_0x14();
    virtual void vtable_0x18();
    virtual void vtable_0x1C();
    virtual void vtable_0x20();
    virtual void vtable_0x24();
    virtual void vtable_0x28();
};

extern "C" void func_game_task_09B625A0(Em59Actor *);

extern "C" void func_em59_09D15730(Em59Actor *actor) {
    actor->vtable_0x24();
}

extern "C" void func_em59_09D15740(Em59Actor *actor) {
    actor->vtable_0x28();
}

extern "C" void func_em59_09D15750(Em59Actor *actor) {
    actor->vtable_0x28();
}

extern "C" void func_em59_09D15760(Em59Actor *actor) {
    actor->vtable_0x28();
}

extern "C" void func_em59_09D15770(Em59Actor *actor) {
    func_game_task_09B625A0(actor);
}

extern "C" void func_em59_09D15778(Em59Actor *) {}
