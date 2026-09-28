// Set actor byte to two and start with action one, then wait.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern "C" void func_game_task_09AC04F8(u8 *, int, int, int);
extern "C" void func_em58_09D16708(u8 *);

extern "C" void func_em58_09D16FD0(u8 *actor) {
    switch (actor[0x1D5]) {
    case 0:
        ++actor[0x1D5];
        *(u32 *)(actor + 0x410) &= ~2U;
        actor[0x280] = 2;
        func_game_task_09AC04F8(actor, 1, 0, 0);
        break;
    case 1:
        if ((bool)(*(u16 *)(actor + 0xBC) & 1) == false) {
            ++actor[0x1D5];
            func_em58_09D16708(actor);
        }
        break;
    }
}
