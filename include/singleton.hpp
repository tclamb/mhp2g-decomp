#pragma once

template<typename T>
struct Singleton {
    static T *objectPtr;
protected:
    inline Singleton();
    inline ~Singleton();
};

template<typename T> Singleton<T>::Singleton() { objectPtr = (T *)this; }
template<typename T> Singleton<T>::~Singleton() { objectPtr = 0; }

// temporary hack, try removing once more is matched
template<typename T>
struct NoInlineConstructorSingleton {
protected:
    NoInlineConstructorSingleton();
    inline ~NoInlineConstructorSingleton();
};

template<typename T> NoInlineConstructorSingleton<T>::NoInlineConstructorSingleton() { Singleton<T>::objectPtr = (T *)this;}
template<typename T> NoInlineConstructorSingleton<T>::~NoInlineConstructorSingleton() { Singleton<T>::objectPtr = 0;}
