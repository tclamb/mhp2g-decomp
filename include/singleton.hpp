#pragma once

template<typename T>
struct Singleton
{
    static T *objectPtr;

    inline Singleton() {
        objectPtr = (T*) this;
    }

    inline ~Singleton() {
        objectPtr = 0;
    }
};
