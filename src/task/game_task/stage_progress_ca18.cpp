typedef signed short s16;
typedef unsigned char u8;
typedef unsigned int u32;
template <typename T> struct Singleton { static T *objectPtr; };
struct GameSys : Singleton<GameSys> {
    u8 reserved_00[4];
    s16 timer_04;
    s16 timer_06;
    u8 reserved_08[0x26];
    u8 mode_2E;
};
struct Cockpit : Singleton<Cockpit> {};
struct GameTask {
    u8 reserved_00[4];
    float member_04[3];
    u8 reserved_10[0x24];
    u32 state_34;
};
struct Ptmf { float word[3]; };
extern "C" Ptmf ptmf_game_task_09BB2610;
extern "C" int __ptmf_test(Ptmf *);
extern "C" void func_eboot_0884A5D0(Cockpit *, u8);
extern "C" void func_game_task_09A5E6C8(GameTask *);
extern "C" void func_eboot_088243B0(Cockpit *);
extern "C" void func_game_sub_09C3CD90();
extern "C" void func_game_task_09A5DA28(GameTask *);
extern "C" void func_game_task_09A5DB68(GameTask *);

extern "C" void func_game_task_09A5CA18(GameTask *self) {
    u32 state = self->state_34;
    if (state == 3) goto case3;
    if (state == 2) goto case2;
    if (state == 1) goto case1;
    switch (state) {
    case 0:
        goto case0;
    default:
        goto common;
    }
case0:
    ++self->state_34;
    GameSys::objectPtr->timer_04 = 0;
    goto common;
case1:
    {
        GameSys *game = GameSys::objectPtr;
        ++game->timer_04;
        if (game->timer_04 < 0xC0) goto common;
        ++self->state_34;
        GameSys::objectPtr->timer_06 = 0;
        func_eboot_0884A5D0(Cockpit::objectPtr, GameSys::objectPtr->mode_2E);
        goto common;
    }
case2:
    {
        GameSys *game = GameSys::objectPtr;
        ++game->timer_06;
        if (game->timer_06 < 0x96) goto common;
        ++self->state_34;
        goto common;
    }
case3:
    {
        Ptmf pm = ptmf_game_task_09BB2610;
        if (__ptmf_test(&pm) != 0) *(Ptmf *)self->member_04 = pm;
        self->state_34 = 0;
    }
common:
    func_game_task_09A5E6C8(self);
    func_eboot_088243B0(Cockpit::objectPtr);
    func_game_sub_09C3CD90();
    func_game_task_09A5DA28(self);
    func_game_task_09A5DB68(self);
}
