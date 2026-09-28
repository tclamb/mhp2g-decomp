// Original single state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em75_09D2EB28(u8 *);

extern "C" void func_em75_09D30270(u8 *actor) {
    switch (actor[0x299]) {
    case 16:
        func_em75_09D2EB28(actor);
        break;
    }
}
