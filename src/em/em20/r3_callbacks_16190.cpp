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
extern "C" void func_em20_09D2B290(EmCallbackActor *actor) { actor->slot_24(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em20_09D2B2A0(EmCallbackActor *actor) { actor->slot_28(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em20_09D2B2B0(EmCallbackActor *actor) { actor->slot_28(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em20_09D2B2C0(EmCallbackActor *actor) { actor->slot_28(); }

// Empty lifecycle/state callback.
extern "C" void func_em20_09D2B2D0(void *) {}
