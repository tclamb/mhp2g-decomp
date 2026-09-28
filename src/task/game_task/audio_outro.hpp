#pragma once

// Minimal declarations for this GameTask audio transition.
typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;

template <typename T> struct Singleton { static T *objectPtr; };
struct Evdemo : Singleton<Evdemo> {};
struct BgmServer : Singleton<BgmServer> {};

extern "C" s32 func_eboot_08885D94(BgmServer *);
extern "C" s32 func_eboot_088D05F8(Evdemo *);

