// EnemyManager per-frame draw registration (called from func_game_task_09A5DB68).
// Object list 3 of ObjManager holds the monsters: every alive, not yet registered object is
// handed to DrawManager::addObj(render_group::GROUP_5, obj, false), which frustum-culls it and
// files it in the opaque bucket group 5 (or the alpha-sorted group 7 when obj->alpha != 0xFF).
#include "common.h"
#include "singleton.hpp"

struct ObjManager : Singleton<ObjManager> {
    void registerObjDraw(int index);
};
struct EnemyManager;

extern "C" void func_game_task_09AAC5E8(EnemyManager *) {
    ObjManager::objectPtr->registerObjDraw(3);
}
