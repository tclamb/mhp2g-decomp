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
extern "C" void func_em54_09D18ED0(EmCallbackActor *actor) { actor->slot_24(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em54_09D18EE0(EmCallbackActor *actor) { actor->slot_28(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em54_09D18EF0(EmCallbackActor *actor) { actor->slot_28(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em54_09D18F00(EmCallbackActor *actor) { actor->slot_28(); }
