#pragma once

#include "task_base.hpp"

struct FadeTask : TaskBase {
    // TODO
};

extern "C" {
    void func_eboot_0884D658(FadeTask*, int, int, int);
}
