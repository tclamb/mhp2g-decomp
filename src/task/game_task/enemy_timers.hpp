#pragma once

#include "common.h"

// EnemyManager keeps two 32-entry lists of timed objects (+0x108 and +0x188) with live counts
// (+0x209, +0x20A). Each object's s16 at +0x1A counts down once per frame; at 0 the slot is freed.
struct EnemyTimedObj {
    u8 pad[0x1A];
    s16 timer;
};

struct EnemyManagerTimers {
    u8 pad_0x0[0x108];
    EnemyTimedObj *list_108[32];
    EnemyTimedObj *list_188[32];
    u8 pad_0x208;
    s8 count_209;
    s8 count_20A;
};
