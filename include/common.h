#pragma once

#define INCLUDE_ASM(path, function)

#define ext(x, pos, size) (((x) >> (pos)) & ((1 << (size)) - 1))

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int64_t;
typedef unsigned long uint64_t;

#include <psptypes.h> // IWYU pragma: export
#include <pspkerneltypes.h> // IWYU pragma: export

extern "C" {
    void *memset(void*, int,  long unsigned int);
    void sceKernelDcacheWritebackRange(const void *p, unsigned int size);
    void sceKernelDcacheWritebackInvalidateRange(const void *p, unsigned int size);
    void sceKernelDcacheInvalidateRange(const void *p, unsigned int size);
}

struct stages {
    enum {
        GREAT_FOREST_N_2 = 259,
        GREAT_FOREST_N_7 = 264,
        TOWER_3 = 130,
    };

private:
    stages();
};
