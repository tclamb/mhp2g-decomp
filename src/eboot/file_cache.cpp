#include "common.h"
#include "io.hpp"
#include "tagged_cache.hpp"

#include <pspsdk/pspsuspend.h>
extern "C" {
    void sceKernelDcacheWritebackInvalidateAll();
    #include <pspsdk/pspdmac.h>
}

#define NUM_ENTRIES 0x2B
#pragma opt_unroll_loops on

enum language_id {
    LANGUAGE_DEFAULT,
    LANGUAGE_ENGLISH,
    LANGUAGE_GERMAN,
    LANGUAGE_FRENCH,
    LANGUAGE_SPANISH,
    LANGUAGE_ITALIAN,
};

enum event_t {
    POWER_SUSPENDING = 1,
    POWER_STANDBY,
    POWER_RESUME_COMPLETE = 4,
};

struct file_cache {
    struct entry {
        u16 flags;
        u16 file_id;
        u8 *buffer;
        u32 was_cancelled;

        void reset();
    };

    static file_cache *INSTANCE;

    entry entries[NUM_ENTRIES];
    int volatile_memory_size;
    u8 *volatile_memory;
    u8 *write_head;
    int remaining_bytes;
    bool loading_flag;
    bool ready_flag;
    bool f0x216_flag;
    u32 counter;
    u8 f0x21C[0xC];
    SceUID check_exit_game_thread;
    u32 f0x22C;
    u16 rng_state[3];
    language_id language;
    u32 f0x23C;

    file_cache();
    ~file_cache();
    void reset();
    void register_power_callbacks();
    void clear();
    void clear_pacs();
    void update();
    void load(s32 slot, s32 file_id, u32 size);
    void duplicate(s32 dst_slot, s32 src_slot);
    u32 copy(void *dst, s32 src_slot, u32 size);
    void cache_emmodel(u8 em_id);
    bool is_loaded(s32 index);
    void free(s32 index);
    u8 *emmodel_pac(u8 em_id);
    bool is_pac_cached(u8 em_id);
    s32 find_pac(u16 file_id);
    s8 find_emmodel(u8 em_id);
    void load_emmodel(u8 em_id, u32 size, s32 index);
    void load_em_with_sfx(u8 em_id, int n);
    void load_emmodel(u8 em_id);
    void free_emmodel(u8 em_id);
    void free_all_emmodels();
    bool allocate_volatile_memory();
    void release_volatile_memory();
    void cache_stage(u16 st_id);
    bool some_test();

    friend int on_power_down(int, event_t, file_cache*);
    friend int on_power_up(int, event_t, file_cache*);

private:
    inline void clear_entries();
    inline void reset_pointers();
    inline void clear_pacs_inner();
    inline s32 next_emmodel_index();
    inline s32 next_pac_index();
};

void file_cache::entry::reset() {
    flags = 0;
    file_id = -1;
    buffer = 0;
}

file_cache::file_cache() {
    INSTANCE = this;
}

file_cache::~file_cache() {
    if (this) {
        INSTANCE = 0;
    }
}

void file_cache::reset() {
    clear();
    f0x216_flag = false;
    ready_flag = true;
}

typedef int (*callback)(int, event_t, file_cache *);

extern "C"
void func_eboot_088AFDFC(callback, void *);

void file_cache::register_power_callbacks() {
    func_eboot_088AFDFC(on_power_down, this);
    func_eboot_088AFDFC(on_power_up, this);
}

inline void file_cache::reset_pointers() {
    volatile_memory_size = 0;
    volatile_memory = 0;
    write_head = volatile_memory;
    remaining_bytes = volatile_memory_size;
}


inline void file_cache::clear_entries() {
    entry *e = &entries[0];
    for (int i = 0; i < NUM_ENTRIES; ++i, ++e) {
        e->reset();
    }
    reset_pointers();
}

void file_cache::clear() {
    clear_entries();
    f0x216_flag = true;
    loading_flag = false;
}

extern "C" {
    extern data_loader *D_eboot_089C7504;
    void func_eboot_088BB548(data_loader *, bool);
}

inline void file_cache::clear_pacs_inner() {
    remaining_bytes = volatile_memory_size;
    write_head = volatile_memory;

    for (int i = 0x15; i <= NUM_ENTRIES - 1; ++i) {
        entries[i].flags &= ~(1 << 1);
    }

    func_eboot_088BB548(D_eboot_089C7504, true);
}

void file_cache::clear_pacs() {
    if (loading_flag) {
        loading_flag = false;
        clear_pacs_inner();
        release_volatile_memory();
    }
    f0x216_flag = false;
    clear_entries();
}

extern "C" {
    struct global_089C7508 {
        u8 pad_0x0[0x2A];
        u8 flag_0x2A;
        u8 pad_0x2B[0x480 - 0x2B];
        bool flag_f0x480;
        u8 pad_0x481[0x6addd - 0x481];
        s8 flag_0x6ADDD;
        u8 pad_0x6ADDE[0x6af0c - 0x6adde];
        bool flag_0x6AF0C;
    };
    extern struct global_089C7508 *D_eboot_089C7508;
}

void file_cache::update() {
    if (f0x216_flag) {
        if (!D_eboot_089C7508->flag_f0x480) {
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
          && !D_eboot_089C7504->is_loaded(entries[i].file_id)) {
            if (entries[i].was_cancelled != 1) {
                entries[i].flags &= ~1;
                entries[i].flags |= 2;
            } else {
                entries[i].flags = 0;
            }
        }
    }
}

void file_cache::load(s32 index, s32 file_id, u32 size) {
    u32 load_size = D_eboot_089C7504->file_blocks_size(file_id);
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
        e.buffer = tagged_cache::INSTANCE->alloc(0x14, load_size);
        notify_on_cancel = false;
    }
    e.was_cancelled = false;
    D_eboot_089C7504->load_file_async(file_id, e.buffer, -1, notify_on_cancel, &e.was_cancelled, true);
    e.flags |= 1;
    e.file_id = file_id;
}

void file_cache::duplicate(s32 destination_index, s32 source_index) {
    entry &destination = entries[destination_index];
    entry &source = entries[source_index];

    u32 size = D_eboot_089C7504->file_blocks_size(source.file_id);
    destination.buffer = tagged_cache::INSTANCE->alloc(0x14, size);

    sceKernelDcacheWritebackInvalidateAll();
    sceDmacMemcpy(destination.buffer, source.buffer, size);

    destination.flags |= 2;
    destination.file_id = source.file_id;
}

u32 file_cache::copy(void *dst, s32 source_index, u32 size) {
    entry &source = entries[source_index];
    if (size == -1) {
        size = D_eboot_089C7504->file_blocks_size(source.file_id);
    }
    sceKernelDcacheWritebackInvalidateAll();
    sceDmacMemcpy(dst, source.buffer, size);
    return size;
}

extern "C" {
    struct mission_target {
        typedef void *target_definition;
        target_definition definitions[5];
        s8 em_ids_0x14[5];
        u8 bytes_0x19[5];
        volatile s8 count_0x1e;
        u8 unknown_0x1f[11];
    };

    struct global_08a5dd4c {
        u8 padding_0x0[0x768];
        mission_target targets[2];
    };

    extern struct global_08a5dd4c *D_eboot_08A5DD4C;
}

inline s32 large_em_count(global_08a5dd4c &global, int target_0_count) {
    s32 result = 0;
    if (target_0_count > 0) {
        result += 1;
    }
    if ((global.targets[1].count_0x1e > 0) && (global.targets[0].em_ids_0x14[0] != global.targets[1].em_ids_0x14[0])) {
        result += 1;
    }
    return result;
}

inline s32 file_cache::next_emmodel_index() {
    s32 offset;

    global_08a5dd4c &global = *D_eboot_08A5DD4C;
    int target_0_count = global.targets[0].count_0x1e;

    entry *e = &entries[large_em_count(global, target_0_count) + 9];
    for (offset = large_em_count(global, target_0_count); offset < 4; ++offset) {
        if ((e->flags & 3) == 0) {
            break;
        }
        e++;
    }
    return offset;
}

inline s32 file_cache::next_pac_index() {
    global_08a5dd4c &global = *D_eboot_08A5DD4C;
    int target_0_count = global.targets[0].count_0x1e;
    entry *e = &entries[large_em_count(global, target_0_count) + 9];
    for (s32 offset = large_em_count(global, target_0_count); offset < 4; ++offset) {
        if ((e->flags & 3) == 0) {
            return offset;
        }
        e++;
    }
    return 4;
}

void file_cache::cache_emmodel(u8 em_id) {
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

bool file_cache::is_loaded(s32 index) {
    u16 flags = entries[index].flags;
    return flags & 1 && !(flags & 2);
}

void file_cache::free(s32 index) {
    entry &e = entries[index];
    tagged_cache::INSTANCE->free(e.buffer);
    e.reset();
}

u8 *file_cache::emmodel_pac(u8 em_id) {
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

bool file_cache::is_pac_cached(u8 em_id) {
    return find_pac(em_id + 0x17AB) != -1;
}

s32 file_cache::find_pac(u16 file_id) {
    entry *e = &entries[0x15];
    for (s32 i = 0x15; i <= NUM_ENTRIES - 1; ++i) {
        if ((e->flags & 2) != 0 && e->file_id == file_id) {
            return i;
        }
        ++e;
    }
    return -1;
}

s8 file_cache::find_emmodel(u8 em_id) {
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

void file_cache::load_emmodel(u8 em_id, u32 size, s32 index) {
    if (index == -1) {
        if (emmodel_pac(em_id) == 0) {
            global_08a5dd4c &global = *D_eboot_08A5DD4C;
            int target_0_count = global.targets[0].count_0x1e;
            entry *e = &entries[large_em_count(global, target_0_count) + 9];
            for (s32 offset = large_em_count(global, target_0_count); offset < 4; ++offset) {
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

void file_cache::load_em_with_sfx(u8 em_id, int n) {
    load_emmodel(em_id);
    load(n + 0x1E, D_game_task_09BC3670[em_id].phd_file_id, 0);
    load(n + 0x22, D_game_task_09BC3670[em_id].bd_file_id, 0);
    load(n + 0x1A, D_game_task_09BC3670[em_id].tsb_file_id, 0);
}

void file_cache::load_emmodel(u8 em_id) {
    u8 *pac = emmodel_pac(em_id);
    if (pac == 0) {

        global_08a5dd4c &global = *D_eboot_08A5DD4C;
        int target_0_count = global.targets[0].count_0x1e;

        entry *e = &entries[large_em_count(global, target_0_count) + 0x16];
        for (s32 offset = large_em_count(global, target_0_count); offset < 4; ++offset) {
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

void file_cache::free_emmodel(u8 em_id) {
    entry *e = &entries[9];
    for (int i = 0; i < 4; ++i) {
        if ((e->flags & 3) != 0 && implicit_converter(em_id + 0x17AB) == e->file_id) {
            free(i + 9);
        }
        ++e;
    }
}

void file_cache::free_all_emmodels() {
    entry *e = &entries[9];
    for (int i = 0; i < 4; ++i) {
        if (e->flags & 3) {
            free(i + 9);
        }
        ++e;
    }
}

bool file_cache::allocate_volatile_memory() {
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

void file_cache::release_volatile_memory() {
    sceKernelVolatileMemUnlock(0);
    reset_pointers();
}

extern "C"
void func_eboot_0884EA44(data_loader *);

int on_power_down(int error, event_t event, file_cache *cache) {
    if (event != POWER_STANDBY && event != POWER_SUSPENDING) {
        return 0;
    }
    D_eboot_089C7508->flag_0x2A |= 1;
    cache->ready_flag = false;
    if (cache->loading_flag != 0) {
        func_eboot_0884EA44(D_eboot_089C7504);
        u32 handle = sceKernelSuspendDispatchThread();
        cache->loading_flag = false;
        cache->clear_pacs_inner();
        cache->release_volatile_memory();
        sceKernelResumeDispatchThread(handle);
    }
    return 0;
}

int on_power_up(int error, event_t event, file_cache *cache) {
    if (event != POWER_RESUME_COMPLETE) {
        return 0;
    }
    D_eboot_089C7508->flag_0x2A &= 0xF0;
    D_eboot_089C7508->flag_0x6AF0C = true;
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

    void func_eboot_088711B8(struct global_08a5dd4c *, u16 st_id);
}

void file_cache::cache_stage(u16 st_id) {
    load(0x15, (u16)(st_id + 0x167C), 0); // st pac
    load(0x29, (u16)(st_id + 0x1572), 0); // st ovl
    func_eboot_088711B8(D_eboot_08A5DD4C, st_id);
    load(0x26, D_game_sub_09CDF7B8[st_id].left, 0);
    load(0x27, D_eboot_089A6470[st_id].left, 0);
    load(0x28, D_eboot_089A6470[st_id].right, 0);
}

bool file_cache::some_test() {
    return D_eboot_089C7508->flag_0x6ADDD && !D_eboot_089C7508->flag_f0x480;
}
