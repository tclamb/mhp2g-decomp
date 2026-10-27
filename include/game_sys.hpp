#pragma once

#include "common.h"
#include "enemy.hpp"
#include "singleton.hpp"

struct EventFlagId {
    enum {
        TRAINING_BASICS_DEMO_SEEN = 0x00DC,
        TRAINING_GATHERING_DEMO_1_SEEN = 0x00DD,
        TRAINING_GATHERING_DEMO_2_SEEN = 0x00DE,
        TRAINING_GATHERING_DEMO_3_SEEN = 0x00DF,
        TRAINING_GATHERING_DEMO_4_SEEN = 0x00E0,
        TRAINING_COMBINING_DEMO_SEEN = 0x00E1,
        TRAINING_FISHING_DEMO_SEEN = 0x00E2,
        TRAINING_TRANSPORT_DEMO_SEEN = 0x00E3,
        FOREST_AND_HILLS_INTRO_SEEN = 0x00E4,
        DESERT_INTRO_SEEN = 0x00E5,
        SWAMP_INTRO_SEEN = 0x00E6,
        VOLCANO_INTRO_SEEN = 0x00E7,
        JUNGLE_INTRO_SEEN = 0x00E8,
        SNOWY_MOUNTAINS_INTRO_SEEN = 0x00E9,
        TOWER_INTRO_SEEN = 0x00EA,
        GREAT_FOREST_INTRO_SEEN = 0x00ED,
        RECKLESS_BULLDROME_HUNTER_DEMO_SEEN = 0x0140,
        HUNT_DOWN_THE_VELOCIDROME_DEMO_SEEN = 0x0141,
        THE_CARNIVOROUS_LEADER_DEMO_SEEN = 0x0142,
        JUNGLE_MENACE_DEMO_SEEN = 0x0143,
        THE_LAND_SHARK_DEMO_SEEN = 0x0144,
        GYPCEROS_VENOMOUS_TERROR_DEMO_SEEN = 0x0145,
        THE_LURKING_DESERT_GIANT_DEMO_SEEN = 0x0146,
        RAREST_OF_THE_RARE_BEASTS_DEMO_SEEN = 0x0147,
        THE_ELDER_DRAGON_OF_WIND_DEMO_SEEN = 0x0148,
        SHADOW_IN_THE_SNOW_DEMO_SEEN = 0x0149,
        THE_RULER_OF_THE_SNOW_DEMO_SEEN = 0x014A,
        WATER_WYVERN_IN_THE_DESERT_DEMO_SEEN = 0x014B,
        COMMANDER_IN_THE_FLAMES_DEMO_SEEN = 0x014C,
        THE_RUNAWAY_DIABLOS_DEMO_SEEN = 0x014D,
        THE_FROZEN_DICTATOR_DEMO_SEEN = 0x014E,
        THE_EMPRESS_S_BLAZING_THRONE_DEMO_SEEN = 0x014F,
        EMPEROR_OF_FLAME_DEMO_SEEN = 0x0150,
        TERROR_OF_THE_GRAVIOS_DEMO_SEEN = 0x0151,
        OVERSEER_OF_THE_ANCIENTS_DEMO_SEEN = 0x0152,
        THE_LEGENDARY_KIRIN_DEMO_SEEN = 0x0153,
        BATTLE_OF_THE_BLOS_DEMO_SEEN = 0x0154,
        THE_ELDER_DRAGON_OF_MIST_DEMO_SEEN = 0x0155,
        THE_FINAL_INVITATION_DEMO_SEEN = 0x0156,
        THE_LONE_BLACK_GARUGA_DEMO_SEEN = 0x0157,
        LAOSHAN_LUNG_INTRO_SEEN = 0x0158,
        ASHEN_LAOSHAN_LUNG_INTRO_SEEN = 0x0159,
        SHEN_GAOREN_INTRO_SEEN = 0x015A,
        SINKING_FEELING_DEMO_SEEN = 0x015B,
        ABSOLUTE_POWER_DEMO_SEEN = 0x015C,
        FATALIS_INTRO_SEEN = 0x015D,
        CRIMSON_FATALIS_INTRO_SEEN = 0x015E,
        WHITE_FATALIS_INTRO_SEEN = 0x015F,
        AKANTOR_INTRO_SEEN = 0x0160,
        YAMA_TSUKAMI_INTRO_SEEN = 0x0162,
        UKANLOS_INTRO_SEEN = 0x0163,
        LAVASIOTH_INTRO_SEEN = 0x0164,
        HYPNOCATRICE_INTRO_SEEN = 0x0165,
        NARGACUGA_INTRO_SEEN = 0x0166,
    };
private:
    EventFlagId();
};

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
    EquipmentEntry weapon;
    EquipmentEntry armor[5];
};

struct EquipmentSetRegistration {
    u16 weaponIndex;
    u16 armorIndices[5];
    u32 armorColors[5];
};

struct RepelProgress {
    u16 unknown_0x0;
    u8 unknown_0x2;
    u8 unknown_0x3;
};

#define ITEM_BAG_CAPACITY 24

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
    EquipmentSet equipment;
    u16 eventMask[16];
    u16 unknown_0x88[16];
    EquipmentEntry equipmentBox[1000];
    InventoryEntry itemBox[1000];
    InventoryEntry itemBag[ITEM_BAG_CAPACITY];
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
    EquipmentSetRegistration equipmentSets[20];
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
    u8 padding_0x6A14A[2];
    u8 armorRecipeFlags[1100];
    u8 weaponRecipeFlags[415];
    u8 decorationRecipeFlags[100];
    u8 padding_0x6A79B[0x6A7B4 - 0x6A79B];
    u32 itemMask[40];
    u8 padding_0x6A854[0x6A938 - 0x6A854];

    void setItemBit(u16 itemId);
    bool getItemBit(u16 itemId);
};

struct Options {
    s8 language;
    s8 sound;
    s8 bgmVolume;
    s8 seVolume;
    s8 quickContinue;
    s8 backgroundLoading;
    s8 dataInstall;
    s8 scope;
    s8 cameraType;
    s8 cameraSetting;
    s8 aiming;
    s8 manualLobbySelection;
    s8 xaxisControl;
    s8 bowgunShoulderCam;
};

struct NetPlayerData {
    u8 active;
    u8 pad_0x1[0x8C - 0x1];
};

struct GameSys : Singleton<GameSys> {
    enum {
        LOBBY_TASK = 1 << 0,
    };

    u8 pad_0x0[0x1C];
    u16 short_0x1C;
    u8 pad_0x1E[0x28 - 0x1E];
    u8 player_id;
    u8 pad_0x29;
    u8 flag_0x2A;
    u8 pad_0x2B[0x2E - 0x2B];
    u8 byte_0x2E;
    u8 pad_0x2F;
    NetPlayerData netPlayers[4];
    u8 pad_0x260[0x390-0x260];
    InventoryEntry itemBox[32];
    u8 pad_0x410[0x422-0x410];
    bool is_demo;
    u8 pad_0x423[0x480 - 0x423];
    bool is_gallery_task;
    u8 pad_0x481[0x4A0 - 0x481];
    UserData userData;
    Options options;
    u8 pad_0x6ADE6[0x6AF0C - 0x6ADE6];
    bool flag_0x6AF0C;
    u16 stage_id;
    u8 map_id;
    u16 mission_id;
    u32 flags_0x6AF14;
    u8 pad_0x6AF18[0x6AF1B - 0x6AF18];
    u8 flag_0x6AF1B;
    u8 pad_0x6AF1C[0x6AF40 - 0x6AF1C];

    GameSys();
    ~GameSys();

    InventoryEntry *method_088567AC(u32 index);
    int bagQuantity(u16 itemId);
    char *method_0885143C(u16);
    char *method_08851448(u16);
    char *weaponBlademasterName(u16 id);
    char *weaponGunnerName(u16 id);
    char *armorHeadName(u16 id);
    char *armorChestName(u16 id);
    char *armorArmName(u16 id);
    char *armorWaistName(u16 id);
    char *armorLegName(u16 id);
    s16 itemBoxSize();
    bool Game_clear_ck(int);
    bool Event_flag_ck(u16);
    u8 em_moto_type_ck(u8);
    void Event_flag_set(u16 event_flag_id);
    void Event_flag_clear(u16 event_flag_id);

    inline bool background_load_check() {
        return GameSys::objectPtr->options.backgroundLoading && !GameSys::objectPtr->is_gallery_task;
    }

    inline bool isLobby() {
        return (flags_0x6AF14 & LOBBY_TASK) != 0;
    }
};


extern "C" {
    char *func_eboot_088515A0(GameSys *, u16);
    int func_eboot_088566DC(GameSys *);
}
