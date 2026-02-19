#pragma once

#include "cache.hpp"

struct tagged_cache {
    static tagged_cache *INSTANCE;

    cache cache;
    u32 padding[6];
    u8 slab[0x83C000];
    struct index_entry {
        int tag;
        void *address;
        u32 size;
    } index[0x200];

    tagged_cache();
    ~tagged_cache();

    void reset(u32 size);
    void *alloc(int tag, u32 size);
    void free_all(int tag);
    void free(void *addr);
    void *find(int tag);

private:
    int next_empty_index();
};