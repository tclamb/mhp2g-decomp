#pragma once

#include "common.h"
#include "singleton.hpp"

struct EnemyManager : Singleton<EnemyManager> {
    u8 method_09AAEE58(u8, bool, bool);

    EnemyManager();
};
