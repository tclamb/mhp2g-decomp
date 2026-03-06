#pragma once

#include "cache.hpp"
#include "singleton.hpp"

struct tagged_cache : singleton<tagged_cache> {

    cache cache;
    u32 padding[5];
    u8 slab[0x83C000];
    struct index_entry {
        int tag;
        u8 *address;
        u32 size;
    } index[0x200];

    tagged_cache();
    ~tagged_cache();

    void reset(u32 size);
    u8 *alloc(int tag, u32 size);
    void free_all(int tag);
    void free(void *addr);
    u8 *find(int tag);

private:
    int next_empty_index();
};
