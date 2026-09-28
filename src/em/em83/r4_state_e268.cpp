// Original single state dispatch callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
extern "C" void func_em83_09D22840(u8 *);

extern "C" void func_em83_09D23368(u8 *actor) {
    switch (actor[0x299]) {
    case 16:
        func_em83_09D22840(actor);
        break;
    }
}
