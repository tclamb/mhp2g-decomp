// Original dual state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em17_09D21B38(u8 *);
extern "C" void func_em17_09D21B38(u8 *);

extern "C" void func_em17_09D22800(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em17_09D21B38(actor); break;
    case 16: func_em17_09D21B38(actor); break;
    }
}
