// Start, then advance on the predicate or the signed countdown, preserving both paths.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern "C" void func_game_task_09AC04F8(u8 *, int, int, int);
extern "C" bool func_game_task_09AD4CD8(u8 *, int, int);
extern "C" void func_em82_09D180C0(u8 *);

extern "C" void func_em82_09D19E30(u8 *actor) {
    switch (actor[0x1D5]) {
    case 0:
        ++actor[0x1D5];
        actor[0x280] = 0;
        *(u32 *)(actor + 0x410) &= ~2U;
        func_game_task_09AC04F8(actor, 3, 0, 0);
        break;
    case 1:
        if (actor[0x637]) {
            if (func_game_task_09AD4CD8(actor, 64, 1) == true) {
                ++actor[0x1D5];
                func_em82_09D180C0(actor);
            }
        } else {
            if (--*(int *)(actor + 0x414) <= 0) {
                ++actor[0x1D5];
                func_em82_09D180C0(actor);
            }
        }
        break;
    }
}
