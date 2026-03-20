#include "cache.hpp"

void cache::reset(void *s, u32 l) {
    len = l;
    first = (slab*)s;
    clear();
}

void cache::clear() {
    first->next = 0;
    first->prev = 0;
    first->next_partial = 0;
    first->prev_partial = 0;
    first->remainder = 0;
    first->free_block_start = 2;
    first->free_blocks = (len - 0x20) >> 4;
    head.next_partial = first;
}

inline u32 dummy(u32 x) {
    return x;
}

u8 *cache::alloc(u32 size, u32 align) {
    if (!size) {
        return 0;
    }

    u32 allocation_size = align + dummy(0x20) + size;
    u32 block_size = (allocation_size + 0xF) >> 4;

    slab_node *prev_partial = &head;
    slab* partial = (slab*)head.next_partial;
    if (partial) {
        loop_5:
        u32 free_block = partial->free_block_start;
        if (partial->free_blocks >= block_size) {
            u32 nalign = -align;
            u32 offset = free_block << 4;

            u32 remainder = partial->remainder;
            u8 *unaligned = (u8*)partial + offset + 0x20 - remainder;

            u32 next_aligned = ((~nalign) + ((u32) unaligned)) & nalign;
            slab *subslab = (slab*)(next_aligned - 0x20);

            remainder += (u32)subslab - (u32)partial - (partial->free_block_start << 4);

            slab_node *next = partial->next;
            subslab->next = next;
            if (next) {
                next->prev = subslab;
            }
            subslab->prev = partial;
            subslab->free_block_start = block_size;
            subslab->remainder = remainder;
            subslab->free_blocks = partial->free_blocks - block_size;
            partial->next = subslab;
            partial->free_blocks = 0U;

            partial->prev_partial = 0;
            slab_node *next_partial = partial->next_partial;
            if (subslab->free_blocks) {
                subslab->prev_partial = prev_partial;
                prev_partial->next_partial = subslab;
                subslab->next_partial = next_partial;
                if (next_partial) {
                    next_partial->prev_partial = subslab;
                }
            } else {
                subslab->next_partial = 0;
                subslab->prev_partial = 0;
                prev_partial->next_partial = next_partial;
                if (next_partial) {
                    next_partial->prev_partial = prev_partial;
                }
            }
            return (u8*)subslab + 0x20;
        }
        prev_partial = partial;
        partial = (slab*)partial->next_partial;
        if (partial) {
            goto loop_5;
        }
    }
    return 0;
}

void cache::free(void* memory) {
    if (!memory) {
        return;
    }

    slab *cur = (slab*)((u8*)memory - 0x20);

    slab *prev = (slab*)cur->prev;
    if (!prev) {
        return;
    }
    prev->free_blocks += cur->free_block_start + cur->free_blocks;

    cur->prev = 0;
    slab_node *next = cur->next;
    if (!next) {
        prev->next = 0;
    } else {
        prev->next = next;
        next->prev = prev;

    }

    slab_node *prevp = cur->prev_partial;
    slab_node *nextp = cur->next_partial;
    if (!prev->prev_partial) {
        // prev was full, becomes partial
        if (!prevp) {
            // by finding nearest partial link and inserting there
            slab_node *ancestor = prev->prev;
            slab_node *insert_partial = &head;
            while (ancestor) {
                if (ancestor->prev_partial != 0) {
                    insert_partial = ancestor;
                    break;
                }
                ancestor = ancestor->prev;
            }
            slab_node *insert_next_partial = insert_partial->next_partial;
            insert_partial->next_partial = prev;
            prev->prev_partial = insert_partial;
            prev->next_partial = insert_next_partial;
            if (insert_next_partial) {
                insert_next_partial->prev_partial = prev;
            }
        } else {
            // by replacing cur in the partial list
            prevp->next_partial = prev;
            prev->prev_partial = prevp;
            prev->next_partial = nextp;
            if (nextp) {
                nextp->prev_partial = prev;
            }
        }
    } else if (prevp) {
        // prev was already partial
        prev->next_partial = nextp;
        if (nextp) {
            nextp->prev_partial = prev;
        }
    }
}
