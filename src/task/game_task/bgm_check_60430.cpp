typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
template <typename T> struct Singleton { static T *objectPtr; };
struct GameSys : Singleton<GameSys> {};
struct Sound : Singleton<Sound> {};
struct BgmServer {
    u8 reserved_00[0x14];
    u32 flags_14;
    u8 reserved_18[4];
    u16 alternates_1C[2];
    u8 reserved_20[4];
    u32 changes_24;
};
struct BgmStageCue { u16 first; u16 second; };
extern "C" BgmStageCue D_game_sub_09CDF7B8[];
extern "C" u32 func_eboot_08882A00(Sound *, u16);
extern "C" void func_game_task_09A60318(BgmServer *, u16);

extern "C" void func_game_task_09A60430(BgmServer *self, u16 stage) {
    self->changes_24 = 0;
    GameSys *game = GameSys::objectPtr;
    u16 game_stage = *(u16 *)((u8 *)game + 0x6AF0E);
    if (game_stage == 0x97) goto check40;
    if (game_stage == 0x10) goto check40;
    switch (game_stage) {
    case 1:
        goto check4;
    default:
        goto stage_done;
    }
check4:
    if ((self->flags_14 & 4) == 0) self->changes_24 = 7;
    goto stage_done;
check40:
    if ((self->flags_14 & 0x40) == 0) self->changes_24 = 7;
stage_done:
    self->flags_14 |= 4;
    self->flags_14 |= 8;
    self->flags_14 |= 0x40;
    u32 primary = func_eboot_08882A00(Sound::objectPtr, 0x12);
    if (primary != D_game_sub_09CDF7B8[stage].first)
        self->changes_24 |= 1;
    func_game_task_09A60318(self, stage);
    u16 first = self->alternates_1C[0];
    u16 second = self->alternates_1C[1];
    if (first != 0xFFFF && func_eboot_08882A00(Sound::objectPtr, 0x13) != first)
        self->changes_24 |= 2;
    if (second != 0xFFFF && func_eboot_08882A00(Sound::objectPtr, 0x14) != second)
        self->changes_24 |= 4;
}
