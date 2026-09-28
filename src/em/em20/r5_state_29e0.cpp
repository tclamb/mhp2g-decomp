// Clear the flags bit on both state transitions, preserving the store order.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern "C" void func_game_task_09AC04F8(u8 *, int, int, int);
extern "C" void func_em20_09D17550(u8 *);

extern "C" void func_em20_09D17AE0(u8 *actor) {
    switch (actor[0x1D5]) {
    case 0:
        ++actor[0x1D5];
        actor[0x280] = 0;
        *(u32 *)(actor + 0x410) &= ~2U;
        func_game_task_09AC04F8(actor, 106, 0, 0);
        break;
    case 1:
        if ((bool)(*(u16 *)(actor + 0xBC) & 1) == false) {
            *(u32 *)(actor + 0x410) &= ~2U;
            ++actor[0x1D5];
            func_em20_09D17550(actor);
        }
        break;
    }
}
