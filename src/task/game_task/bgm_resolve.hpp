#pragma once

// Minimal declarations for this matched BgmServer helper.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
template <typename T> struct Singleton { static T *objectPtr; };
struct StageManager : Singleton<StageManager> { u8 reserved[0x288]; u16 stage_id; };
struct BgmStageCue { u16 first; u16 second; };
extern "C" BgmStageCue D_game_sub_09CDF7B8[];

