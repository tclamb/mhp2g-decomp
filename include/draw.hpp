#pragma once

#include "common.h"

struct DrawFlag {
    enum {
        ACTIVE = 0x1,
        DRAW_QUEUED = 0x2,
        SKIP_DRAW = 0x4,
    };
private:
    DrawFlag();
};

struct Draw {
    virtual ~Draw() {}
    virtual void draw() = 0;

    u32 drawFlags;
    Draw *next;
    float zindex;

    static inline void operator delete(void *) {}
};
