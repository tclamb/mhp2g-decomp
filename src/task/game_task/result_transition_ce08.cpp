typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
template <typename T> struct Singleton { static T *objectPtr; };
struct GameSys : Singleton<GameSys> {};
struct ResultCheck : Singleton<ResultCheck> {
    u8 reserved_00[0xA0];
    u32 field_A0;
    u8 reserved_A4[0x20];
    u32 field_C4;
};
struct GameTask {
    u8 reserved_00[4];
    float member_04[3];
    u8 reserved_10[0x24];
    u32 state_34;
};
extern "C" u8 func_eboot_08874C50(ResultCheck *);
struct Ptmf { float word[3]; };
extern "C" int __ptmf_test(Ptmf *);
extern "C" Ptmf ptmf_game_task_09BB2630;

extern "C" void func_game_task_09A5CE08(GameTask *self) {
    u32 state = self->state_34;
    if (state == 1) goto case1;
    switch (state) {
    case 0:
        goto case0;
    default:
        return;
    }
case0:
    *(u16 *)((u8 *)GameSys::objectPtr + 0x6AF0E) = 0;
    ResultCheck::objectPtr->field_A0 = 0;
    ResultCheck::objectPtr->field_C4 = 0;
    ++self->state_34;
    return;
case1:
    {
        if (func_eboot_08874C50(ResultCheck::objectPtr) != 1) return;
        self->state_34 = 0;
        Ptmf pm = ptmf_game_task_09BB2630;
        if (__ptmf_test(&pm) != 0)
            *(Ptmf *)self->member_04 = pm;
    }
}
