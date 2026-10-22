#pragma once

#include "common.h"
#include "singleton.hpp"

struct BgmServer : Singleton<BgmServer> {
    u32 unknown_0x0;
    u32 unknown_0x4;
    u16 unknown_0x8;
    u16 unknown_0xA;
    u16 unknown_0xC;
    u16 unknown_0xE;
    u32 unknown_0x10;
    u32 unknown_0x14;
    u8 unknown_0x18;
    u8 unknown_0x19;
    u8 unknown_0x1A;
    u16 unknown_0x1C;
    u16 unknown_0x1E;
    u16 unknown_0x20;
    u16 unknown_0x22;
    u32 unknown_0x24;
    u16 unknown_0x28;
    u16 unknown_0x2A;
};

extern "C" {
    void func_eboot_08885D78(BgmServer *, u16);
    void func_eboot_08885D9C(BgmServer *, int);
    void func_game_task_09A60148(BgmServer *, int);
    void func_game_task_09A60190(BgmServer *);
    s32 func_game_task_09A5FBF0(BgmServer *);
}
