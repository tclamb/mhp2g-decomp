#pragma once

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int64_t;
typedef unsigned long uint64_t;

#include <psptypes.h>
#include <pspkerneltypes.h>

extern "C" {
    void sceKernelDcacheWritebackRange(const void *p, unsigned int size);
    void sceKernelDcacheWritebackInvalidateRange(const void *p, unsigned int size);
    void sceKernelDcacheInvalidateRange(const void *p, unsigned int size);
}

#include "game.hpp"
#include "io.hpp"