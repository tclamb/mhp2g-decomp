// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em15_09D1B6D0(u8 *);
extern "C" void func_em15_09D1B780(u8 *);
extern "C" void func_em15_09D1B828(u8 *);
extern "C" void func_em15_09D1B8D0(u8 *);
extern "C" void func_em15_09D1B980(u8 *);

extern "C" void func_em15_09D22170(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em15_09D1B6D0(actor);
        break;
    case 3:
        func_em15_09D1B780(actor);
        break;
    case 5:
        func_em15_09D1B828(actor);
        break;
    case 6:
        func_em15_09D1B8D0(actor);
        break;
    case 7:
        func_em15_09D1B980(actor);
        break;
    }
}
