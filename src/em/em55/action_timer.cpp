typedef unsigned char u8;
typedef unsigned short u16;

extern "C" void func_em55_09D16070(u8 *);
extern "C" void func_em55_09D18580(u8 *);
extern "C" void func_em55_09D18988(u8 *);

extern "C" void func_em55_09D18C10(u8 *actor) {
    actor[0x4B4] = 5;
    *(u16 *)(actor + 0x35E) = 5;
    switch (actor[0x1D5]) {
    case 0:
        actor[0x1D5]++;
        *(int *)(actor + 0x414) = 5;
        break;
    case 1:
        if (--*(int *)(actor + 0x414) <= 0) {
            func_em55_09D16070(actor);
        }
        break;
    }
}

extern "C" void func_em55_09D18C70(u8 *actor) {
    switch (actor[0x299]) {
    case 0:
        func_em55_09D18580(actor);
        break;
    case 1:
        func_em55_09D18988(actor);
        break;
    case 0x99:
        func_em55_09D18C10(actor);
        break;
    }
}
