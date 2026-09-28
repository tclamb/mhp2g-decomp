// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em83_09D21CF0(u8 *);
extern "C" void func_em83_09D21F08(u8 *);
extern "C" void func_em83_09D220F8(u8 *);
extern "C" void func_em83_09D22568(u8 *);
extern "C" void func_em83_09D227B8(u8 *);

extern "C" void func_em83_09D23240(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em83_09D21CF0(actor);
        break;
    case 1:
        func_em83_09D21F08(actor);
        break;
    case 2:
        func_em83_09D220F8(actor);
        break;
    case 3:
        func_em83_09D22568(actor);
        break;
    case 4:
        func_em83_09D227B8(actor);
        break;
    }
}
