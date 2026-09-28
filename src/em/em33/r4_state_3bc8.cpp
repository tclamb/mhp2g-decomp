// Original dual state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em33_09D18700(u8 *);
extern "C" void func_em33_09D187F0(u8 *);

extern "C" void func_em33_09D18CC8(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em33_09D18700(actor); break;
    case 1: func_em33_09D187F0(actor); break;
    }
}
