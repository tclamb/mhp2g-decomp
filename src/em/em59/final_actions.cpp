typedef unsigned char u8;

// The unnamed virtual methods preserve the observed 0x64 action_state slot.
struct Em59Actor {
    virtual ~Em59Actor();
    virtual void unknown_01();
    virtual void unknown_02();
    virtual void unknown_03();
    virtual void unknown_04();
    virtual void unknown_05();
    virtual void unknown_06();
    virtual void unknown_07();
    virtual void unknown_08();
    virtual void unknown_09();
    virtual void unknown_10();
    virtual void unknown_11();
    virtual void unknown_12();
    virtual void unknown_13();
    virtual void unknown_14();
    virtual void unknown_15();
    virtual void unknown_16();
    virtual void unknown_17();
    virtual void unknown_18();
    virtual void unknown_19();
    virtual void unknown_20();
    virtual void unknown_21();
    virtual void unknown_22();
    virtual u8 action_state();
};

extern "C" u8 func_em59_09D18A30(Em59Actor *);

extern "C" void func_em59_09D21980(Em59Actor *) {}

extern "C" int func_em59_09D21988(Em59Actor *actor) {
    return func_em59_09D18A30(actor) == 2;
}

extern "C" int func_em59_09D219B0(Em59Actor *actor) {
    return actor->action_state() == 1 ? 0 : 0xFF;
}
