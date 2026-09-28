#pragma once

// Shells (projectiles) that own a separately drawn model object at +0x84. Their vtable +0x14
// method (called every frame from func_game_task_09B5E048, the ShellManager draw pass) registers
// that object with DrawManager (render group 5 = opaque buckets, 7 = sorted translucent, or a
// per-shell group byte at +0xE0), then chains to the common shell method func_game_task_09B62458.
#include "common.h"
#include "singleton.hpp"
#include "draw.hpp"

struct DrawManager : Singleton<DrawManager> {
    int add(u8 group, Draw *object, ScePspFVector4 *position, bool no_culling);
};

struct ShellDrawObj : Draw {
    u8 pad_0x10[0x10];
    ScePspFVector4 position;   // +0x20: depth-sort / cull position
};

struct ShellWithModel {
    u8 pad_0x0[0x84];
    ShellDrawObj *draw_obj;    // +0x84
    u8 pad_0x88[0xE0 - 0x88];
    u8 draw_group_E0;          // +0xE0 (only some shell types use it)
    u8 pad_0xE1[0x114 - 0xE1];
    ShellDrawObj *draw_obj_114; // +0x114 (second model, some shell types)
    u8 pad_0x118[0x12C - 0x118];
    ShellDrawObj *draw_obj_12C; // +0x12C (second model, some shell types)
};

// The common shell method takes the frame state; most callers pass it as a u8, one as an int
// (the declared type changes whether the caller re-masks it).
#ifdef SHELL_BASE_STATE_INT
extern "C" void func_game_task_09B62458(ShellWithModel *, int);
#else
extern "C" void func_game_task_09B62458(ShellWithModel *, u8);
#endif
