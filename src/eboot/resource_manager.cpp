#include "resource_manager.hpp"

#pragma opt_unroll_loops on

template<> ResourceManager *Singleton<ResourceManager>::objectPtr;

ResourceManager::ResourceManager() {
    /// empty
}

ResourceManager::~ResourceManager() {
    // empty
}

void ResourceManager::reset(u32 size) {
    cache.reset(slab, size);

    int i = 0;
    while (i < 0x200) {
        index[i].tag = -1;
        index[i].address = 0;
        ++i;
    }
}

u8 *ResourceManager::alloc(int tag, u32 size) {
    s32 i = next_empty_index();
    if (i != -1) {
        index[i].address = cache.alloc(size, 0x10);
        if (index[i].address != 0) {
            index[i].size = size;
            index[i].tag = tag;
            return index[i].address;
        }
    }
    return 0;
}

void ResourceManager::free_all(int tag) {
    s32 i = 0;
    while (i < 0x200) {
        if (index[i].tag == tag) {
            cache.free(index[i].address);
            index[i].tag = -1;
        }
        i++;
    }
}

void ResourceManager::free(void *addr) {
    int i = 0;
    while (i < 0x200) {
        if (index[i].address == addr) {
            cache.free(index[i].address);
            index[i].tag = -1;
            return;
        }
        ++i;
    }
}

int ResourceManager::next_empty_index() {
    int i = 0;
    while (i < 0x200) {
        if (index[i].tag == -1) {
            return i;
        }
        ++i;
    }
    return -1;
}

u8 *ResourceManager::find(int tag) {
    int i = 0;
    while (i < 0x200) {
        if (index[i].tag == tag) {
            return index[i].address;
        }
        ++i;
    }
    return 0;
}
