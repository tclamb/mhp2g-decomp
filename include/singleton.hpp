#pragma once

template<typename T>
struct singleton
{
    static T *INSTANCE;

    inline singleton() {
        INSTANCE = (T*) this;
    }

    inline ~singleton() {
        INSTANCE = 0;
    }

    inline static T *get() {
        return INSTANCE;
    }
};
