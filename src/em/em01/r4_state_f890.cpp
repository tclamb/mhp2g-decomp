// Original single state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em01_09D23BE0(u8 *);

extern "C" void func_em01_09D24990(u8 *actor) {
    switch (actor[0x299]) {
    case 16:
        func_em01_09D23BE0(actor);
        break;
    }
}
