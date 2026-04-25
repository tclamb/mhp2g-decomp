#pragma once

#include "common.h"
#include "singleton.hpp"

struct InventoryEntry {
    u16 itemId;
    s16 quantity;
};

struct FarmUpgrade {
    u8 vars[22];
};

struct EquipmentId {
    bool present;
    u8 type;
    u16 id;
};

struct EquipmentEntry {
    EquipmentId id;
    u16 upgrades;
    u16 decorationIds[3];
};

struct EquipmentSet {
    u16 weaponIndex;
    u16 armorIndices[5];
    u32 armorColors[5];
};

struct RepelProgress {
    u16 unknown_0x0;
    u8 unknown_0x2;
    u8 unknown_0x3;
};

struct UserData {
    u16 name[8];
    u8 unknown_0x10;
    u8 unknown_0x11;
    u8 unknown_0x12;
    u8 sexId;
    u8 voiceId;
    u8 clothingId;
    u8 faceId;
    u8 hairId;
    u8 unknown_0x18;
    u32 hairColor;
    EquipmentEntry equipment[6];
    u16 eventMask[16];
    u16 unknown_0x88[16];
    EquipmentEntry equipmentBox[1000];
    InventoryEntry itemBox[1000];
    InventoryEntry inventory[24]; // TODO rename to itemBag?
    u32 missionMask[16];
    u32 playTime;
    u8 mixMask[22];
    u8 padding_0x3FE2[42];
    u8 monsterListMask[16];
    u8 hunterRank;
    u8 padding_0x401D[15];
    u16 captureCounts[90];
    u16 largestSizes[90];
    u16 smallestSizes[90];
    u16 killCounts[90];
    u32 titleMask[14];
    EquipmentSet equipmentSets[20];
    u8 padding_0x45B4[4];
    u8 guildCards[96][4216];
    u8 padding_0x672B8[0x69248 - 0x672B8];
    s32 pokkePoints;
    s32 guildPoints;
    s32 money;
    u8 padding_0x69254[0x694A2 - 0x69254];
    u8 kitchenSkillIds[6];
    u8 padding_0x694A8[0x69D98 - 0x694A8];
    u8 farmUpgrades[22];
    u8 padding_0x69DAE[0x6A0AA - 0x69DAE];
    RepelProgress repelProgress[5][8];
    u8 padding_0x6A14A[0x6A7B4 - 0x6A14A];
    u32 itemMask[40];
    u8 padding_0x6A854[0x6A938 - 0x6A854];

    void setItemBit(u16 itemId);
    bool getItemBit(u16 itemId);
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
    u8 pad_0x481[0x4A0 - 0x481];
    UserData userData;
    u8 pad_0x6ADD8[0x6ADDD - 0x6ADD8];
    s8 flag_0x6ADDD;
    bool flag_0x6ADDE;
    u8 pad_0x6ADDF[0x6AF0C - 0x6ADDF];
    bool flag_0x6AF0C;
    u16 stage_id;
    u8 mapId;
    u16 missionId;
    u32 flags_0x6AF14;
    u8 pad_0x6AF18[0x6AF40 - 0x6AF18];

    GameSys();
    ~GameSys();

    InventoryEntry *method_088567AC(u32 index);
    int bagQuantity(u16 itemId);
    char *method_0885143C(u16);
    char *method_08851448(u16);
    int itemBoxSize();

    inline bool unknownTest() {
        return GameSys::objectPtr->flag_0x6ADDD && !GameSys::objectPtr->flag_0x480;
    }
};


extern "C" {
    char *func_eboot_088515A0(GameSys *, u16);
    int func_eboot_088566DC(GameSys *);
    bool func_eboot_0884F9A0(GameSys *, int);
}
