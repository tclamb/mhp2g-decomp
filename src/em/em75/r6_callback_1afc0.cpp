// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em75_09D2DFB8(u8 *);
extern "C" void func_em75_09D2E278(u8 *);
extern "C" void func_em75_09D2E4F0(u8 *);
extern "C" void func_em75_09D2E838(u8 *);
extern "C" void func_em75_09D2EA88(u8 *);

extern "C" void func_em75_09D300C0(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em75_09D2DFB8(actor);
        break;
    case 1:
        func_em75_09D2E278(actor);
        break;
    case 2:
        func_em75_09D2E4F0(actor);
        break;
    case 3:
        func_em75_09D2E838(actor);
        break;
    case 4:
        func_em75_09D2EA88(actor);
        break;
    }
}
