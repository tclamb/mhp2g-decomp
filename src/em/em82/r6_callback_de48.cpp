// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em82_09D22FE8(u8 *);
extern "C" void func_em82_09D235B0(u8 *);
extern "C" void func_em82_09D23A50(u8 *);
extern "C" void func_em82_09D23C88(u8 *);
extern "C" void func_em82_09D23E60(u8 *);

extern "C" void func_em82_09D22F48(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em82_09D22FE8(actor);
        break;
    case 1:
        func_em82_09D235B0(actor);
        break;
    case 4:
        func_em82_09D23A50(actor);
        break;
    case 5:
        func_em82_09D23C88(actor);
        break;
    case 6:
        func_em82_09D23E60(actor);
        break;
    }
}
