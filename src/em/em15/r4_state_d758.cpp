// Original dual state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em15_09D21950(u8 *);
extern "C" void func_em15_09D21C78(u8 *);

extern "C" void func_em15_09D22858(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em15_09D21950(actor); break;
    case 16: func_em15_09D21C78(actor); break;
    }
}
