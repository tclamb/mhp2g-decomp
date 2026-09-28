#include "obj_manager.hpp"

#include "draw_manager.hpp"
#include "resource_manager.hpp"
#include "vram_manager.hpp"
#include "file_sys.hpp"

#pragma opt_unroll_loops on

//#define BUILD_NONMATCHING

template<> ObjManager *Singleton<ObjManager>::objectPtr;

ObjManager::ObjManager() {
    cache.reset(slab, sizeof(slab));
    for (int i = 0; i < 15; ++i) {
        objListHeads[i] = 0;
        objListTails[i] = 0;
    }
    unknown_0x800A0 = 0;
    memset(resourceAddresses, 0, sizeof(resourceAddresses));
    memset(resourceSizes, 0, sizeof(resourceSizes));
    memset(resourceState, 0, sizeof(resourceState));
    memset(resourceFileIds, 0xFF, sizeof(resourceFileIds));
    unknown_0x800A4 = 0;
    memset(map, 0, sizeof(map));
    fixResourceSize = 0;
    fixResourceUsed = 0;
    fixResourcePtr = 0;
    fixVramSize = 0;
    fixVramUsed = 0;
    fixVramPtr = 0;
    floatResourceSize = 0;
    floatResourceUsed = 0;
    floatResourcePtr = 0;
    floatVramSize = 0;
    floatVramUsed = 0;
    floatVramPtr = 0;
}

ObjManager::~ObjManager() {
    // empty
}

void ObjManager::reset() {
    for (int i = 0; i < 15; ++i) {
        objListHeads[i] = 0;
        objListTails[i] = 0;
    }
    unknown_0x800A0 = 0;
    cache.reset(slab, sizeof(slab));
}

void ObjManager::clear() {
    for (int i = 0; i < 15; ++i) {
        ObjBase *objPtr = objListHeads[i];
        if (objPtr) {
            ObjBase *next = objPtr->nextObj;
            delete objPtr;
            cache.free(objPtr);
            objPtr = next;

            while (objPtr) {
                next = objPtr->nextObj;
                delete objPtr;
                cache.free(objPtr);
                objPtr = next;
            }
        }
        objListHeads[i] = 0;
        objListTails[i] = 0;
    }
    unknown_0x800A0 = 0;
    cache.reset(slab, sizeof(slab));
}

void ObjManager::call_vtable_0x14(int index) {
    ObjBase *objPtr = objListHeads[index];
    while (objPtr) {
        objPtr->vtable_0x14();
        objPtr = objPtr->nextObj;
    }
}

void ObjManager::registerObjDraw(int index) {
    ObjBase *objPtr = objListHeads[index];
    while (objPtr) {
        if ((bool)(objPtr->flags & Draw::ALIVE) == true) {
            if ((bool)(objPtr->flags & Draw::REGISTERED) == false) {
                DrawManager::objectPtr->addObj(render_group::GROUP_5, objPtr, false);
            }
        }
        objPtr = objPtr->nextObj;
    }
}

void ObjManager::setFixMemoryMap(MapEntry *map) {
    MapEntry *entry = &map[0];
    for (int i = 0; i < 0x30; ++i, ++entry) {
        switch (entry->load) {
        case MEM_GET_NOUSE:
            break;
        case MEM_GET_FIX:
            if (entry->size != -1) {
                resourceAddresses[i] = allocateFixMemory(entry->size);
                resourceSizes[i] = entry->size;
            }
            this->map[i] = *entry;
            break;
        }
    }
}

void ObjManager::clearFixMemoryMap() {
    MapEntry *entry = &map[0];
    for (int i = 0; i < 0x30; ++i, ++entry) {
        if (entry->size != -1) {
            switch (entry->load) {
            case MEM_GET_NOUSE:
                break;
            case MEM_GET_FIX:
                resourceAddresses[i] = 0;
                resourceSizes[i] = 0;
                if (resourceState[i] != 3) {
                    resourceState[i] = 0;
                }
                map[i].load = MEM_GET_NOUSE;
                map[i].size = 0;
                break;
            }
        }
    }
}

void ObjManager::setFloatMemoryMap(MapEntry *map) {
    MapEntry *entry = &map[0];
    for (int i = 0; i < 0x30; ++i, ++entry) {
        switch (entry->load) {
        case MEM_GET_NOUSE:
            break;
        case MEM_GET_FLOAT:
            if (entry->size != -1) {
                resourceAddresses[i] = allocateFloatMemory(entry->size);
                resourceSizes[i] = entry->size;
            }
            this->map[i] = *entry;
            break;
        }
    }
}

void ObjManager::clearFloatMemoryMap() {
    MapEntry *entry = &map[0];
    for (int i = 0; i < 0x30; ++i, ++entry) {
        if (entry->size != -1) {
            switch (entry->load) {
            case MEM_GET_NOUSE:
                break;
            case MEM_GET_FLOAT:
                resourceAddresses[i] = 0;
                resourceSizes[i] = 0;
                if (resourceState[i] != 3) {
                    resourceState[i] = 0;
                }
                map[i].load = MEM_GET_NOUSE;
                map[i].size = 0;
                break;
            }
        }
    }
}

void ObjManager::reserveFixMemory(u32 resourceSize, u32 vramSize) {
    fixResourceSize = resourceSize;
    fixResourcePtr = ResourceManager::objectPtr->alloc(2, resourceSize);
    fixResourceUsed = 0;

    fixVramSize = vramSize;
    VramManager::objectPtr->method_08812F04(7, vramSize, VramManager::INVALID_ADRS);
    fixVramPtr = VramManager::objectPtr->method_088133D0(7);
    fixVramUsed = 0;
}

void *ObjManager::allocateFixMemory(u32 size) {
    u8 *result = (u8*)fixResourcePtr;
    result += fixResourceUsed;
    fixResourceUsed += size;
    return result;
}

#define VRAM_PL_COUNT 4
#define VRAM_ID_WEAPON 6

u32 D_eboot_089A3100[7] = {
    0, 0x5800, 0x6800, 0x9000, 0xE800, 0x12800, 0x17000
};

void *ObjManager::getPlayerFixVram(int pl_id, int parts) {
    u32 offset = pl_id * 0x1A800;
    u8 *result = (u8 *)fixVramPtr + offset;
    return result + D_eboot_089A3100[parts];
}

void *ObjManager::allocateFloatMemory(u32 size) {
    u8 *result = (u8*)floatResourcePtr;
    result += floatResourceUsed;
    floatResourceUsed += size;
    return result;
}

void ObjManager::reserveFloatMemory(u32 resourceSize, u32 vramSize) {
    if ((int)resourceSize > 0) {
        floatResourceSize = resourceSize;
        floatResourcePtr = ResourceManager::objectPtr->alloc(3, resourceSize);
        floatResourceUsed = 0;
    }

    floatVramSize = vramSize;
    VramManager::objectPtr->method_08812F04(8, vramSize, VramManager::INVALID_ADRS);
    floatVramPtr = VramManager::objectPtr->method_088133D0(8);
    floatVramUsed = 0;
}

void ObjManager::releaseFixMemory() {
    ResourceManager::objectPtr->free_all(2);
    fixResourcePtr = NULL;
    fixResourceSize = 0;
    fixResourceUsed = 0;

    VramManager::objectPtr->method_08813024(7);
    fixVramPtr = NULL;
    fixVramSize = 0;
    fixVramUsed = 0;
}

void ObjManager::releaseFloatMemory() {
    ResourceManager::objectPtr->free_all(3);
    floatResourcePtr = NULL;
    floatResourceSize = 0;
    floatResourceUsed = 0;

    VramManager::objectPtr->method_08813024(8);
    floatVramPtr = NULL;
    floatVramSize = 0;
    floatVramUsed = 0;
}

#ifdef BUILD_NONMATCHING
// edit player models
extern u16 D_edit_task_09A63210[2];

bool ObjManager::isEditPlayerModelLoaded(u8 mapId, u8 sex) {
    return (u8)loadFile(D_edit_task_09A63210[(u8) sex], (u8)(mapId << 3)) == true;
}
#else
INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", isEditPlayerModelLoaded__10ObjManagerFUcUc);
#endif

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_08866E08);

extern "C" int func_eboot_08866F64(ObjManager *mgr, u8 map, u16 file) {
    return (u8)mgr->loadFile((u16)(file + 0xD3C), (u8)(map * 8 + 6)) == true;
}

extern u16 D_eboot_089B16DC[2];

extern "C"
int func_eboot_08866FA8(ObjManager *this_, u8 a, u8 b, u8 c) {
    u16 sum = c + D_eboot_089B16DC[b];
    return (u8)this_->loadFile(sum, (u8)((a << 3) + 7)) == true;
}

extern "C" void func_eboot_088675B4(ObjManager *, u16);

extern "C" void func_eboot_08867000(ObjManager *mgr, u8 a, u8 b) {
    func_eboot_088675B4(mgr, (u8)(a * 8 + b));
}

extern "C" void func_eboot_08867018(ObjManager *mgr, u8 a) {
    func_eboot_088675B4(mgr, (u8)(a * 8 + 7));
}

extern "C" void func_eboot_0886702C(ObjManager *mgr, u8 a) {
    func_eboot_088675B4(mgr, (u8)(a * 8 + 6));
}

extern "C" void func_eboot_08867040(ObjManager *mgr, u8 a) {
    func_eboot_088675B4(mgr, (u8)(a * 8));
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_08867050);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_08867258);

int ObjManager::loadFile(int fileId, u16 mapId) {
    if ((u16)fileId == 0xFFFF) {
        return true;
    }
    u32 size;
    MapEntry &entry = map[mapId];
    u8 state = resourceState[mapId];
    switch (state) {
    case 0:
        size = FileSys::objectPtr->file_size(fileId);
        if (entry.size == -1) {
            switch (entry.load) {
            case MEM_GET_FIX:
                resourceAddresses[mapId] = allocateFixMemory(size);
                break;
            case MEM_GET_FLOAT:
                resourceAddresses[mapId] = allocateFloatMemory(size);
                break;
            }
            resourceSizes[mapId] = size;
        }
        ++resourceState[mapId];
        break;
    case 1:
        if (FileSys::objectPtr->load_file_async(fileId, (u8 *)resourceAddresses[mapId], -1, false, NULL, true) == 0) {
            break;
        }
        resourceState[mapId] = 2;
    case 2:
        if (FileSys::objectPtr->is_loading() == false) {
            resourceState[mapId] = 3;
            resourceFileIds[mapId] = fileId;
            return true;
        }
        break;
    case 3:
        return true;
    }

    return false;
}

extern "C" void func_eboot_088675B4(ObjManager *mgr, u16 id) {
    if (mgr->resourceState[id] == 0) {
        return;
    }
    if (mgr->resourceState[id] == 4) {
        mgr->resourceSizes[id] = 0;
    } else {
        ObjManager::MapEntry *entry = (ObjManager::MapEntry *)(id * 8 + (u8 *)mgr + 0x800A8);
        if (entry->size == (u32)-1) {
            switch (entry->load) {
            case ObjManager::MEM_GET_FIX:
                mgr->fixResourceUsed -= mgr->resourceSizes[id];
                break;
            case ObjManager::MEM_GET_FLOAT:
                mgr->floatResourceUsed -= mgr->resourceSizes[id];
                break;
            }
            mgr->resourceSizes[id] = 0;
        }
    }
    mgr->resourceState[id] = 0;
    mgr->resourceFileIds[id] = 0xFFFF;
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_088676D0);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_08867894);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_08867B2C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_08867D08);

extern "C" void func_eboot_08867F2C(ObjManager *, void *, void *, u8);

extern "C" int func_eboot_08867EC0(ObjManager *mgr, void *a, void *b) {
    for (int i = 0; i < 6; ++i) {
        func_eboot_08867F2C(mgr, a, b, (u8)i);
    }
    return 1;
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_08867F2C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_manager", func_eboot_0886822C);

extern "C" void func_eboot_08868490(ObjManager *mgr, void *record) {
    u8 *p = (u8 *)record;
    for (int i = 0; i < 6; ++i) {
        u8 index = (u8)i;
        if (*(void **)((index == 0 ? p + 0x50 : p + 0x830 + (index - 1) * 0x20) + 8) != NULL) {
            mgr->cache.free(*(void **)((index == 0 ? p + 0x50 : p + 0x830 + (index - 1) * 0x20) + 8));
        }
        memset(index == 0 ? p + 0x50 : p + 0x830 + (index - 1) * 0x20, 0, 0x20);
        if (*(void **)((index == 0 ? p + 0x80 : p + 0x8D0 + (index - 1) * 0x140) + 0x110) != NULL) {
            mgr->cache.free(*(void **)((index == 0 ? p + 0x80 : p + 0x8D0 + (index - 1) * 0x140) + 0x110));
        }
        memset(index == 0 ? p + 0x80 : p + 0x8D0 + (index - 1) * 0x140, 0, 0x140);
        if (*(void **)((index == 0 ? p + 0x70 : p + 0xF10 + (index - 1) * 0x10) + 4) != NULL) {
            mgr->cache.free(*(void **)((index == 0 ? p + 0x70 : p + 0xF10 + (index - 1) * 0x10) + 4));
        }
        memset(index == 0 ? p + 0x70 : p + 0xF10 + (index - 1) * 0x10, 0, 0x10);
    }
}

extern "C" void func_eboot_08868670(ObjManager *, void *entry) {
    *(u32 *)((u8 *)entry + 0x1AC) = 0;
}

extern "C" void func_eboot_08868678(ObjManager *mgr, void *record) {
    u8 *p = (u8 *)record;
    if (*(void **)(p + 0xF68) != NULL) {
        u16 player = *(u16 *)(p + 0x1E6);
        int slot = player << 3;
        slot += 6;
        if (mgr->map[slot].load == ObjManager::MEM_GET_FIX) {
            mgr->fixVramUsed -= 0x3800;
        }
        mgr->cache.free(*(void **)(p + 0xF68));
    }
    memset(p + 0xF60, 0, 0x20);
    if (*(void **)(p + 0x1090) != NULL) {
        mgr->cache.free(*(void **)(p + 0x1090));
    }
    memset(p + 0xF80, 0, 0x140);
    if (*(void **)(p + 0x10C4) != NULL) {
        mgr->cache.free(*(void **)(p + 0x10C4));
    }
    memset(p + 0x10C0, 0, 0x10);
    func_eboot_08868670(mgr, record);
}

extern "C" void func_eboot_08868770(ObjManager *mgr, void *record) {
    u8 *p = (u8 *)record;
    u8 type = p[0x10F1];
    if (*(void **)((type < 5 ? p + 0x50 : p + 0xF60) + 8) != NULL) {
        mgr->cache.free(*(void **)((type < 5 ? p + 0x50 : p + 0xF60) + 8));
    }
    memset(p[0x10F1] < 5 ? p + 0x50 : p + 0xF60, 0, 0x20);

    type = p[0x10F1];
    if (*(void **)((type < 5 ? p + 0x80 : p + 0xF80) + 0x110) != NULL) {
        mgr->cache.free(*(void **)((type < 5 ? p + 0x80 : p + 0xF80) + 0x110));
    }
    memset(p[0x10F1] < 5 ? p + 0x80 : p + 0xF80, 0, 0x140);

    type = p[0x10F1];
    if (*(void **)((type < 5 ? p + 0x70 : p + 0x10C0) + 4) != NULL) {
        mgr->cache.free(*(void **)((type < 5 ? p + 0x70 : p + 0x10C0) + 4));
    }
    memset(p[0x10F1] < 5 ? p + 0x70 : p + 0x10C0, 0, 0x10);
}

extern "C" void func_eboot_088688C0(ObjManager *mgr, void *record) {
    u8 *p = (u8 *)record;
    void *resource = *(void **)(p + 0x58);
    if (resource != NULL) {
        mgr->cache.free(resource);
    }
    memset(p + 0x50, 0, 0x20);
    resource = *(void **)(p + 0x190);
    if (resource != NULL) {
        mgr->cache.free(resource);
    }
    memset(p + 0x80, 0, 0x140);
    resource = *(void **)(p + 0x74);
    if (resource != NULL) {
        mgr->cache.free(resource);
    }
    memset(p + 0x70, 0, 0x10);
}

extern "C" void func_eboot_08868960(ObjManager *mgr, void *record) {
    u8 *p = (u8 *)record;
    void *resource = *(void **)(p + 0x58);
    if (resource != NULL) {
        mgr->cache.free(resource);
        if (p[0x1E8] == 0) {
            VramManager::objectPtr->method_08813024(0x29);
        }
    }
    resource = *(void **)(p + 0x190);
    if (resource != NULL) {
        mgr->cache.free(resource);
    }
    resource = *(void **)(p + 0x74);
    if (resource != NULL) {
        mgr->cache.free(resource);
    }
}
