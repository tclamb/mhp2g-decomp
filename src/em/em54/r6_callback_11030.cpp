// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em54_09D25008(u8 *);
extern "C" void func_em54_09D25230(u8 *);
extern "C" void func_em54_09D25508(u8 *);

extern "C" void func_em54_09D26130(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em54_09D25008(actor);
        break;
    case 1:
        func_em54_09D25008(actor);
        break;
    case 2:
        func_em54_09D25230(actor);
        break;
    case 3:
        func_em54_09D25508(actor);
        break;
    }
}
