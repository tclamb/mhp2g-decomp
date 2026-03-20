#pragma once

#include "common.h"

struct Draw {
    virtual ~Draw() {}
    virtual void draw() = 0;

    enum {
        DISPOSE = 0x1,
        VISIBLE = 0x2,
    };

    u32 flags;
    Draw *next;
    float zindex;

    static inline void operator delete(void *) {}
};
