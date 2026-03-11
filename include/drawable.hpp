#pragma once

struct drawable {
    virtual ~drawable() {}
    virtual void draw() = 0;

    enum {
        DISPOSE = 0x1,
        VISIBLE = 0x2,
    };

    u32 flags;
    drawable *next;
    float zindex;

    static inline void operator delete(void *) {}
};
