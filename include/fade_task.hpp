#pragma once

#include "singleton.hpp"
#include "task_base.hpp"

struct FadeTask : Singleton<FadeTask>, TaskBase {
    // TODO

    FadeTask();
};

extern "C" {
    void func_eboot_0884D658(FadeTask*, int, int, int);
}
