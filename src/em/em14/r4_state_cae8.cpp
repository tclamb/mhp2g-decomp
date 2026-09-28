// Original single state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em14_09D211E0(u8 *);

extern "C" void func_em14_09D21BE8(u8 *actor) {
    switch (actor[0x299]) {
    case 16:
        func_em14_09D211E0(actor);
        break;
    }
}
