#pragma once

#include "common.h"
#include "singleton.hpp"

struct QuestNet : Singleton<QuestNet> {
    volatile s8 unknown_0x0;
    u8 pad_0x1[0x17F];

    QuestNet();
};
