#pragma once

#include "common.h"
#include "singleton.hpp"
#include "task_base.hpp"
#include "cache.hpp"

struct TaskManager : Singleton<TaskManager> {
    TaskBase *active[4];
    u8 slab[0x14000];
    cache cache;
    u8 unknown_0x1402C[0xC];

    TaskManager();
    ~TaskManager();
};
