#pragma once

#include "singleton.hpp"
#include "task_base.hpp"

struct FadeTask : Singleton<FadeTask>, TaskBase {
    virtual void load();
    void draw();
    void fade(s16 durationFrames, u8 unused, bool endVisible);
    void fadeBlack(s16 durationFrames, bool endVisible);
    void fadeWhite(s16 durationFrames, bool endVisible);
    virtual ~FadeTask();
    FadeTask();

    u32 fadeState;
    s32 renderGroup;
    s16 totalFrames;
    s16 elapsedFrames;
    u8 unknown_0x28;
    u8 alphaEnd;
    u8 alphaStart;
    u32 rgba;
    u32 unknown_0x30;
};
