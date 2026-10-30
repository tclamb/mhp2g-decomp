#pragma once

#include "common.h"
#include "cache.hpp"
#include "singleton.hpp"
#include "obj_base.hpp"
#include "player.hpp"

struct ObjListId {
    enum {
        PLAYER = 2,
    };
private:
    ObjListId();
};

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

    inline void removeObj(ObjBase *obj) {
        ObjBase *next = obj->nextObj;
        ObjBase *prev = obj->prevObj;
        if (next == 0 && prev == 0) {
            if (objListHeads[obj->objListId] != obj) {
                return;
            }
            objListHeads[obj->objListId] = 0;
        }
        if (next != 0) {
            if (prev != 0) {
                next->prevObj = prev;
            } else {
                objListHeads[obj->objListId] = next;
                next->prevObj = 0;
            }
        }
        if (prev != 0) {
            if (next != 0) {
                prev->nextObj = next;
            } else {
                prev->nextObj = 0;
                objListTails[obj->objListId] = prev;
            }
        }
        objCount -= 1;
    }

    inline void addObj(int listId, ObjBase *obj) {
        if (objListHeads[listId] == NULL) {
            objListHeads[listId] = obj;
        } else {
            objListTails[listId]->nextObj = obj;
            obj->prevObj = objListTails[listId];
        }
        obj->objListId = listId;
        objListTails[listId] = obj;
        objCount += 1;
    }

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
    bool acquirePlayerModel(u8 playerId, u8 sexId, u8 type, u16 modelId, u8 skinId, int clothingId, int isArmor);
    bool acquirePlayerWeapon(u8 playerId, u16 modelId);
    bool acquirePlayerSkin(u8 playerId, int sexId, u16 modelId);
    void releasePlayerModel(u8 playerId, int type);
    void releasePlayerSkin(u8 playerId);
    void releasePlayerWeapon(u8 playerId);
    void bindPlayerModels(Player *player, u8 index);
    void bindPlayerWeapon(Player *player, u8 index);
    void clearModel(Player *player);
    void clearWeapon(Player *player);
    bool isEditPlayerModelLoaded(u8 mapId, u8 sex);

    int loadFile(int fileId, u16 mapId);

    u8 slab[0x80000];
    cache cache;
    u8 padding_0x8001C[0xC];
    ObjBase *objListHeads[15];
    ObjBase *objListTails[15];
    u32 objCount;
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
