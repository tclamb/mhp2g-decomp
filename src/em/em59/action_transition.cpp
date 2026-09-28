typedef unsigned char u8;
typedef unsigned int u32;

// Placeholder methods place action at the observed vtable slot 0x88.
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
    virtual void unknown_23();
    virtual void unknown_24();
    virtual void unknown_25();
    virtual void unknown_26();
    virtual void unknown_27();
    virtual void unknown_28();
    virtual void unknown_29();
    virtual void unknown_30();
    virtual void unknown_31();
    virtual void action(int, int, int);
};

extern "C" void func_em59_09D18F28(Em59Actor *actor) {
    u8 *bytes = (u8 *)actor;
    *(u32 *)(bytes + 0x410) &= ~2u;
    bytes[0x4B5] = 1;
    *(float *)(bytes + 0x4B0) = 1.0f;
    bytes[0x280] = 0;
    actor->action(0, 1, 0);
}
