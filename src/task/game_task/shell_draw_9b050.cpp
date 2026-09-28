#include "common.h"
#include "singleton.hpp"
#include "draw.hpp"

struct DrawManager : Singleton<DrawManager> {
    int add(u8 group, Draw *object, ScePspFVector4 *position, bool no_culling);
};

struct ShellPartDraw : Draw {
    ScePspFVector4 position;   // +0x10
};

struct ShellPart {
    u8 pad_0x0[0x40];
    ShellPartDraw *draw;       // +0x40
    u8 pad_0x44[0xC];
};

struct ShellWithParts {
    u8 pad_0x0[0x24];
    u8 type_24;
    u8 pad_0x25[0x60 - 0x25];
    ShellPart parts[1];        // +0x60, count from D_game_task_09C06CC8[type_24]
};

extern "C" s16 D_game_task_09C06CC8[];

// Shell with up to N sub-parts (N from a per-type table), each owning a draw object at +0x40 of a
// 0x50-byte part record starting at +0x60; registers each part in the sorted translucent group 7.
extern "C" void func_game_task_09B9B050(ShellWithParts *self) {
    ShellPart *part = self->parts;
    s16 count = D_game_task_09C06CC8[self->type_24];
    for (int i = 0; i < count; i++, part++) {
        ShellPartDraw *d = part->draw;
        if (d != 0) {
            if ((bool)(d->flags & Draw::REGISTERED) != true) {
                DrawManager::objectPtr->add(7, d, &d->position, false);
            }
        }
    }
}
