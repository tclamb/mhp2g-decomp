// Original triple state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em02_09D1EDD0(u8 *);
extern "C" void func_em02_09D1EFA8(u8 *);
extern "C" void func_em02_09D1F190(u8 *);

extern "C" void func_em02_09D1F978(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em02_09D1EDD0(actor); break;
    case 1: func_em02_09D1EFA8(actor); break;
    case 2: func_em02_09D1F190(actor); break;
    }
}
