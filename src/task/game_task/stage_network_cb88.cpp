typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
template <typename T> struct Singleton { static T *objectPtr; };
struct GameSys : Singleton<GameSys> {};
struct Sound : Singleton<Sound> {};
struct Net : Singleton<Net> {};
struct System {};
struct Dialog : Singleton<Dialog> {};
struct ResultCheck : Singleton<ResultCheck> {
    u8 reserved_00[0x104];
    u32 value_104;
    u32 value_108;
};
struct LobbyNet : Singleton<LobbyNet> {
    u8 reserved_00[0x1AFC];
    u8 flag_1AFC;
};
struct FileSys {
    virtual void method00();
    virtual void method01();
    virtual void method02();
    virtual void method03();
    virtual void method04();
    virtual void method05();
    virtual void method06();
    virtual void method07();
    virtual void method08();
    virtual void method09();
    virtual void method10();
    virtual void method11();
    virtual u32 poll();
};
extern "C" FileSys *objectPtr__7FileSys;
extern "C" System *objectPtr__6System;
struct GameTask {
    u8 reserved_00[4];
    float member_04[3];
    u8 reserved_10[0x24];
    u32 state_34;
    u8 reserved_38[0x25];
    u8 flag_5D;
};
struct Ptmf { float word[3]; };
extern "C" Ptmf ptmf_game_task_09BB2620;
extern "C" int __ptmf_test(Ptmf *);
extern "C" u16 func_eboot_0884FAB0(GameSys *);
extern "C" void func_eboot_0888285C(Sound *, u32, u32);
extern "C" void func_eboot_08885234(Sound *, u32);
extern "C" void func_eboot_088B0C14(Net *);
extern "C" u32 func_eboot_088B0C44(Net *);
extern "C" void func_eboot_0889025C(System *);
extern "C" void func_game_sub_09C26998(Dialog *, u32);
extern "C" void func_eboot_0888368C(Sound *, u32, u32, u32, u32, u32, u32, u32, u32);
extern "C" void func_eboot_08855304(GameSys *);
extern "C" void func_eboot_0887E78C(ResultCheck *);
extern "C" u32 func_eboot_08855338();
extern "C" void func_eboot_0887E824(ResultCheck *);
extern "C" void func_eboot_088741CC(ResultCheck *);
extern "C" void func_game_sub_09C26270(Dialog *);

extern "C" void func_game_task_09A5CB88(GameTask *self) {
    u16 keys = func_eboot_0884FAB0(GameSys::objectPtr);
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
    func_eboot_0888285C(Sound::objectPtr, 0, 4);
    func_eboot_08885234(Sound::objectPtr, 0);
    func_eboot_088B0C14(Net::objectPtr);
    ++self->state_34;
    goto common;
case1:
    if (func_eboot_088B0C44(Net::objectPtr) != 0) ++self->state_34;
    goto common;
case2:
    {
        FileSys *file = objectPtr__7FileSys;
        if (file->poll() != 0) goto common;
        func_eboot_0889025C(objectPtr__6System);
        *(u8 *)((u8 *)objectPtr__7FileSys + 0x2FA04) = 0;
        func_game_sub_09C26998(Dialog::objectPtr, 1);
        ++self->state_34;
        goto common;
    }
case3:
    if ((keys & 0x2000) == 0) goto common;
    func_eboot_0888368C(Sound::objectPtr, 0, 0x13, 0, 0, 0, 0, 0, 0);
    {
        ResultCheck *result = ResultCheck::objectPtr;
        *(u32 *)((u8 *)GameSys::objectPtr + 0x696F0) = result->value_104;
        *(u32 *)((u8 *)GameSys::objectPtr + 0x696E8) = result->value_108;
    }
    func_eboot_08855304(GameSys::objectPtr);
    {
        GameSys *game = GameSys::objectPtr;
        u32 flag = (*(u32 *)((u8 *)game + 0x6AF14) & 4) != 0;
        if (flag != 0) {
            *(u8 *)((u8 *)game + 0x6AEA4) = 1;
            func_eboot_0887E78C(ResultCheck::objectPtr);
        } else if (func_eboot_08855338() != 0) {
            func_eboot_0887E824(ResultCheck::objectPtr);
        } else {
            func_eboot_088741CC(ResultCheck::objectPtr);
        }
    }
    LobbyNet::objectPtr->flag_1AFC = 0;
    self->flag_5D = 1;
    self->state_34 = 0;
    {
        Ptmf pm = ptmf_game_task_09BB2620;
        if (__ptmf_test(&pm) != 0) *(Ptmf *)self->member_04 = pm;
    }
common:
    func_game_sub_09C26270(Dialog::objectPtr);
}
