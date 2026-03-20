#pragma once

#include "task_base.hpp"

struct ContTask : TaskBase {
    ContTask();
    virtual ~ContTask();
    virtual void load();

    u32 status;

    void on_load();
};
