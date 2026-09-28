typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef int s32;
template <typename T> struct Singleton { static T *objectPtr; };

struct Ptmf { float word[3]; };
extern "C" int __ptmf_test(Ptmf *);
extern "C" Ptmf ptmf_game_task_09BB25A0;
extern "C" Ptmf ptmf_game_task_09BB25B0;
extern "C" Ptmf ptmf_game_task_09BB25C0;
extern "C" Ptmf ptmf_game_task_09BB25D0;
extern "C" Ptmf ptmf_game_task_09BB25E0;

struct System {
    static System *objectPtr;
    u8 reserved_00[0xF7A3BC];
    u32 flags_F7A3BC;
};

static inline bool is_net_mode() { u32 flag = (System::objectPtr->flags_F7A3BC & 1) != 0; return flag; }

struct GameSys : Singleton<GameSys> {
    u8 reserved_00[0xC];
    u8 snapshot_state_0C;
    u8 reserved_0D[0x28 - 0xD];
    u8 player_id;
    u8 reserved_29[0x422 - 0x29];
    bool allow_skip_422;
    u8 reserved_423[0x480 - 0x423];
    bool flag_480;
    u8 reserved_481[0x488 - 0x481];
    s32 timer_488;
    u8 reserved_48C[0x6AF0E - 0x48C];
    u16 stage_id;
    u8 reserved_6AF10[0x6AF1B - 0x6AF10];
    u8 flag_6AF1B;

    u16 method_0885143C(u16);
};
extern "C" u16 D_eboot_089A9FE4;

struct FadeTask : Singleton<FadeTask> {
    void fadeBlack(s16 durationFrames, bool endVisible);
};
struct BgmServer : Singleton<BgmServer> {};
struct Sound : Singleton<Sound> {};
struct Quest : Singleton<Quest> {
    u8 reserved_00[0x4C];
    u8 *info_4C;
};
struct QuestNet : Singleton<QuestNet> {
    u8 reserved_00[0x170];
    u8 flag_170;
    u8 reserved_171[4];
    u8 flag_175;
};
struct Dialog : Singleton<Dialog> {};
struct Evdemo : Singleton<Evdemo> {};
struct Cockpit : Singleton<Cockpit> {
    u8 reserved_00[0x538];
    u8 *snapshot_538;
};
struct ResourceManager : Singleton<ResourceManager> {
    u8 *alloc(int tag, u32 size);
};
struct DrawManager : Singleton<DrawManager> {
    bool queue_vram_transfer(void *dst, u8 fragment_index);
};
struct Pad {
    static u32 RISING_EDGE;
};

struct StageEntry { u8 data[0x20]; };
struct StageBase {
    virtual void m00();
    virtual void m01();
    virtual void m02();
    virtual void m03();
    virtual void m04();
    virtual void m05();
    virtual void m06();
    virtual void m07();
    virtual void m08();
    virtual void m09();
    virtual void m10();
    virtual void m11();
    virtual void m12();
    virtual void *m3C(u8);
    virtual s32 entry_count();
    virtual StageEntry *entries();
};
struct StageManager : Singleton<StageManager> {
    StageBase *stage;
};

struct Player {
    virtual void m00();
    virtual void m01();
    virtual void m02();
    virtual void m03();
    virtual void m04();
    virtual void m05();
    virtual void m06();
    virtual void m07();
    virtual void m08();
    virtual void m09();
    virtual void m10();
    virtual void m11();
    virtual void m12();
    virtual void m13();
    virtual void m14();
    virtual void m15();
    virtual void m16();
    virtual void m17();
    virtual void m18();
    virtual void set_action(s32, s32, void *, s32);
};
struct PlayerManager : Singleton<PlayerManager> {
    u8 *method_088DF804(s32);
};

struct Vec3 { float x, y, z; };
struct SpawnPoint {
    u16 id;
    u8 reserved_02[0x22];
    Vec3 pos;
    u16 angle;
};

struct GameTask {
    u8 reserved_00[4];
    float member_04[3];
    u8 reserved_10[0xC];
    u16 spawn_id;
    u8 reserved_1E[2];
    Vec3 spawn_pos;
    u16 spawn_angle;
    u8 reserved_2E[6];
    u32 state_34;
    u8 reserved_38[0x14];
    SpawnPoint *spawn_4C;
    u8 reserved_50[4];
    u32 flags_54;
    u8 reserved_58[0x14];
    u8 flag_6C;
};

extern "C" u8 func_game_task_09A5ED28(GameTask *);
extern "C" void func_game_task_09A5D9B0(GameTask *);
extern "C" u8 func_eboot_08885D80(BgmServer *, u16);
extern "C" u16 func_eboot_08885D4C(BgmServer *);
extern "C" u16 func_eboot_08885D70(BgmServer *);
extern "C" u32 func_eboot_08885D68(BgmServer *);
extern "C" u8 func_eboot_08885E80(BgmServer *, u16);
extern "C" void func_eboot_08885D78(BgmServer *, u16);
extern "C" void func_eboot_08882C08(Sound *, u32, u32);
extern "C" void func_eboot_08882928(Sound *, u32, u32, u32);
extern "C" void func_eboot_08882B28(Sound *, u32, u16, u32);
extern "C" void func_eboot_08882BA4(Sound *, u32, u16, u32, u32);
extern "C" void func_eboot_08880490(Sound *, u16);
extern "C" void func_eboot_08885574(Sound *, u32, u32);
extern "C" void func_eboot_0888286C(Sound *, u32);
extern "C" u8 func_eboot_088698E4(Quest *);
extern "C" void func_eboot_0886B0D8(Quest *);
extern "C" void func_game_sub_09C8F808(StageEntry *, void *);
extern "C" u8 D_game_sub_09CE03A8[];
extern "C" void func_eboot_088AA234();
extern "C" void func_eboot_088AA22C(u32);
extern "C" void func_eboot_088AECDC(QuestNet *);
extern "C" void func_eboot_088AEE5C(QuestNet *);
extern "C" void func_game_sub_09C26748(Dialog *, u32, u16, u32);
extern "C" void func_game_sub_09C26998(Dialog *, u32);
extern "C" void func_game_sub_09C26270(Dialog *);
extern "C" void func_eboot_088CFB78(Evdemo *);
extern "C" u8 func_eboot_088566DC(GameSys *);
extern "C" u8 func_eboot_0884F9A0(GameSys *, u32);

extern "C" void func_game_task_09A5B508(GameTask *self) {
    switch (self->state_34) {
    case 0:
        FadeTask::objectPtr->fadeBlack(0x10, false);
        if (func_game_task_09A5ED28(self) == 1) {
            if (func_eboot_08885D80(BgmServer::objectPtr, 0xFFFF) == 0) {
                u16 cue = func_eboot_08885D4C(BgmServer::objectPtr);
                if (cue == func_eboot_08885D70(BgmServer::objectPtr)) {
                    func_eboot_08882C08(Sound::objectPtr, 0, 0);
                    func_eboot_08882928(Sound::objectPtr, 0, 0xF, func_eboot_08885D68(BgmServer::objectPtr));
                } else if (func_eboot_08885E80(BgmServer::objectPtr, func_eboot_08885D4C(BgmServer::objectPtr)) == 1) {
                    func_eboot_08882B28(Sound::objectPtr, 0, func_eboot_08885D4C(BgmServer::objectPtr), func_eboot_08885D68(BgmServer::objectPtr));
                } else {
                    func_eboot_08882BA4(Sound::objectPtr, 0, func_eboot_08885D4C(BgmServer::objectPtr), 4, func_eboot_08885D68(BgmServer::objectPtr));
                }
            }
            func_eboot_08885D78(BgmServer::objectPtr, func_eboot_08885D4C(BgmServer::objectPtr));
        } else {
            func_eboot_08885D78(BgmServer::objectPtr, 0xFFFF);
        }
        func_eboot_08880490(Sound::objectPtr, GameSys::objectPtr->stage_id);
        s32 i = 0;
        if (StageManager::objectPtr->stage->entry_count() > 0) {
        s32 off = 0;
        do {
            if (func_eboot_088698E4(Quest::objectPtr) == 1) {
                func_game_sub_09C8F808((StageEntry *)((u8 *)StageManager::objectPtr->stage->entries() + off), D_game_sub_09CE03A8);
            } else {
                StageBase *stage = StageManager::objectPtr->stage;
                func_game_sub_09C8F808((StageEntry *)((u8 *)stage->entries() + off), stage->m3C(Quest::objectPtr->info_4C[0x44]));
            }
            off += 0x20;
            i++;
        } while (i < StageManager::objectPtr->stage->entry_count());
        }
        self->flags_54 = 0;
        if (is_net_mode()) {
            QuestNet *net = QuestNet::objectPtr;
            func_eboot_088AA234();
            net->flag_175 = 0;
        }
        self->flag_6C = 0;
        self->state_34++;
    case 1:
        if (is_net_mode()) {
            QuestNet *qn = QuestNet::objectPtr;
            if (qn->flag_170 != 0) {
                func_game_sub_09C26748(Dialog::objectPtr, 0x82, GameSys::objectPtr->method_0885143C(D_eboot_089A9FE4), 0x12);
                func_game_sub_09C26998(Dialog::objectPtr, 0);
                func_game_sub_09C26270(Dialog::objectPtr);
                Ptmf pm = ptmf_game_task_09BB25A0;
                if (__ptmf_test(&pm) != 0) *(Ptmf *)self->member_04 = pm;
                self->state_34 = 0;
                return;
            }
            func_eboot_088AECDC(qn);
            func_eboot_088AEE5C(QuestNet::objectPtr);
        }
        if (GameSys::objectPtr->flag_6AF1B == 0) func_game_task_09A5D9B0(self);
        func_eboot_088CFB78(Evdemo::objectPtr);
        if (func_eboot_088566DC(GameSys::objectPtr) == 1) return;
        func_eboot_0886B0D8(Quest::objectPtr);
        if (GameSys::objectPtr->snapshot_state_0C == 0x81) {
            Cockpit::objectPtr->snapshot_538 = ResourceManager::objectPtr->alloc(9, 0x44000);
            DrawManager::objectPtr->queue_vram_transfer(Cockpit::objectPtr->snapshot_538, 8);
            GameSys::objectPtr->snapshot_state_0C = 0x80;
        }
        if (func_eboot_0884F9A0(GameSys::objectPtr, 2) == 1) {
            self->state_34 = 0;
            {
                Ptmf pm = ptmf_game_task_09BB25B0;
                if (__ptmf_test(&pm) != 0) *(Ptmf *)self->member_04 = pm;
            }
            u8 *player = PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id);
            if (player != 0) {
                player[0x564] = 0;
                *(u32 *)(player + 0x614) &= ~0x1040;
                player[0x285] = 0;
                *(u16 *)(player + 0x3B8) = 0;
                player[0x33C] = 0;
                player[0x396] = 0;
                player[0x10FE] = 0;
                player[0x565] = 0;
            }
            return;
        }
        if (GameSys::objectPtr->flag_480) {
            GameSys *game = GameSys::objectPtr;
            s32 *timer = &game->timer_488;
            if (*timer > 0) (*timer)--;
            if (!GameSys::objectPtr->allow_skip_422 || GameSys::objectPtr->timer_488 <= 0x10 ||
                (Pad::RISING_EDGE & 0x4000)) {
                FadeTask::objectPtr->fadeBlack(0x10, true);
                func_eboot_08885574(Sound::objectPtr, 0x10, 0);
                func_eboot_0888286C(Sound::objectPtr, 0x10);
                Ptmf pm = ptmf_game_task_09BB25C0;
                if (__ptmf_test(&pm) != 0) *(Ptmf *)self->member_04 = pm;
                self->state_34 = 0;
                return;
            }
        }
        if (self->flags_54 & 2) {
            {
                Ptmf pm = ptmf_game_task_09BB25D0;
                if (__ptmf_test(&pm) != 0) *(Ptmf *)self->member_04 = pm;
                self->state_34 = 0;
            }
            if (is_net_mode()) {
                QuestNet *net = QuestNet::objectPtr;
                func_eboot_088AA22C(0x3C);
                net->flag_175 = 1;
            }
        } else if (self->flags_54 & 1) {
            self->spawn_id = self->spawn_4C->id;
            {
                SpawnPoint *sp = self->spawn_4C;
                self->spawn_pos = sp->pos;
                self->spawn_angle = sp->angle;
            }
            Player *player = (Player *)PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id);
            player->set_action(5, 0, &self->spawn_id, 0);
            {
                Ptmf pm = ptmf_game_task_09BB25E0;
                if (__ptmf_test(&pm) != 0) *(Ptmf *)self->member_04 = pm;
                self->state_34 = 0;
            }
            if (is_net_mode()) {
                QuestNet *net = QuestNet::objectPtr;
                func_eboot_088AA22C(0x3C);
                net->flag_175 = 1;
            }
        }
    }
}
