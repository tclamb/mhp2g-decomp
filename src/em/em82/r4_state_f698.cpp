// Original single state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em82_09D247D0(u8 *);

extern "C" void func_em82_09D24798(u8 *actor) {
    switch (actor[0x299]) {
    case 16:
        func_em82_09D247D0(actor);
        break;
    }
}
