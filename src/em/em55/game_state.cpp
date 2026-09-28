typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

struct GameSys;
template <class T> struct Singleton {
    static T *objectPtr;
};

struct Em55SyncState {
    u8 unused[0x1E];
    s8 mode;
    u8 state;
};

extern "C" void func_em55_09D1BB10(u8 *actor, u8 value) {
    u8 *slot = actor + 0x790;
    if (actor[0x6DB]) {
        *slot = value;
    }
}

extern "C" bool func_em55_09D1BB28(u8 *actor, u8 value) {
    return actor[0x790] != value;
}

extern "C" void func_em55_09D1BB40(u8 *actor, Em55SyncState *dest) {
    dest->state = actor[0x790];
    dest->mode = ((s8 *)actor)[0x791];
    *(u16 *)(actor + 0x792) = 0x96;
}

extern "C" void func_em55_09D1BB60(u8 *actor, Em55SyncState *dest) {
    dest->state = actor[0x790];
    dest->mode = ((s8 *)actor)[0x791];
    *(u16 *)(actor + 0x792) = 0x96;
}

extern "C" void func_em55_09D1BB80(u8 *actor, Em55SyncState *dest) {
    dest->state = actor[0x790];
    dest->mode = ((s8 *)actor)[0x791];
    *(u16 *)(actor + 0x792) = 0x96;
}

extern "C" void func_em55_09D1BBA0(u8 *actor, Em55SyncState *dest) {
    dest->state = actor[0x790];
    dest->mode = ((s8 *)actor)[0x791];
    *(u16 *)(actor + 0x792) = 0x96;
}

extern "C" void func_em55_09D1BBC0(u8 *actor, Em55SyncState *source) {
    actor[0x790] = source->state;
    s8 mode = source->mode;
    actor[0x791] = mode;
    ((u8 *)Singleton<GameSys>::objectPtr)[0x38F] = mode;
}

extern "C" void func_em55_09D1BBE0(u8 *actor, Em55SyncState *source) {
    actor[0x790] = source->state;
    s8 mode = source->mode;
    actor[0x791] = mode;
    ((u8 *)Singleton<GameSys>::objectPtr)[0x38F] = mode;
}

extern "C" void func_em55_09D1BC00(u8 *actor, Em55SyncState *source) {
    actor[0x790] = source->state;
    s8 mode = source->mode;
    actor[0x791] = mode;
    ((u8 *)Singleton<GameSys>::objectPtr)[0x38F] = mode;
}
