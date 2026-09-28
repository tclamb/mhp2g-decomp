// Original triple state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em14_09D1FD78(u8 *);
extern "C" void func_em14_09D20070(u8 *);
extern "C" void func_em14_09D20468(u8 *);

extern "C" void func_em14_09D21B80(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em14_09D1FD78(actor); break;
    case 1: func_em14_09D20070(actor); break;
    case 2: func_em14_09D20468(actor); break;
    }
}
