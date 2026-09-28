// Original dual state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em58_09D1A4F0(u8 *);
extern "C" void func_em58_09D1A948(u8 *);

extern "C" void func_em58_09D1AD38(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em58_09D1A4F0(actor); break;
    case 1: func_em58_09D1A948(actor); break;
    }
}
