// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em20_09D22F30(u8 *);
extern "C" void func_em20_09D234F8(u8 *);
extern "C" void func_em20_09D239B0(u8 *);
extern "C" void func_em20_09D23BE8(u8 *);

extern "C" void func_em20_09D22EA8(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em20_09D22F30(actor);
        break;
    case 1:
        func_em20_09D234F8(actor);
        break;
    case 4:
        func_em20_09D239B0(actor);
        break;
    case 5:
        func_em20_09D23BE8(actor);
        break;
    }
}
