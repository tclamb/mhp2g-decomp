// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em15_09D21070(u8 *);
extern "C" void func_em15_09D212C8(u8 *);
extern "C" void func_em15_09D214F8(u8 *);
extern "C" void func_em15_09D21808(u8 *);

extern "C" void func_em15_09D22738(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em15_09D21070(actor);
        break;
    case 1:
        func_em15_09D212C8(actor);
        break;
    case 2:
        func_em15_09D214F8(actor);
        break;
    case 3:
        func_em15_09D21808(actor);
        break;
    }
}
