#include "common.h"
#include "singleton.hpp"
#include "draw.hpp"

struct DrawManager : Singleton<DrawManager> {
    int add(u8 group, Draw *object, ScePspFVector4 *position, bool no_culling);
};

struct ShellDrawObj : Draw {
    u8 pad_0x10[0x10];
    ScePspFVector4 position;   // +0x20
};

struct ShellSubModel {
    s16 kind;                  // 2 = opaque (group 5), else translucent (group 7)
    u8 pad_0x2[2];
    ShellDrawObj *draw;        // +0x4
    u8 pad_0x8[8];
};

struct ShellTriple {
    u8 pad_0x0[0xD];
    u8 type_0D;
    u8 pad_0xE[0x84 - 0xE];
    ShellDrawObj *draw_obj;    // +0x84
    u8 pad_0x88[0x104 - 0x88];
    ShellSubModel subs[3];     // +0x104 (type 4 only)
};

extern "C" void func_game_task_09B62458(ShellTriple *, u8);

// Shell type 4 carries three sub-models (+0x104, stride 0x10) whose kind picks the render group,
// plus the main model at +0x84 (group 5).
extern "C" void func_game_task_09B7E0F0(ShellTriple *self, u8 state) {
    switch (self->type_0D) {
    case 4: {
        ShellSubModel *sub = self->subs;
        for (int i = 0; i < 3; i++, sub++) {
            ShellDrawObj *d = sub->draw;
            if (d != 0) {
                if ((bool)(d->flags & Draw::REGISTERED) == false) {
                    switch (sub->kind) {
                    case 2:
                        DrawManager::objectPtr->add(5, d, &d->position, false);
                        break;
                    default:
                        DrawManager::objectPtr->add(7, d, &d->position, false);
                        break;
                    }
                }
                if (state == 0) {
                    sub->draw->flags |= Draw::REGISTERED;
                }
            }
        }
        break;
    }
    }
    ShellDrawObj *d = self->draw_obj;
    if (d != 0) {
        if ((bool)(d->flags & Draw::REGISTERED) == false) {
            DrawManager::objectPtr->add(5, d, &d->position, false);
            if (state == 0) {
                self->draw_obj->flags |= Draw::REGISTERED;
            }
        }
    }
    func_game_task_09B62458(self, state);
}
