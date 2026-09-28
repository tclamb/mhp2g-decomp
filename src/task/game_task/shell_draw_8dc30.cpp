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

struct ShellPartA {            // 0x60 bytes, from +0xF0
    u8 pad_0x0[0x44];
    ShellDrawObj *draw;        // +0x44
    u8 pad_0x48[0x5A - 0x48];
    u8 translucent;            // +0x5A: 0 -> group 5, else group 7
    u8 pad_0x5B[0x60 - 0x5B];
};

struct ShellPartB {            // 0xA0 bytes, from +0xE0 (types 0xF and 0x10)
    u8 pad_0x0[0x30];
    ShellDrawObj *draw;        // +0x30
    u8 pad_0x34[0xA0 - 0x34];
};

struct ShellParts {
    u8 pad_0x0[0xD];
    u8 type_0D;
    u8 pad_0xE[0xE0 - 0xE];
    union {
        ShellPartB parts_b[1];         // +0xE0
        struct {
            u8 pad_0xE0[0x10];
            ShellPartA parts_a[1];     // +0xF0
        } a;
    };
};

extern "C" s16 D_game_task_09C05B18[];
extern "C" void func_game_task_09B62458(ShellParts *, u8);

// Multi-part shells: the part count comes from a per-type table; types 0xF/0x10 use 0xA0-byte
// parts (translucent, group 7), the others 0x60-byte parts whose byte +0x5A picks group 5 or 7.
extern "C" void func_game_task_09B8DC30(ShellParts *self, u8 state) {
    u8 type = self->type_0D;
    int count = D_game_task_09C05B18[type];
    switch (type) {
    default: {
        int i;
        ShellPartA *part = self->a.parts_a;
        for (i = 0; i < count; i++, part++) {
            ShellDrawObj *d = part->draw;
            if (d != 0) {
                if ((bool)(d->flags & Draw::REGISTERED) == false) {
                    if (part->translucent == 0) {
                        DrawManager::objectPtr->add(5, d, &d->position, false);
                    } else {
                        DrawManager::objectPtr->add(7, d, &d->position, false);
                    }
                    if (state == 0) {
                        part->draw->flags |= Draw::REGISTERED;
                    }
                }
            }
        }
        break;
    }
    case 0xF:
    case 0x10: {
        ShellPartB *part = self->parts_b;
        for (int i = 0; i < count; i++, part++) {
            ShellDrawObj *d = part->draw;
            if (d != 0) {
                if ((bool)(d->flags & Draw::REGISTERED) == false) {
                    DrawManager::objectPtr->add(7, d, &d->position, false);
                    if (state == 0) {
                        part->draw->flags |= Draw::REGISTERED;
                    }
                }
            }
        }
        break;
    }
    }
    func_game_task_09B62458(self, state);
}
