#pragma once

#include "common.h"
#include "singleton.hpp"

struct InventoryEntry {
    u16 itemId;
    u16 quantity;
};

struct FarmState {
    u8 vars[8];
};

struct GameSys : Singleton<GameSys> {
    u8 pad_0x0[0x1C];
    u16 short_0x1C;
    u8 pad_0x1E[0x28 - 0x1E];
    u8 player_id;
    u8 pad_0x29;
    u8 flag_0x2A;
    u8 pad_0x2B[0x2E - 0x2B];
    u8 byte_0x2E;
    u8 pad_0x2F[0x390-0x2F];
    InventoryEntry itemBox[32];
    u8 pad_0x410[0x422-0x410];
    bool allow_hidden_flag;
    u8 pad_0x423[0x480 - 0x423];
    bool flag_0x480;
    u8 pad_0x481[0x43C8 - 0x481];
    InventoryEntry inventory[24];
    u8 pad_0x4428[0x6A238 - 0x4428];
    FarmState farm;
    u8 pad_0x6A240[0x6ADDD - 0x6A240];
    s8 flag_0x6ADDD;
    bool flag_0x6ADDE;
    u8 pad_0x6ADDF[0x6AF0C - 0x6ADDF];
    bool flag_0x6AF0C;
    u8 pad_0x6AF0D;
    u16 stage_id;
    u8 mapId;
    u32 flags_0x6AF14;
    u8 pad_0x6AF18[0x6AF40 - 0x6AF18];

    GameSys();
    ~GameSys();

    InventoryEntry *method_088567AC(u32 index);
    char *method_0885143C(u16);
    char *method_08851448(u16);
    int method_088567D4(u16);

    inline bool unknownTest() {
        return Singleton<GameSys>::objectPtr->flag_0x6ADDD && !Singleton<GameSys>::objectPtr->flag_0x480;
    }
};


extern "C" {
    char *func_eboot_088515A0(GameSys *, u16);
    int func_eboot_088566DC(GameSys *);
    bool func_eboot_0884F9A0(GameSys *, int);
}
