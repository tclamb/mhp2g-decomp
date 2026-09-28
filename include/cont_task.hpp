#pragma once

#include "task_base.hpp"

struct ContTask : TaskBase {
    ContTask();
    virtual ~ContTask();
    virtual void load();

    u32 status;       // 0x1C
    u32 unknown_0x20; // 0x20 (sizeof(ContTask) == 0x24, from the task factory)

    void on_load();
};
