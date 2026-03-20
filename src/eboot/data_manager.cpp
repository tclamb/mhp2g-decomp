#include "data_manager.hpp"

#include "quest.hpp"
#include "resource_manager.hpp"
#include "singleton.hpp"
#include "file_sys.hpp"
#include "singleton.hpp"
#include "game_sys.hpp"

#include <pspsdk/pspthreadman.h>
#include <pspsdk/pspsuspend.h>
extern "C" {
    void sceKernelDcacheWritebackInvalidateAll();
    #include <pspsdk/pspdmac.h>
}

#pragma opt_unroll_loops on

template<> DataManager *Singleton<DataManager>::objectPtr;

void DataManager::entry::reset() {
    flags = 0;
    file_id = -1;
    buffer = 0;
}

DataManager::DataManager() {
    // empty
}

DataManager::~DataManager() {
    // empty
}

void DataManager::reset() {
    clear();
    f0x216_flag = false;
    ready_flag = true;
}

typedef int (*callback)(int, int, DataManager *);

int on_power_down(int error, int event, DataManager *cache);
int on_power_up(int error, int event, DataManager *cache);

extern "C"
void func_eboot_088AFDFC(callback, void *);

void DataManager::register_power_callbacks() {
    func_eboot_088AFDFC(on_power_down, this);
    func_eboot_088AFDFC(on_power_up, this);
}

inline void DataManager::reset_pointers() {
    volatile_memory_size = 0;
    volatile_memory = 0;
    write_head = volatile_memory;
    remaining_bytes = volatile_memory_size;
}


inline void DataManager::clear_entries() {
    entry *e = &entries[0];
    for (int i = 0; i < NUM_ENTRIES; ++i, ++e) {
        e->reset();
    }
    reset_pointers();
}

void DataManager::clear() {
    clear_entries();
    f0x216_flag = true;
    loading_flag = false;
}

inline void DataManager::clear_pacs_inner() {
    remaining_bytes = volatile_memory_size;
    write_head = volatile_memory;

    for (int i = 0x15; i <= NUM_ENTRIES - 1; ++i) {
        entries[i].flags &= ~(1 << 1);
    }

    Singleton<FileSys>::objectPtr->update_ringbuf(1);
}

void DataManager::clear_pacs() {
    if (loading_flag) {
        loading_flag = false;
        clear_pacs_inner();
        release_volatile_memory();
    }
    f0x216_flag = false;
    clear_entries();
}

void DataManager::update() {
    if (f0x216_flag) {
        if (!Singleton<GameSys>::objectPtr->flag_0x480) {
            if (ready_flag && !loading_flag && allocate_volatile_memory()) {
                loading_flag = true;
                clear_pacs_inner();
            }
        }
    }
    for (int i = 0; i < NUM_ENTRIES; ++i) {
        u16 flags = entries[i].flags;
        if (((flags & 1) != 0)
          && ((flags & 2) == 0)
          && !Singleton<FileSys>::objectPtr->is_loading(entries[i].file_id)) {
            if (entries[i].was_cancelled != 1) {
                entries[i].flags &= ~1;
                entries[i].flags |= 2;
            } else {
                entries[i].flags = 0;
            }
        }
    }
}

void DataManager::load(s32 index, s32 file_id, u32 size) {
    u32 load_size = Singleton<FileSys>::objectPtr->file_size(file_id);
    if (size != 0) {
        load_size = size;
    }
    entry &e = entries[index];
    bool notify_on_cancel;
    if (index >= 0x15 && 0x2a >= index) {
        u8 *buf;
        if (remaining_bytes >= load_size) {
            remaining_bytes -= load_size;
            write_head += load_size;
            buf = write_head - load_size;
        } else {
            buf = 0;
        }
        e.buffer = buf;
        if (e.buffer == 0) {
            return;
        }
        notify_on_cancel = true;
    } else {
        e.buffer = Singleton<ResourceManager>::objectPtr->alloc(0x14, load_size);
        notify_on_cancel = false;
    }
    e.was_cancelled = false;
    Singleton<FileSys>::objectPtr->load_file_async(file_id, e.buffer, -1, notify_on_cancel, &e.was_cancelled, true);
    e.flags |= 1;
    e.file_id = file_id;
}

void DataManager::duplicate(s32 destination_index, s32 source_index) {
    entry &destination = entries[destination_index];
    entry &source = entries[source_index];

    u32 size = Singleton<FileSys>::objectPtr->file_size(source.file_id);
    destination.buffer = Singleton<ResourceManager>::objectPtr->alloc(0x14, size);

    sceKernelDcacheWritebackInvalidateAll();
    sceDmacMemcpy(destination.buffer, source.buffer, size);

    destination.flags |= 2;
    destination.file_id = source.file_id;
}

u32 DataManager::copy(void *dst, s32 source_index, u32 size) {
    entry &source = entries[source_index];
    if (size == -1) {
        size = Singleton<FileSys>::objectPtr->file_size(source.file_id);
    }
    sceKernelDcacheWritebackInvalidateAll();
    sceDmacMemcpy(dst, source.buffer, size);
    return size;
}

inline s32 DataManager::next_emmodel_index() {
    s32 offset;

    Quest &global = *Singleton<Quest>::objectPtr;
    int target_0_count = global.targets[0].count_0x1e;

    entry *e = &entries[global.largeEnemyCount(target_0_count) + 9];
    for (offset = global.largeEnemyCount(target_0_count); offset < 4; ++offset) {
        if ((e->flags & 3) == 0) {
            break;
        }
        e++;
    }
    return offset;
}

inline s32 DataManager::next_pac_index() {
    Quest &global = *Singleton<Quest>::objectPtr;
    int target_0_count = global.targets[0].count_0x1e;
    entry *e = &entries[global.largeEnemyCount(target_0_count) + 9];
    for (s32 offset = global.largeEnemyCount(target_0_count); offset < 4; ++offset) {
        if ((e->flags & 3) == 0) {
            return offset;
        }
        e++;
    }
    return 4;
}

void DataManager::cache_emmodel(u8 em_id) {
    entry* src = &entries[0x16];
    int src_offset = 0;
    for (; src_offset < 4; ++src_offset) {
        u16 file_id = 0x17AB + em_id;
        if ((src->flags & 2) != 0 && (src->file_id == file_id)) {
            break;
        }
        src++;
    }

    s32 dst = next_emmodel_index();
    duplicate(9 + dst, 0x16 + src_offset);
}

bool DataManager::is_loaded(s32 index) {
    u16 flags = entries[index].flags;
    return flags & 1 && !(flags & 2);
}

void DataManager::free(s32 index) {
    entry &e = entries[index];
    Singleton<ResourceManager>::objectPtr->free(e.buffer);
    e.reset();
}

u8 *DataManager::emmodel_pac(u8 em_id) {
    entry *e = &entries[9];
    for (int i = 0; i < 4; ++i) {
        u16 file_id = 0x17AB + em_id;
        if (e->flags & 2 && e->file_id == file_id) {
            return e->buffer;
        }
        ++e;
    }
    return 0;
}

bool DataManager::is_pac_cached(u8 em_id) {
    return find_pac(em_id + 0x17AB) != -1;
}

s32 DataManager::find_pac(u16 file_id) {
    entry *e = &entries[0x15];
    for (s32 i = 0x15; i <= NUM_ENTRIES - 1; ++i) {
        if ((e->flags & 2) != 0 && e->file_id == file_id) {
            return i;
        }
        ++e;
    }
    return -1;
}

s8 DataManager::find_emmodel(u8 em_id) {
    entry *e = &entries[9];
    for (s8 i = 0; i < 4; ++i) {
        u16 file_id = 0x17AB + em_id;
        if ((e->flags & 2) != 0 && e->file_id == file_id) {
            return i;
        }
        ++e;
    }
    return -1;
}

void DataManager::load_emmodel(u8 em_id, u32 size, s32 index) {
    if (index == -1) {
        if (emmodel_pac(em_id) == 0) {
            Quest &global = *Singleton<Quest>::objectPtr;
            int target_0_count = global.targets[0].count_0x1e;
            entry *e = &entries[global.largeEnemyCount(target_0_count) + 9];
            for (s32 offset = global.largeEnemyCount(target_0_count); offset < 4; ++offset) {
                if ((e->flags & 3) == 0) {
                    u16 file_id = em_id + 0x17AB;
                    load(offset + 9, file_id, size);
                    return;
                }
                e++;
            }
        }
    } else {
        entry *e = &entries[index + 9];
        if ((e->flags & 2) != 0) {
            u16 file_id = em_id + 0x17AB;
            if (e->file_id == file_id) {
                return;
            }
        }
        u16 file_id = em_id + 0x17AB;
        load(index + 9, file_id, size);
    }
}

extern "C" {
    struct sound_effect_file_ids {
        u16 phd_file_id;
        u16 bd_file_id;
        u16 tsb_file_id;
    };

    extern struct sound_effect_file_ids D_game_task_09BC3670[90];
}

void DataManager::load_em_with_sfx(u8 em_id, int n) {
    load_emmodel(em_id);
    load(n + 0x1E, D_game_task_09BC3670[em_id].phd_file_id, 0);
    load(n + 0x22, D_game_task_09BC3670[em_id].bd_file_id, 0);
    load(n + 0x1A, D_game_task_09BC3670[em_id].tsb_file_id, 0);
}

void DataManager::load_emmodel(u8 em_id) {
    u8 *pac = emmodel_pac(em_id);
    if (pac == 0) {

        Quest &global = *Singleton<Quest>::objectPtr;
        int target_0_count = global.targets[0].count_0x1e;

        entry *e = &entries[global.largeEnemyCount(target_0_count) + 0x16];
        for (s32 offset = global.largeEnemyCount(target_0_count); offset < 4; ++offset) {
            if ((e->flags & 3) == 0) {
                u16 file_id = em_id + 0x17AB;
                load(offset + 0x16, file_id, 0);
                return;
            }
            e++;
        }
    }
}

struct implicit_converter {
    int value;

    inline implicit_converter(int x) : value(x) {}

    operator u16() { return value; }
};

void DataManager::free_emmodel(u8 em_id) {
    entry *e = &entries[9];
    for (int i = 0; i < 4; ++i) {
        if ((e->flags & 3) != 0 && implicit_converter(em_id + 0x17AB) == e->file_id) {
            free(i + 9);
        }
        ++e;
    }
}

void DataManager::free_all_emmodels() {
    entry *e = &entries[9];
    for (int i = 0; i < 4; ++i) {
        if (e->flags & 3) {
            free(i + 9);
        }
        ++e;
    }
}

bool DataManager::allocate_volatile_memory() {
    volatile_memory_size = 0;
    volatile_memory = 0;
    int error = sceKernelVolatileMemLock(0, (void**)&volatile_memory, &volatile_memory_size);
    if (error < 0) {
        volatile_memory_size = 0;
    }
    write_head = volatile_memory;
    remaining_bytes = volatile_memory_size;
    if (error == 0) {
        return true;
    }
    return false;
}

void DataManager::release_volatile_memory() {
    sceKernelVolatileMemUnlock(0);
    reset_pointers();
}

extern "C"
void func_eboot_0884EA44(void *);

int on_power_down(int error, int event, DataManager *cache) {
    if (event != PowerEventType::STANDBY && event != PowerEventType::SUSPENDING) {
        return 0;
    }
    Singleton<GameSys>::objectPtr->flag_0x2A |= 1;
    cache->ready_flag = false;
    if (cache->loading_flag != 0) {
        func_eboot_0884EA44(Singleton<FileSys>::objectPtr);
        u32 handle = sceKernelSuspendDispatchThread();
        cache->loading_flag = false;
        cache->clear_pacs_inner();
        cache->release_volatile_memory();
        sceKernelResumeDispatchThread(handle);
    }
    return 0;
}

int on_power_up(int error, int event, DataManager *cache) {
    if (event != PowerEventType::RESUME_COMPLETE) {
        return 0;
    }
    Singleton<GameSys>::objectPtr->flag_0x2A &= 0xF0;
    Singleton<GameSys>::objectPtr->flag_0x6AF0C = true;
    cache->ready_flag = true;
    return 0;
}

extern "C" {
    struct short_pair {
        u16 left;
        u16 right;
    };

    extern struct short_pair D_game_sub_09CDF7B8[267];
    extern struct short_pair D_eboot_089A6470[267];
}

void DataManager::cache_stage(u16 st_id) {
    load(0x15, (u16)(st_id + 0x167C), 0); // st pac
    load(0x29, (u16)(st_id + 0x1572), 0); // st ovl
    func_eboot_088711B8(Singleton<Quest>::objectPtr, st_id);
    load(0x26, D_game_sub_09CDF7B8[st_id].left, 0);
    load(0x27, D_eboot_089A6470[st_id].left, 0);
    load(0x28, D_eboot_089A6470[st_id].right, 0);
}

bool DataManager::some_test() {
    return Singleton<GameSys>::objectPtr->unknownTest();
}
