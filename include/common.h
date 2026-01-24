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

typedef struct game game;

#include "io.hpp"

struct game {
    static game *instance;

    u16 next_index(u32 type);

    u8 padding_0x0[0xf7a3ac];
    u16 unknown_indexes[3];
    u8 padding_0xf7a3b2[0x4abe];
    SceUID sha1_thread_id;
    u8 unknown_flag;
};