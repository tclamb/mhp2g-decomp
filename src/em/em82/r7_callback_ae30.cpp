// Begin only when the five-argument guard succeeds, then wait for the busy bit to clear.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern "C" void func_game_task_09AC04F8(u8 *, int, int, int);
extern "C" int func_game_task_09AD4880(u8 *, int, int, int, int);
extern "C" void func_em82_09D180C0(u8 *);

extern "C" void func_em82_09D1FF30(u8 *actor) {
    switch (actor[0x1D5]) {
    case 0:
        if (func_game_task_09AD4880(actor, 3640, 3, 6, 5)) {
            ++actor[0x1D5];
            actor[0x280] = 0;
            *(u32 *)(actor + 0x410) &= ~2U;
            func_game_task_09AC04F8(actor, 102, 0, 0);
        }
        break;
    case 1:
        if ((bool)(*(u16 *)(actor + 0xBC) & 1) == false) {
            ++actor[0x1D5];
            func_em82_09D180C0(actor);
        }
        break;
    }
}
