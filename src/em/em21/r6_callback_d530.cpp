// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em21_09D1A610(u8 *);
extern "C" void func_em21_09D1A6E8(u8 *);
extern "C" void func_em21_09D1A808(u8 *);
extern "C" void func_em21_09D1A8B8(u8 *);

extern "C" void func_em21_09D22630(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em21_09D1A610(actor);
        break;
    case 1:
        func_em21_09D1A6E8(actor);
        break;
    case 2:
        func_em21_09D1A808(actor);
        break;
    case 3:
        func_em21_09D1A8B8(actor);
        break;
    }
}
