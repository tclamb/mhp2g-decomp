// Original state start wait clear then flags args callback.
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
extern "C" void func_game_task_09AC04F8(u8 *, int, int, int);
extern "C" void func_em75_09D210D8(u8 *, int, int);

extern "C" void func_em75_09D27CE0(u8 *actor) {
    switch (actor[0x1D5]) {
    case 0:
        ++actor[0x1D5];
        actor[0x280] = 0;
        *(u32 *)(actor + 0x410) &= ~2U;
        func_game_task_09AC04F8(actor, 99, 0, 0);
        break;
    case 1:
        if ((bool)(*(u16 *)(actor + 0xBC) & 1) == false) {
            ++actor[0x1D5];
            func_em75_09D210D8(actor, 0, 0);
        }
        break;
    }
}
