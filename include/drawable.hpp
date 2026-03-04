#pragma once

struct drawable {
    virtual ~drawable() {}
    virtual void draw() = 0;

    static inline void operator delete(void *) {}
};
