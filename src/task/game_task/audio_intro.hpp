#pragma once

// Minimal declarations for the GameTask audio intro state machine.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

template <typename T> struct Singleton { static T *objectPtr; };
struct GameSys : Singleton<GameSys> {};
struct Sound : Singleton<Sound> {};
struct GameTaskAudioState { u8 reserved[0x40]; s32 sound_index; };

extern "C" u16 D_eboot_089A6470[];
extern "C" void func_eboot_088805E0(Sound *, s32, u16, u16, s32);
extern "C" u8 func_eboot_08881674(Sound *);

