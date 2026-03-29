#pragma once

#include "common.h"
#include "cache.hpp"
#include "singleton.hpp"
#include "obj_base.hpp"

struct ObjManager : Singleton<ObjManager> {
    enum MapEntryType {
            MEM_GET_NOUSE,
            MEM_GET_FIX,
            MEM_GET_FLOAT,
    };

    struct MapEntry {
        u8 load;
        u32 size;
    };

    void reset();
    void clear();
    void call_vtable_0x14(int index);
    void registerObjDraw(int index);
    void setFixMemoryMap(MapEntry *map);
    void clearFixMemoryMap();
    void setFloatMemoryMap(MapEntry *map);
    void clearFloatMemoryMap();
    void reserveFixMemory(u32 resourceSize, u32 vramSize);
    void *allocateFixMemory(u32 size);
    void *getPlayerFixVram(int pl_id, int parts);
    void reserveFloatMemory(u32 resourceSize, u32 vramSize);
    void *allocateFloatMemory(u32 size);
    void releaseFixMemory();
    void releaseFloatMemory();
    bool acquireEditPlayerModel(u8 mapId, u8 sex);

    int loadFile(int fileId, u16 mapId);

    u8 slab[0x80000];
    cache cache;
    u8 padding_0x8001C[0xC];
    ObjBase *objListHeads[15];
    ObjBase *objListTails[15];
    u32 unknown_0x800A0;
    u8 unknown_0x800A4;
    MapEntry map[48];
    u32 fixResourceSize;
    u32 fixResourceUsed;
    void *fixResourcePtr;
    u32 fixVramSize;
    u32 fixVramUsed;
    void *fixVramPtr;
    u32 floatResourceSize;
    u32 floatResourceUsed;
    void *floatResourcePtr;
    u32 floatVramSize;
    u32 floatVramUsed;
    void *floatVramPtr;
    void *resourceAddresses[48];
    u32 resourceSizes[48];
    u8 resourceState[48];
    u16 resourceFileIds[48];

    ObjManager();
    ~ObjManager();
};
