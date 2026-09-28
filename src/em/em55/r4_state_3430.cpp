// Original dual state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em55_09D182B0(u8 *);
extern "C" void func_em55_09D18418(u8 *);

extern "C" void func_em55_09D18530(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em55_09D182B0(actor); break;
    case 1: func_em55_09D18418(actor); break;
    }
}
