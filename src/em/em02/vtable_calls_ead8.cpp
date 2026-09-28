struct Em02Actor {
    virtual ~Em02Actor();
    virtual void vtable_0x0C();
    virtual void vtable_0x10();
    virtual void vtable_0x14();
    virtual void vtable_0x18();
    virtual void vtable_0x1C();
    virtual void vtable_0x20();
    virtual void vtable_0x24();
    virtual void vtable_0x28();
};

extern "C" void func_game_task_09B625A0(Em02Actor *);

extern "C" void func_em02_09D23BD8(Em02Actor *actor) {
    actor->vtable_0x24();
}

extern "C" void func_em02_09D23BE8(Em02Actor *actor) {
    actor->vtable_0x28();
}

extern "C" void func_em02_09D23BF8(Em02Actor *actor) {
    actor->vtable_0x28();
}

extern "C" void func_em02_09D23C08(Em02Actor *actor) {
    actor->vtable_0x28();
}

extern "C" void func_em02_09D23C18(Em02Actor *actor) {
    func_game_task_09B625A0(actor);
}

extern "C" void func_em02_09D23C20(Em02Actor *) {}
