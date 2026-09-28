#pragma once

#include "singleton.hpp"
#include "task_base.hpp"

// Full-screen fade overlay (fade to/from black or white between scenes).
// draw() runs as the task action every frame and appends one flat-colored
// 480x272 SPRITES primitive to Ge render slot `renderGroup` (17 by default).
struct FadeTask : TaskBase, Singleton<FadeTask> {
    virtual void load();
    void draw();
    void fade(s16 durationFrames, u8 unused, bool endVisible);
    void fadeBlack(s16 durationFrames, bool endVisible);
    void fadeWhite(s16 durationFrames, bool endVisible);
    virtual ~FadeTask();
    FadeTask();

    u32 fadeState;     // 0x1C: 0 idle (nothing drawn), 1 fading out, 2 fading in, 3 holding
    s32 renderGroup;   // 0x20: Ge render slot (0..19), 17 after load()
    s16 totalFrames;   // 0x24
    s16 elapsedFrames; // 0x26
    u8 unknown_0x28;   // 0x28: the `unused` argument of fade()
    u8 alphaEnd;       // 0x29: target alpha (0 or 255; callers may lower it, e.g. Osk uses 192)
    u8 alphaStart;     // 0x2A: alpha when the fade started
    u8 unknown_0x2B;
    u8 r;              // 0x2C: overlay color (ABGR8888 vertex color bytes)
    u8 g;              // 0x2D
    u8 b;              // 0x2E
    u8 a;              // 0x2F: current alpha, recomputed by draw()
    u32 unknown_0x30;
};

// Inlined into the task factory (func_eboot_08895128, case 12). The original
// stores objectPtr = (this + 0x1C) - 0x1C with no downcast null check; only an
// integer subtraction gives that (a pointer cast adds `beqz`, a reference cast
// folds the +/-0x1C away).
template<> inline Singleton<FadeTask>::Singleton() { objectPtr = (FadeTask *)((u32)this - sizeof(TaskBase)); }
inline FadeTask::FadeTask() {}
