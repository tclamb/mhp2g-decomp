// Original dual state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em58_09D1A150(u8 *);
extern "C" void func_em58_09D1A288(u8 *);

extern "C" void func_em58_09D1A4A0(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em58_09D1A150(actor); break;
    case 1: func_em58_09D1A288(actor); break;
    }
}
