#include "common.h"
#include "task_base.hpp"
#include "singleton.hpp"

struct GameTask : Singleton<GameTask>, TaskBase {
    // lookup quest string table 1
    char *method_09A5E4E0(u16 stringId);
};
