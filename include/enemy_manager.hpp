#pragma once

#include "common.h"
#include "enemy.hpp"
#include "singleton.hpp"

struct EnemyManager : Singleton<EnemyManager> {
    u8 pad_0x0[0x1220];
    Enemy *enemies[20];
    u8 pad_0x1270[0x1290 - 0x1270];

    u8 method_09AAEE58(u8, bool, bool);

    EnemyManager();

    inline Enemy *by_index(int i) {
        if (i < 0 || i >= 20) {
            return 0;
        }
        return enemies[i];
    }
};
