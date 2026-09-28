#pragma once

// Minimal declarations for the matched BgmServer alternate cue setup.
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
template <typename T> struct Singleton { static T *objectPtr; };
struct GameSys : Singleton<GameSys> {};
struct Quest : Singleton<Quest> {};
struct BgmServer {
    u8 reserved_00[0x14];
    u32 flags_14;
    u8 reserved_18[4];
    u16 alternates_1C[2];
};
struct BgmStageOverride { u16 stage; u16 cue; u16 first; u8 second; u8 unused; };
extern "C" BgmStageOverride D_game_sub_09CDFBE8[];

