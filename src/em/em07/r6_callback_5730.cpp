// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em07_09D1A1B0(u8 *);
extern "C" void func_em07_09D1A290(u8 *);
extern "C" void func_em07_09D1A500(u8 *);
extern "C" void func_em07_09D1A5C0(u8 *);
extern "C" void func_em07_09D1A6F8(u8 *);

extern "C" void func_em07_09D1A830(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em07_09D1A1B0(actor);
        break;
    case 2:
        func_em07_09D1A290(actor);
        break;
    case 16:
        func_em07_09D1A500(actor);
        break;
    case 18:
        func_em07_09D1A5C0(actor);
        break;
    case 19:
        func_em07_09D1A6F8(actor);
        break;
    }
}
