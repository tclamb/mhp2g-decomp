#include "vram_manager.hpp"

#include <pspsdk/pspge.h>
#include "ge.hpp"
#include "immediate_ge.hpp"

#pragma opt_unroll_loops on

template<> VramManager *Singleton<VramManager>::objectPtr;

void VramManager::method_08812A44() {
    vramStart = sceGeEdramGetAddr();
    vramSize = sceGeEdramGetSize();

    int i;
    for (i = 0; i < ALLOCATION_COUNT; ++i) {
        allocations[i].id = i;
        allocations[i].flags = 0;
    }
    for (i = 0; i < MASK_COUNT; ++i) {
        unknown_0x458[i] = 0;
    }
    unknown_0xC58[0] = 0;
    method_08812BD8(   0, 0x200, 0x110, 1, 0, INVALID_ADRS);
    method_08812BD8(   1, 0x200, 0x110, 1, 0, INVALID_ADRS);
    method_08812BD8(   2, 0x200, 0x110, 0, 0, INVALID_ADRS);
    method_08812BD8(0x2D, 0x100,  0x20, 3, 0, 0x1F8000);
}

u8 VramManager::method_08812BD8(u8 vramId, u16 width, u16 height, u16 imageFormat, u16 paletteWidth, u32 vramOffset) {
    if (vramId == INVALID_ID) {
        vramId = method_088130C8();
        if (vramId == INVALID_ID) {
            return INVALID_ID;
        }
    }

    VramAllocation &allocation = allocations[vramId];
    if (allocation.flags != 0) {
        return INVALID_ID;
    }

    allocation.texture.imageFormat = imageFormat;
    allocation.texture.paletteWidth = paletteWidth;

    VramTexture &texture = allocation.texture;
    u32 blockCount;
    switch (texture.imageFormat) {
    case 0:
    case 1:
    case 2:
        texture.alignedWidth = ((width + 7) / 8) * 8;
        texture.alignedHeight = ((height + 7) / 8) * 8;
        blockCount = (u32)(texture.alignedWidth * texture.alignedHeight * 2) >> 7;
        texture.vramBlockAddress = 0;
        break;
    case 3:
        texture.alignedWidth = ((width + 3) / 4) << 2;
        texture.alignedHeight = ((height + 7) / 8) << 3;
        blockCount = (u32)(texture.alignedWidth * texture.alignedHeight * 4) >> 7;
        texture.vramBlockAddress = 0;
        break;
    case 4:
        texture.alignedWidth = ((width + 31) / 32) << 5;
        texture.alignedHeight = ((height + 7) / 8) << 3;
        blockCount = (texture.alignedWidth * texture.alignedHeight / 2) / 128;
        texture.vramBlockAddress = method_08813188();
        if (texture.vramBlockAddress == (void *)INVALID_ADRS) {
            return INVALID_ID;
        }
        break;
    case 5:
        texture.alignedWidth = ((width + 15) / 16) << 4;
        texture.alignedHeight = ((height + 7) / 8) << 3;
        blockCount = (texture.alignedWidth * texture.alignedHeight) / 128;
        texture.vramBlockAddress = method_08813188();
        if (texture.vramBlockAddress == (void *)INVALID_ADRS) {
            return INVALID_ID;
        }
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    default:
        return INVALID_ID;
    }

    u32 blockStart;
    if (vramOffset == INVALID_ADRS) {
        blockStart = method_088130F8(blockCount);
        if (blockStart == INVALID_ADRS) {
            return INVALID_ID;
        }
        vramOffset = (blockStart << 7);
        texture.vramAddress = (u8 *)vramStart + vramOffset;
    } else {
        blockStart = vramOffset >> 7;
        texture.vramAddress = (void *)(vramOffset + (int)vramStart);
    }

    method_08813298(blockStart, blockCount);
    allocation.blockCount = blockCount;
    allocation.flags = 3;
    return vramId;
}

u8 VramManager::method_08812F04(u8 vramId, s32 size, u32 vramOffset) {
    if (vramId == INVALID_ID) {
        vramId = method_088130C8();
        if (vramId == INVALID_ID) {
            return INVALID_ID;
        }
    }
    VramAllocation &allocation = allocations[vramId];
    if (allocation.flags != 0) {
        return INVALID_ID;
    }

    VramTexture &texture = allocation.texture;
    u32 blockCount = (u32)(size + 0x7F) / 128;
    u32 blockStart;
    if (vramOffset == INVALID_ADRS) {
        blockStart = method_088130F8(blockCount);
        if (blockStart == INVALID_ADRS) {
            return 0xFF;
        }
        texture.vramAddress = (u8 *)vramStart + (blockStart << 7);
    } else {
        blockStart = vramOffset >> 7;
        texture.vramAddress = (void *)(vramOffset + (int)vramStart);
    }
    method_08813298(blockStart, blockCount);
    allocation.blockCount = blockCount;
    allocation.flags = 5;
    allocation.data[0].ui = (blockCount << 7);
    return vramId;
}

void VramManager::method_08813024(u8 vramId) {
    VramAllocation &allocation = allocations[vramId];
    int flags = allocation.flags;
    if (flags != 0) {
        u32 blockStart;
        if (((u8)flags & 2) != 0) {
            VramTexture &texture = allocation.texture;
            if (texture.vramBlockAddress != 0) {
                method_0881322C(texture.vramBlockAddress);
            }
            blockStart = ((u32)texture.vramAddress - (u32)vramStart) / 128;
        } else {
            blockStart = ((u32)allocation.texture.vramAddress - (u32)vramStart) / 128;
        }
        u32 blockCount = allocation.blockCount;
        method_088132FC(blockStart, blockCount);
        allocation.flags = 0;
    }
}

u8 VramManager::method_088130C8() {
    VramAllocation *transfer = &allocations[0];
    for (int i = 0; i < ALLOCATION_COUNT; ++i, ++transfer) {
        if (transfer->flags == 0) {
            return i;
        }
    }
    return INVALID_ID;
}

u32 VramManager::method_088130F8(u32 size) {
    s32 i;
    s32 runLength = 0;
    s32 result = 0;
    for (i = 0; i < MASK_COUNT << 5; ++i) {
        int v = i >> 5;
        if (i < 0) {
            v = (i + 0x1F) >> 5;
        }
        u32 t = 1 << (i % 32);
        if ((unknown_0x458[v] & t) == 0) {
            if (runLength == 0) {
                result = i;
            }
            ++runLength;
            if (runLength >= size) {
                return result;
            }
        } else {
            runLength = 0;
        }
    }
    return INVALID_ADRS;
}

void *VramManager::method_08813188() {
    void *vramPtr = method_088133D0(0x2D);
    if (vramPtr != (void *)INVALID_ADRS) {
        for (s32 i = 0; i < 0x20; ++i) {
            u32 t = 1 << ((s32)i % 32);
            int v = (s32)i >> 5;
            if ((s32)i < 0) {
                v = (s32)(i + 0x1F) >> 5;
            }
            if ((unknown_0xC58[v] & t) == 0) {
                unknown_0xC58[v] |= t;
                return (void *)((u32 *)vramPtr + i * 0x100);
            }
        }
    }
    return (void *)INVALID_ADRS;
}

void VramManager::method_0881322C(void *vramPtr) {
    void *last = method_088133D0(0x2D);
    if (last != (void *)INVALID_ADRS) {
        s32 i = ((u32)vramPtr - (s32)last) >> 15;
        unknown_0xC58[i] &= ~(1 << ext((u32)vramPtr - (s32)last, 10, 5));
    }
}

void VramManager::method_08813298(u32 blockStart, int blockCount) {
    for (s32 i = blockStart; i < blockStart + blockCount; ++i) {
        int t = 1 << (i % 32);
        int v = i >> 5;
        if (i < 0) {
            v = (i + 0x1F) >> 5;
        }
        unknown_0x458[v] |= t;
    }
}

void VramManager::method_088132FC(u32 blockStart, u32 blockCount) {
for (s32 i = blockStart; i < blockStart + blockCount; ++i) {
        int t = ~(1 << (i % 32));
        int v = i >> 5;
        if (i < 0) {
            v = (i + 0x1F) >> 5;
        }
        unknown_0x458[v] &= t;
    }
}

template<typename T> inline T *dummy(T *t) { return t; }

int VramManager::method_08813364(u8 vramId, VramAllocation *out) {
    if (allocations[vramId].flags != 0) {
        *out = allocations[vramId];
        return 1;
    }
    return 0;
}

void *VramManager::method_088133D0(u8 vramId) {
    if (allocations[vramId].flags != 0) {
        return allocations[vramId].texture.vramAddress;
    }
    return (void *)INVALID_ADRS;
}

u8 VramManager::method_088133FC(u8 vramId, tmh_header *tmh, u32 textureIndex, u32 imageIndex, u32 paletteIndex) {
    GeTexture texture;
    if ((u8)Ge::objectPtr->method_08859768(tmh, textureIndex, imageIndex, paletteIndex, &texture) == 0) {
        return INVALID_ID;
    }

    u8 copyId = method_08812BD8(vramId, texture.width, texture.height, texture.format, texture.palette_width, INVALID_ADRS);
    if (copyId == INVALID_ID) {
        return INVALID_ID;
    }

    VramAllocation allocation;
    if ((u8) method_08813364(copyId, &allocation) == 0) {
        method_08813024(copyId);
        return INVALID_ID;
    }

    u32 width;
    u32 height;
    switch (allocation.texture.imageFormat) {
    case 1:
        width = texture.width / 2;
        height = texture.height;
        break;
    case 3:
        width = texture.width;
        height = texture.height;
        break;
    case 4:
        width = texture.width / 8;
        height = texture.height;
        break;
    case 5:
        width = texture.width / 4;
        height = texture.height;
        break;
    default:
        method_08813024(copyId);
        return INVALID_ID;
    }

    ge_command display_list[32];
    u32 *write_head = &display_list[0];
    *write_head++ = (GE_CMD_TRANSFERSRC << 24) | ((u32)texture.data & 0xFFFFFF);
    *write_head++ = (GE_CMD_TRANSFERSRCW << 24) | (((u32)texture.data & 0xFF000000)) >> 8 | width;
    *write_head++ = (GE_CMD_TRANSFERDST << 24) | ((u32)allocation.texture.vramAddress & 0xFFFFFF);
    *write_head++ = (GE_CMD_TRANSFERDSTW << 24) | (((u32)allocation.texture.vramAddress & 0xFF000000)) >> 8 | width;
    *write_head++ = GE_CMD_TRANSFERSRCPOS << 24;
    *write_head++ = GE_CMD_TRANSFERDSTPOS << 24;
    *write_head++ = (GE_CMD_TRANSFERSIZE << 24) | ((height - 1) << 10) | (width - 1);
    *write_head++ = (GE_CMD_TRANSFERSTART << 24) | GE_TRANSFER_BPP_4;

    if (texture.palette_data != NULL) {
        u32 palette_size;
        switch (allocation.texture.paletteWidth) {
        case 1:
            palette_size = texture.palette_height / 2;
            break;
        case 3:
            palette_size = texture.palette_height;
            break;
        default:
            method_08813024(copyId);
            return INVALID_ID;
        }

        *write_head++ = (GE_CMD_TRANSFERSRC << 24) | ((u32)texture.palette_data & 0xFFFFFF);
        *write_head++ = (GE_CMD_TRANSFERSRCW << 24) | (((u32)texture.palette_data & 0xFF000000)) >> 8 | palette_size;
        *write_head++ = (GE_CMD_TRANSFERDST << 24) | ((u32)allocation.texture.vramBlockAddress & 0xFFFFFF);
        *write_head++ = (GE_CMD_TRANSFERDSTW << 24) | (((u32)allocation.texture.vramBlockAddress & 0xFF000000)) >> 8 | palette_size;
        *write_head++ = GE_CMD_TRANSFERSRCPOS << 24;
        *write_head++ = GE_CMD_TRANSFERDSTPOS << 24;
        *write_head++ = (GE_CMD_TRANSFERSIZE << 24) | (palette_size - 1);
        *write_head++ = (GE_CMD_TRANSFERSTART << 24) | GE_TRANSFER_BPP_4;
    }

    *write_head++ = GE_CMD_TEXSYNC << 24;
    *write_head++ = GE_CMD_BASE << 24;
    *write_head++ = GE_CMD_JUMP << 24;

    Ge::objectPtr->method_088593A0(display_list, write_head - &display_list[0], 1);

    return copyId;
}
