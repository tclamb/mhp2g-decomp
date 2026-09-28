// Shell type with an optional pair of sub-models (+0xE0, stride 0x10, only when the per-type table
// D_game_task_09BF91A0 says 1 and the type is 0x16), drawn translucent (group 7), plus the main
// model at +0x84 (group 5).
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
    u32 unknown_0x0;
    ShellDrawObj *draw;        // +0x4
    u8 pad_0x8[8];
};

struct ShellMulti {
    u8 pad_0x0[0xD];
    u8 type_0D;
    u8 pad_0xE[0x84 - 0xE];
    ShellDrawObj *draw_obj;    // +0x84
    u8 pad_0x88[0xE0 - 0x88];
    ShellSubModel subs[2];     // +0xE0 (type 0x16 only)
};

extern "C" s8 D_game_task_09BF91A0[];
extern "C" void func_game_task_09B62458(ShellMulti *, u8);

extern "C" void func_game_task_09B65C50(ShellMulti *self, u8 state) {
    u8 type = self->type_0D;
    switch (D_game_task_09BF91A0[type]) {
    case 1:
        switch (type) {
        case 0x16: {
            ShellSubModel *sub = self->subs;
            for (int i = 0; i < 2; i++, sub++) {
                ShellDrawObj *d = sub->draw;
                if (d != 0) {
                    if ((bool)(d->flags & Draw::REGISTERED) == false) {
                        DrawManager::objectPtr->add(7, d, &d->position, false);
                        if (state == 0) {
                            sub->draw->flags |= Draw::REGISTERED;
                        }
                    }
                }
            }
            break;
        }
        }
        break;
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
