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

    void reset(void *slab, u32 len);
    void clear();
    void *alloc(u32 size, u32 align);
    void free(void* buf);
};