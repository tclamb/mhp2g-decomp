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
extern "C" void func_em21_09D25930(EmCallbackActor *actor) { actor->slot_24(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em21_09D25940(EmCallbackActor *actor) { actor->slot_28(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em21_09D25950(EmCallbackActor *actor) { actor->slot_28(); }

// Dispatches virtual slot 0x28.
extern "C" void func_em21_09D25960(EmCallbackActor *actor) { actor->slot_28(); }
