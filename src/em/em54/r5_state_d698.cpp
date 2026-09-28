// Prepare the actor before starting; finish with the original two zero arguments.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern "C" void func_game_task_09AC04F8(u8 *, int, int, int);
extern "C" void func_em54_09D1C178(u8 *, int, int);
extern "C" void func_game_task_09AB61C0(u8 *);

extern "C" void func_em54_09D22798(u8 *actor) {
    switch (actor[0x1D5]) {
    case 0:
        ++actor[0x1D5];
        actor[0x280] = 0;
        *(u32 *)(actor + 0x410) &= ~2U;
        func_game_task_09AB61C0(actor);
        func_game_task_09AC04F8(actor, 61, 0, 0);
        break;
    case 1:
        if ((bool)(*(u16 *)(actor + 0xBC) & 1) == false) {
            ++actor[0x1D5];
            func_em54_09D1C178(actor, 0, 0);
        }
        break;
    }
}
