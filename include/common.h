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

#include <psptypes.h>
#include <pspkerneltypes.h>

extern "C" {
    void *memset(void*, int,  long unsigned int);
    void sceKernelDcacheWritebackRange(const void *p, unsigned int size);
    void sceKernelDcacheWritebackInvalidateRange(const void *p, unsigned int size);
    void sceKernelDcacheInvalidateRange(const void *p, unsigned int size);
}

struct stages {
    enum {
        GREAT_FOREST_N_2 = 259,
        TOWER_3 = 130,
    };

private:
    stages();
};

extern "C" {
    // camera?
    extern struct global_089C6CB4 {
        float near_z;
        float far_z;
        u8 padding_0x8[0xB80 - 0x8];
        ScePspFMatrix4 perspective; // unsure
        ScePspFMatrix4 world;
        ScePspFMatrix4 projection;
        u8 padding_0xC40[0xCA0 - 0xC40];
        ScePspFVector4 position;
        u8 padding_0xCB0[0xDA0 - 0xCB0];
        ScePspVector3 viewport_scale;
        ScePspVector3 viewport_center;
    } *D_eboot_089C6CB4;

    // clipping test; objects are clipped when false
    int func_eboot_08816EA8(global_089C6CB4 *, ScePspFVector4 *position, float clipping_distance);
}
