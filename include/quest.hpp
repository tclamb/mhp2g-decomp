#pragma once

#include "common.h"

struct ResourceType {
    enum {
        PICKAXE = 3,
        BUG_NET = 4,
    };
private: ResourceType();
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

struct Quest {
    u8 pad_0x0[0x768];
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
};

extern "C" {
    void func_eboot_088711B8(Quest *, u16 st_id);

    ResourceNode *func_eboot_08869568(Quest *, int);

    bool func_eboot_0886A304(Quest *, bool);
    int func_eboot_0886A354(Quest *, bool);
}
