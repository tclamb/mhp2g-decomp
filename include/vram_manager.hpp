#pragma once

#include "common.h"
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
    s16 blockCount;
    VramTexture texture;
};

struct VramManager : Singleton<VramManager> {
    enum {
        ALLOCATION_COUNT = 0x2E,
        INVALID_ID = 0xFF,
        INVALID_ADRS = 0xFFFFFFFF,
    };

    VramAllocation allocations[ALLOCATION_COUNT];
    void *vramStart;
    u32 vramSize;
    u32 unknown_0x458[0x200];
    u32 unknown_0xC5C[9];

    VramManager();

    u8 method_08812BD8(u8 vramId, u16 width, u16 height, u16 image_format, u16 palette_width, u32 vramOffset);
    u8 method_088130C8();
    void *method_08813188();
    u32 method_088130F8(u32);
    void method_08813298(u32, int);
    void *method_088133D0(u8 vramId);
};

extern "C" {
    void func_eboot_08812F04(VramManager *, s32, s32, s32);
    void func_eboot_08813024(VramManager *, u32);
    bool func_eboot_08813364(VramManager *, u8 vramId, VramAllocation *allocation);
}
