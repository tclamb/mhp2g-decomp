#include "common.h"
#include "singleton.hpp"
#include "draw.hpp"

struct DrawManager : Singleton<DrawManager> {
    int add(u8 group, Draw *object, ScePspFVector4 *position, bool no_culling);
};

struct ObjBase : Draw {
    bool inLoadedStage();
};

struct AttachedDraw : Draw {
    ScePspFVector4 position;   // +0x10
};

// object_089B6548 (an effect attached to a game object): while the owner is not registered for
// drawing itself and is in the loaded stage, registers its own draw object in the sorted
// translucent group 7.
struct AttachedEffect {
    u8 pad_0x0[0xC];
    ObjBase *owner;            // +0xC
    u8 pad_0x10[0x44 - 0x10];
    AttachedDraw *draw_44;     // +0x44
};

extern "C" void func_game_task_09AAC2D8(AttachedEffect *self) {
    if ((bool)(self->owner->flags & Draw::REGISTERED) == false) {
        if (self->owner->inLoadedStage() == true) {
            AttachedDraw *d = self->draw_44;
            DrawManager::objectPtr->add(7, d, &d->position, false);
        }
    }
}
