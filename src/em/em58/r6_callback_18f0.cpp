// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em58_09D16748(u8 *);
extern "C" void func_em58_09D16750(u8 *);
extern "C" void func_em58_09D167D0(u8 *);
extern "C" void func_em58_09D16898(u8 *);
extern "C" void func_em58_09D16970(u8 *);

extern "C" void func_em58_09D169F0(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em58_09D16748(actor);
        break;
    case 1:
        func_em58_09D16750(actor);
        break;
    case 2:
        func_em58_09D167D0(actor);
        break;
    case 3:
        func_em58_09D16898(actor);
        break;
    case 4:
        func_em58_09D16970(actor);
        break;
    }
}
