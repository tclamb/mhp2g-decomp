#pragma once

// Minimal declarations and inlined stage lookup for this GameTask method.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

template <typename T> struct Singleton { static T *objectPtr; };
struct GameSys : Singleton<GameSys> {};
struct GameTask { u8 reserved[0x6C]; u8 flag_6C; };
extern "C" u16 D_game_sub_09CDFF78[];
extern "C" u16 D_game_sub_09CE0190[];

static inline u8 is_special_stage(u16 stage) {
    if (stage == 0xFFFF) return 1;
    return D_game_sub_09CDFF78[stage] == 0;
}

static inline u16 remap_stage(u16 stage) {
    if (stage == 0xFFFF) return stage;
    u16 mapped = D_game_sub_09CE0190[stage];
    return mapped != 0xFFFF ? mapped : stage;
}

static inline u16 select_stage(u16 stage) {
    if (is_special_stage(stage) == 1) return stage;
    return remap_stage(stage);
}

