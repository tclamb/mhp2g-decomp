// Monster lifecycle and state callbacks.

struct EmCallbackActor {
    virtual ~EmCallbackActor();
    virtual void slot_0C();
    virtual void slot_10();
    virtual void slot_14();
    virtual void slot_18();
    virtual void slot_1C();
    virtual void slot_20();
    virtual void slot_24();
    virtual void slot_28();
};

// Dispatches virtual slot 0x24.
extern "C" void func_em82_09D2B6F0(EmCallbackActor *actor) { actor->slot_24(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em82_09D2B700(EmCallbackActor *actor) { actor->slot_28(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em82_09D2B710(EmCallbackActor *actor) { actor->slot_28(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em82_09D2B720(EmCallbackActor *actor) { actor->slot_28(); }

// Empty lifecycle/state callback.
extern "C" void func_em82_09D2B730(void *) {}
