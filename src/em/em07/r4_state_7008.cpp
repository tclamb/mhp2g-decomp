// Original triple state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em07_09D1B9E8(u8 *);
extern "C" void func_em07_09D1BDB0(u8 *);
extern "C" void func_em07_09D1C0A8(u8 *);

extern "C" void func_em07_09D1C108(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em07_09D1B9E8(actor); break;
    case 1: func_em07_09D1BDB0(actor); break;
    case 153: func_em07_09D1C0A8(actor); break;
    }
}
