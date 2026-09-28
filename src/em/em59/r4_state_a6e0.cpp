// Original triple state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em59_09D1E288(u8 *);
extern "C" void func_em59_09D1E638(u8 *);
extern "C" void func_em59_09D1E958(u8 *);

extern "C" void func_em59_09D1F7E0(u8 *actor) {
    switch (actor[0x299]) {
    case 0: func_em59_09D1E288(actor); break;
    case 1: func_em59_09D1E638(actor); break;
    case 2: func_em59_09D1E958(actor); break;
    }
}
