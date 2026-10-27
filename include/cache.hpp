#pragma once

#include "common.h"

struct slab_node {
    struct slab_node *next;
    struct slab_node *prev;
    struct slab_node *next_partial;
    struct slab_node *prev_partial;
};

struct slab : slab_node {
    u32 free_block_start;
    u32 free_blocks;
    u32 remainder;
};

struct cache {
    u32 len;
    slab *first;
    slab_node head;
    u32 padding;

    void reset(void *slab, u32 len);
    void clear();
    u8 *alloc(u32 size, u32 align);
    void free(void* buf);

    template <typename T, typename R, typename M>
    static inline R *pullObjWork(M *manager) {
        T *memory = (T *)manager->cache.alloc(sizeof(T), alignof(ScePspFVector4));
        if (memory != 0) {
            memset(memory, 0, sizeof(T));
            T *obj = new (memory) T();
            R *result = obj;
            if (result == 0) {
                manager->cache.free(memory);
            }
            return result;
        }
        return 0;
    }

    template <typename T, typename M>
    static inline T *pullObjWork(M *manager) {
        return pullObjWork<T, T, M>(manager);
    }

    template <typename T, typename M>
    static inline void pushObjWork(T *set, M *manager) {
        delete set;
        manager->cache.free(set);
    }

    ~cache() {}
};
