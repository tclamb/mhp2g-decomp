#pragma once

#include "common.h"
#include "singleton.hpp"

struct Evdemo;

struct EvdemoData;

typedef int (Evdemo::*ptmf_check)(EvdemoData *);
typedef int (Evdemo::*ptmf_event)(EvdemoData *);

struct EvdemoBgmType {
    enum {
        NONE = 0,
        DEFAULT = 1,
        LAOSHAN_LUNG = 2,
        FATALIS = 3,
        UNUSED_4 = 4,
        UNUSED_5 = 5,
        UNUSED_6 = 6,
        UNUSED_7 = 7,
        ELDER_DRAGON = 8,
        SHEN_GAOREN = 9,
        CRIMSON_FATALIS = 10,
        YAMA_TSUKAMI = 11,
    };
private:
    EvdemoBgmType();
};

struct EvdemoFlag {
    enum {
        EM_DEMO = 1 << 0,
        MAP_DEMO = 1 << 1,
        MISSION_DEMO = 1 << 2,
    };
private:
    EvdemoFlag();
};

struct FadeCueType {
    enum {
        NONE = 0,
        FADE_IN = 1,
        FADE_TO_BLACK = 2,
    };
private:
    FadeCueType();
};

struct FadeCue {
    s32 time;
    u32 type;
    u32 duration;
};

struct EvdemoDefinition {
    u16 flag;
    u16 day_stage_id;
    u8 demo_id;
    u8 em_type;
    u8 bgm_type;
    u16 bgm_id;
    u8 flags;
    u32 pad_0xC;
    ptmf_check check;
    ptmf_event event;
    FadeCue *fades;
};

struct EvdemoData {
    union {
        u32 u;
        struct {
            u8 is_present;
            u8 state;
        } uc;
    };
    u32 pad_0x4[6];
    EvdemoDefinition *definition;
};

struct Evdemo : Singleton<Evdemo> {
    u8 is_playing;
    u8 slot;
    EvdemoData demos[4];
    u32 pad_0x84;
    u32 is_requested;
    u8 pad_0x8C[12];
    u8 pl_skip_mask;
    bool is_skipping;
    bool is_dragon_demo;
    u8 is_bgm_loading;
    s32 sound_stop_timer;

    Evdemo();

    void EvDemoInitialize();
    int ev_demo_init_sub(EvdemoData *data, EvdemoDefinition *definition);
    u16 dragon_quest_chk();
    int map_demo_chk();
    void EvDemoMove();
    int check000(EvdemoData *data);
    int event000(EvdemoData *data);
    int demo_finish(EvdemoData *data);
    void evdemo_camera_request(EvdemoDefinition *definition);
    bool demo_skip_chk();
    void start_next_demo();
    u16 bgm_id();
    u8 bgm_type();
    int event_flag_id();
    bool Em_demo_chk();
    int em_demo_ready_chk();
    void load_demo_bgm();
    bool stage_se_mute_chk(u32 tsb_id, bool is_update);
    void fade_sub(EvdemoDefinition *definition);
    void fade_sub_sub(FadeCue *fade);
};
