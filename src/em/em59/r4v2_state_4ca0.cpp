// Original state start wait clear callback.
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
extern "C" void func_game_task_09AC04F8(u8 *, int, int, int);
extern "C" void func_em59_09D18F28(u8 *);

extern "C" void func_em59_09D19DA0(u8 *actor) {
    switch (actor[0x1D5]) {
    case 0:
        ++actor[0x1D5];
        actor[0x280] = 0;
        func_game_task_09AC04F8(actor, 70, 0, 0);
        break;
    case 1:
        if ((bool)(*(u16 *)(actor + 0xBC) & 1) == false) {
            ++actor[0x1D5];
            func_em59_09D18F28(actor);
        }
        break;
    }
}
