#pragma once

#include "common.h"
#include "game_sys.hpp"

struct ItemCombination {
    u16 materialB;
    u16 result;
    u8 mixRateId;
    u8 quantityOddsId;
    u8 unknown_0x6;
    u8 flags;
    u8 unknown_0x8;
    u8 unknown_0x9;
};

struct ItemDefinition {
    u8 unknown_0x0;
    u8 unknown_0x1;
    u8 unknown_0x2;
    u8 stackSize;
    u8 unknown_0x4;
    u8 unknown_0x5;
    u16 unknown_0x6;
    u16 unknown_0x8;
    u16 unknown_0xA;
    u32 unknown_0xC;
    u32 unknown_0x10;
    u16 unknown_0x14;
    u16 unknown_0x16;
};

typedef s8 MixRate;

struct ItemManager : Singleton<ItemManager> {
    ItemCombination *method_0885B5B0(s16, s16, int, bool, int);
    MixRate method_0885BE00(ItemCombination *, u32);

    ItemManager();
};

extern ItemDefinition ITEM_DEFINITIONS[1261];

extern ItemCombination ITEM_COMBINATIONS[172];

extern "C" {
    u8 func_eboot_0885C710(MixRate id);
}
