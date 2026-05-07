#pragma once

#include "common.h"
#include "model_base.hpp" // FIXME: move tmh structs into proper header
#include "singleton.hpp"

struct VramTexture {
    u16 alignedWidth;
    u16 alignedHeight;
    void *vramAddress;
    u16 imageFormat;
    void *vramBlockAddress;
    u16 paletteWidth;
};

struct VramAllocation {
    u8 id;
    u8 flags;
    u16 blockCount;
    union {
        VramTexture texture;
        ScePspUnion32 data[5];
    };
};

struct VramManager : Singleton<VramManager> {
    enum {
        INVALID_ID = 0xFF,
        INVALID_ADRS = 0xFFFFFFFF,
        ALLOCATION_COUNT = 0x2E,
        MASK_COUNT = 0x200,
    };

    VramAllocation allocations[ALLOCATION_COUNT];
    void *vramStart;
    u32 vramSize;
    u32 unknown_0x458[MASK_COUNT];
    u32 unknown_0xC58[10];

    VramManager();

    void method_08812A44();
    u8 alloc(u8 vramId, u16 width, u16 height, u16 image_format, u16 palette_width, u32 vramOffset);
    u8 method_08812F04(u8 vramId, s32 size, u32 vramOffset);
    void method_08813024(u8 vramId);
    u8 method_088130C8();
    u32 method_088130F8(u32);
    void *method_08813188();
    void method_0881322C(void *);
    void method_08813298(u32, int);
    void method_088132FC(u32 blockStart, u32 blockCount);
    int method_08813364(u8 vramId, VramAllocation *out);
    void *method_088133D0(u8 vramId);
    u8 method_088133FC(u8 vramId, tmh_header *tmh, u32 textureIndex, u32 imageIndex, u32 paletteIndex);
};
