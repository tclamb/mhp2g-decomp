// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em21_09D21790(u8 *);
extern "C" void func_em21_09D219A8(u8 *);
extern "C" void func_em21_09D21B98(u8 *);
extern "C" void func_em21_09D22010(u8 *);
extern "C" void func_em21_09D22260(u8 *);

extern "C" void func_em21_09D22BB0(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em21_09D21790(actor);
        break;
    case 1:
        func_em21_09D219A8(actor);
        break;
    case 2:
        func_em21_09D21B98(actor);
        break;
    case 3:
        func_em21_09D22010(actor);
        break;
    case 4:
        func_em21_09D22260(actor);
        break;
    }
}
