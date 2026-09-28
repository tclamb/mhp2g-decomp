// Original triple state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em21_09D208A0(u8 *);
extern "C" void func_em21_09D20AC8(u8 *);
extern "C" void func_em21_09D215B0(u8 *);

extern "C" void func_em21_09D22C50(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em21_09D208A0(actor); break;
    case 1: func_em21_09D20AC8(actor); break;
    case 2: func_em21_09D215B0(actor); break;
    }
}
