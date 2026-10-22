#pragma once

#include "common.h"
#include "enemy.hpp"
#include "singleton.hpp"

struct MissionFlag {
    enum {
        SLAYING = 1 << 0,
        GATHERING = 1 << 1,
        HUNTING = 1 << 2,
        SPECIAL = 1 << 3,
        TREASURE = 1 << 4,

        REPEL = 1 << 7,

        UNK_SHAKALAKA_0x800 = 1 << 11,

        UNK_0x4000 = 1 << 14,

        FORTRESS_DEFENSE = 1 << 18,
        PAW_PASS = 1 << 19,

        MARATHON = 1 << 21,
        EPIC_HUNTING = 1 << 22,
        TUTORIAL = 1 << 23,
        TRAINING = 1 << 24,
        URGENT = 1 << 25,
        KEY = 1 << 26,

        G_RANK = 1 << 29,
        UNK_G_RANK_0x40000000 = 1 << 30,
    };
private:
    MissionFlag();
};

struct ResourceNodeType {
    enum {
        PICKAXE = 3,
        BUG_NET = 4,
    };
private: ResourceNodeType();
};

struct ResourceNode {
    ScePspFVector3 position;
    float unknown_0xC; // interaction distance?
    u16 unknown_0x10; // loot table?
    u16 remaining;
    u16 type;
    u16 unknown_0x16; // high tier rolls?
};

struct QuestTargetDefinition;

struct QuestTarget {
    QuestTargetDefinition *definitions[5];
    s8 emId;
    u8 pad_0x15[4];
    u8 bytes_0x19[5];
    volatile s8 count_0x1e;
    u8 unknown_0x1f[11];
};

struct Quest : Singleton<Quest> {
    u8 pad_0x0[0x24];
    Enemy *camera_enemy;
    u32 mission_flags;
    u8 pad_0x2C[0x45 - 0x2C];
    s8 field_0x45;
    u8 pad_0x46[0x4C - 0x46];
    void *field_0x4C;
    u8 pad_0x50[0x720 - 0x50];
    Enemy *boss;
    u8 pad_0x724[0x768 - 0x724];
    QuestTarget targets[2];
    u8 pad_0x7C0[0x8A8 - 0x7C0];

    // TODO: expand scope, this probably should take no arguments
    inline s32 largeEnemyCount(int target_0_count) {
        s32 result = 0;
        if (target_0_count > 0) {
            result += 1;
        }
        if ((targets[1].count_0x1e > 0) && (targets[0].emId != targets[1].emId)) {
            result += 1;
        }
        return result;
    }

    Quest();

    int Quest_em_serch(u8 *em_types);
};

extern "C" {
    void func_eboot_088711B8(Quest *, u16 stage_id);

    ResourceNode *func_eboot_08869568(Quest *, int);

    bool func_eboot_0886A304(Quest *, bool);
    int func_eboot_0886A354(Quest *, bool);
}
