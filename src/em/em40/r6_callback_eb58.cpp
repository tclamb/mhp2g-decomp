// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em40_09D214E8(u8 *);
extern "C" void func_em40_09D21EB0(u8 *);
extern "C" void func_em40_09D22350(u8 *);
extern "C" void func_em40_09D22588(u8 *);

extern "C" void func_em40_09D23C58(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em40_09D214E8(actor);
        break;
    case 1:
        func_em40_09D21EB0(actor);
        break;
    case 4:
        func_em40_09D22350(actor);
        break;
    case 5:
        func_em40_09D22588(actor);
        break;
    }
}
