// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em83_09D20540(u8 *);
extern "C" void func_em83_09D20768(u8 *);
extern "C" void func_em83_09D21230(u8 *);
extern "C" void func_em83_09D21410(u8 *);

extern "C" void func_em83_09D232E0(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em83_09D20540(actor);
        break;
    case 1:
        func_em83_09D20768(actor);
        break;
    case 2:
        func_em83_09D21230(actor);
        break;
    case 3:
        func_em83_09D21410(actor);
        break;
    }
}
