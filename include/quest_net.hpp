#pragma once

#include "common.h"

struct QuestNet {
    volatile s8 unknown_0x0;
    u8 pad_0x1[0x17F];

    inline QuestNet();
};

QuestNet::QuestNet() {
    unknown_0x0 = 0;
}
