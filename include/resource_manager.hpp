#pragma once

#include "cache.hpp"
#include "singleton.hpp"

struct ResourceType {
    enum {
        RESIDES_TMH = 0,
        TASK_STR_PAC = 4,
        DEMO_TMH = 6,
        COCK_TMH = 11,
        GUILD_TMH = 13,
        STG_ICON_TMH = 14,
        ARCADE_TMH = 15,
        GALLERY_TMH = 18,
    };

private:
    ResourceType();
};

struct ResourceManager : Singleton<ResourceManager> {
    cache cache;
    u32 padding[5];
    u8 slab[0x83C000];
    struct index_entry {
        int tag;
        u8 *address;
        u32 size;
    } index[0x200];

    ResourceManager();
    ~ResourceManager();

    void reset(u32 size);
    u8 *alloc(int tag, u32 size);
    void free_all(int tag);
    void free(void *addr);
    u8 *find(int tag);

private:
    int next_empty_index();
};
