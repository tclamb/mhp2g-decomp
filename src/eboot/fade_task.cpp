#include "fade_task.hpp"

void FadeTask::load() {
    fadeState = 0;
    renderGroup = 0x11;
    totalFrames = 0;
    elapsedFrames = 0;
    unknown_0x28 = 0;
    alphaEnd = 0xFF;
    a = 0;
    b = 0;
    g = 0;
    r = 0;
    set_action((mem_fn)&FadeTask::draw);
}

INCLUDE_ASM("asm/eboot/nonmatchings/fade_task", draw__8FadeTaskFv);

// FadeTask::fade(s16, u8, bool). The code compares the last argument with 1 without masking it,
// which mwcc only does for an int, so it is defined here under its mangled name with an int.
// endVisible == 1: fade in to an opaque cover (state 2, alpha -> 255); otherwise fade the cover
// out (state 1, alpha -> 0). Either way it starts from the current alpha.
extern "C" void fade__8FadeTaskFsUcb(FadeTask *this_, s16 durationFrames, u8 unused, int endVisible) {
    this_->totalFrames = durationFrames;
    this_->elapsedFrames = 0;
    this_->unknown_0x28 = unused;
    if (endVisible == 1) {
        this_->fadeState = 2;
        this_->alphaEnd = 0xFF;
    } else {
        this_->fadeState = 1;
        this_->alphaEnd = 0;
    }
    this_->alphaStart = this_->a;
}

void FadeTask::fadeBlack(s16 durationFrames, bool endVisible) {
    fade(durationFrames, 0, endVisible);
    r = 0;
    g = 0;
    b = 0;
}

void FadeTask::fadeWhite(s16 durationFrames, bool endVisible) {
    fade(durationFrames, 0, endVisible);
    r = 0xFF;
    g = 0xFF;
    b = 0xFF;
}

FadeTask::~FadeTask() {}
