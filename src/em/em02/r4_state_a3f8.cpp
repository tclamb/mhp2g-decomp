// Original triple state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em02_09D1AF18(u8 *);
extern "C" void func_em02_09D1AFB8(u8 *);
extern "C" void func_em02_09D1B060(u8 *);

extern "C" void func_em02_09D1F4F8(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em02_09D1AF18(actor); break;
    case 1: func_em02_09D1AFB8(actor); break;
    case 2: func_em02_09D1B060(actor); break;
    }
}
