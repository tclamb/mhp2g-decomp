// Dispatch the actor byte to the original case-specific callback.
typedef unsigned char u8;
extern "C" void func_em59_09D1EAD0(u8 *);
extern "C" void func_em59_09D1ECD8(u8 *);
extern "C" void func_em59_09D1ECE0(u8 *);
extern "C" void func_em59_09D1EF78(u8 *);

extern "C" void func_em59_09D1F758(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em59_09D1EAD0(actor);
        break;
    case 1:
        func_em59_09D1ECD8(actor);
        break;
    case 2:
        func_em59_09D1ECE0(actor);
        break;
    case 3:
        func_em59_09D1EF78(actor);
        break;
    }
}
