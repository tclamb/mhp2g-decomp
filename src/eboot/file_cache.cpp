#include "common.h"
#include "io.hpp"
#include "tagged_cache.hpp"

#include <pspsdk/pspsuspend.h>
extern "C" {
    void sceKernelDcacheWritebackInvalidateAll();
    #include <pspsdk/pspdmac.h>
}

#define NUM_ENTRIES 0x2b

enum language_id {
    LANGUAGE_DEFAULT,
    LANGUAGE_ENGLISH,
    LANGUAGE_GERMAN,
    LANGUAGE_FRENCH,
    LANGUAGE_SPANISH,
    LANGUAGE_ITALIAN,
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
    void cancel_middle_entries();
    void update();
    void load(s32 slot, u16 file_id, u32 size);
    void duplicate(s32 dst_slot, s32 src_slot);
    u32 copy(void *dst, s32 src_slot, u32 size);
    void cache_emmodel(u8 em_id);
    bool is_loaded(s32 index);
    void free(s32 index);
    u8 *emmodel_pac(u8 em_id);
    bool is_emmodel_cached(u8 em_id);
    s32 find_middle_entries(u16 file_id);
    s32 find_emmodel(u8 em_id);
    void load_emmodel(u8 em_id, u32 size, s32 index);
    void load_em_with_sfx(u8 em_id, int n);
    void load_emmodel(u8 em_id);
    void free_emmodel(u8 em_id);
    void free_all_emmodels();
    SceBool allocate_volatile_memory();
    void release_volatile_memory();
    void cache_stage(u16 st_id);
    bool some_test();

private:
    inline void clear_entries();
    inline void reset_pointers();
    inline void cancel_middle_entries_inner();
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

typedef void (*callback)(int, int, void *);

extern "C"
void func_eboot_088AFDFC(callback, void *);

void on_power_down(int, int, void*);
void on_power_up(int, int, void*);

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
    for (int i = 0; i < NUM_ENTRIES; ++i) {
        entries[i].reset();
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

inline void file_cache::cancel_middle_entries_inner() {
    remaining_bytes = volatile_memory_size;
    write_head = volatile_memory;
    for (int i = 0x15; i < 0x24; ++i) {
        entries[i].flags &= ~2;
    }
    func_eboot_088BB548(D_eboot_089C7504, true);
}

#pragma opt_unroll_loops on
void file_cache::cancel_middle_entries() {
    if (loading_flag) {
        loading_flag = false;
        cancel_middle_entries_inner();
        release_volatile_memory();
    }
    f0x216_flag = false;
    clear_entries();
}
#pragma opt_unroll_loops reset

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

#pragma opt_unroll_loops on
void file_cache::update() {
    if (f0x216_flag) {
        if (!D_eboot_089C7508->flag_f0x480) {
            if (ready_flag && !loading_flag && allocate_volatile_memory()) {
                loading_flag = true;
                cancel_middle_entries_inner();
            }
        }
    }
    for (int i = 0; i < NUM_ENTRIES; ++i) {
        u16 flags = entries[i].flags;
        if (((flags & 1) != 0)
         && ((flags & 2) == 0)
         && !D_eboot_089C7504->is_loaded(entries[i].file_id)) {
            if (entries[i].was_cancelled) {
                entries[i].flags = 0;
            } else {
                entries[i].flags &= ~1;
                entries[i].flags |= 2;
            }
        }
    }
}
#pragma opt_unroll_loops reset

void file_cache::load(s32 index, u16 file_id, u32 size) {
    u32 load_size = D_eboot_089C7504->file_blocks_size(file_id);
    if (size != 0) {
        load_size = size;
    }
    bool notify_on_cancel;
    if (index >= 0x15 && 0x2a >= index) {
        if (load_size >= remaining_bytes) {
            entries[index].buffer = 0;
            return;
        }
        remaining_bytes -= load_size;
        write_head += load_size;
        entries[index].buffer = write_head - load_size;
        notify_on_cancel = true;
    } else {
        entries[index].buffer = tagged_cache::INSTANCE->alloc(0x14, load_size);
        notify_on_cancel = false;
    }
    entries[index].was_cancelled = false;
    D_eboot_089C7504->load_file_async(file_id, entries[index].buffer, -1, notify_on_cancel, &entries[index].was_cancelled, true);
    entries[index].flags |= 1;
    entries[index].file_id = file_id;
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
    struct global_08a5dd4c {
        u8 padding_0x0[0x77C];
        u8 byte_0x77C;
        u8 padding_0x77D[0x786 - 0x77D];
        u8 byte_0x786;
        u8 padding_0x787[0x7a8 - 0x787];
        u8 byte_0x7A8;
        u8 padding_0x7a9[0x7b2 - 0x7a9];
        u8 byte_0x7B2;
    };

    extern struct global_08a5dd4c *D_eboot_08A5DD4C;
}

void file_cache::cache_emmodel(u8 em_id) {
    int offset = 0;
    for (; offset < 4; ++offset) {
        entry &e = entries[offset + 0x16];
        if (e.flags & 2 != 0 && e.file_id == 6059 + em_id) {
            break;
        }
    }
    u32 count = 0;
    if (0 < D_eboot_08A5DD4C->byte_0x786) {
        count++;
    }
    if (0 < D_eboot_08A5DD4C->byte_0x7B2 && D_eboot_08A5DD4C->byte_0x77C != D_eboot_08A5DD4C->byte_0x7A8) {
        count++;
    }
    entry &e = entries[offset + count + 9];

    count = 0;
    if (0 < D_eboot_08A5DD4C->byte_0x786) {
        count++;
    }
    if (0 < D_eboot_08A5DD4C->byte_0x7B2 && D_eboot_08A5DD4C->byte_0x77C != D_eboot_08A5DD4C->byte_0x7A8) {
        count++;
    }

    if (count < 4) {
        for (; count < 4; count++) {
            if (entries[offset + count + 9].flags & 3 == 0) {
                break;
            }
        }
    }
    duplicate(count + 9, offset + 0x16);
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
    for (int i = 0; i < 4; ++i) {
        entry &e = entries[i + 9];
        if (e.flags & 2 && e.file_id == 0x17AB + em_id) {
            return e.buffer;
        }
    }
    return 0;
}

bool file_cache::is_emmodel_cached(u8 em_id) {
    return find_middle_entries(em_id + 0x17AB) != -1;
}

s32 file_cache::find_middle_entries(u16 file_id) {
    for (s32 i = 0x15; i <= 0x2a; ++i) {
        entry &e = entries[i];
        if (e.flags & 2 && e.file_id == file_id) {
            return i;
        }
    }
    return -1;
}

s32 file_cache::find_emmodel(u8 em_id) {
    for (s32 i = 0; i < 4; ++i) {
        entry &e = entries[i + 9];
        if (e.flags & 2 && e.file_id == em_id + 0x17AB) {
            return i;
        }
    }
    return -1;
}

void file_cache::load_emmodel(u8 em_id, u32 size, s32 index) {
    if (index == -1) {
        u8 *pac = emmodel_pac(em_id);
        if (pac == 0) {
            u32 count = 0 < D_eboot_08A5DD4C->byte_0x786;
            if (0 < D_eboot_08A5DD4C->byte_0x7B2 && D_eboot_08A5DD4C->byte_0x77C != D_eboot_08A5DD4C->byte_0x7A8) {
                count++;
            }
            int i = count + 9;
            count = 0 < D_eboot_08A5DD4C->byte_0x786;
            if (0 < D_eboot_08A5DD4C->byte_0x7B2 && D_eboot_08A5DD4C->byte_0x77C != D_eboot_08A5DD4C->byte_0x7A8) {
                count++;
            }
            for (; count < 4; ++count) {
                if (!(entries[i].flags & 3)) {
                    load(count + 9, em_id + 0x17AB, size);
                    return;
                }
            }
        }
        return;
    } else if (entries[index + 9].flags & 2 && entries[index + 9].file_id == em_id + 0x17AB) {
        return;
    }
    load(index + 9, em_id + 0x17AB, size);
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
    cache_emmodel(em_id);
    load(n + 0x1E, D_game_task_09BC3670[em_id].phd_file_id, 0);
    load(n + 0x22, D_game_task_09BC3670[em_id].bd_file_id, 0);
    load(n + 0x1A, D_game_task_09BC3670[em_id].tsb_file_id, 0);
}

void file_cache::load_emmodel(u8 em_id) {
    u8 *pac = emmodel_pac(em_id);
    if (pac == 0) {
        u32 count = 0 < D_eboot_08A5DD4C->byte_0x786;
        if (0 < D_eboot_08A5DD4C->byte_0x7B2 && D_eboot_08A5DD4C->byte_0x77C != D_eboot_08A5DD4C->byte_0x7A8) {
            count++;
        }
        int i = count + 0x16;
        count = 0 < D_eboot_08A5DD4C->byte_0x786;
        if (0 < D_eboot_08A5DD4C->byte_0x7B2 && D_eboot_08A5DD4C->byte_0x77C != D_eboot_08A5DD4C->byte_0x7A8) {
            count++;
        }
        for (; count < 4; ++count) {
            if (!(entries[i].flags & 3)) {
                load(count + 0x16, em_id + 0x17AB, 0);
                return;
            }
        }
    }
}

void file_cache::free_emmodel(u8 em_id) {
    for (int i = 0; i < 4; ++i) {
        entry &e = entries[i + 9];
        if (e.flags & 3 && e.file_id == em_id + 0x17AB) {
            free(i + 9);
        }
    }
}

void file_cache::free_all_emmodels() {
    for (int i = 0; i < 4; ++i) {
        if (entries[i + 9].flags & 3) {
            free(i + 9);
        }
    }
}

SceBool file_cache::allocate_volatile_memory() {
    volatile_memory_size = 0;
    volatile_memory = 0;
    SceBool success = sceKernelVolatileMemLock(0, (void**)&volatile_memory, &volatile_memory_size);
    if (success < 0) {
        volatile_memory_size = 0;
    }
    write_head = volatile_memory;
    remaining_bytes = volatile_memory_size;
    return success;
}

void file_cache::release_volatile_memory() {
    sceKernelVolatileMemUnlock(0);
    reset_pointers();
}

enum event_t {
    POWER_SUSPENDING = 1,
    POWER_STANDBY,
    POWER_RESUME_COMPLETE = 4,
};

extern "C"
void func_eboot_0884EA44(data_loader *);

#pragma opt_unroll_loops on
void on_power_down(int error, event_t event, file_cache *cache) {
    if (event == POWER_SUSPENDING || event == POWER_STANDBY) {
        D_eboot_089C7508->flag_0x2A |= 1;
        cache->ready_flag = false;
        if (cache->loading_flag) {
            func_eboot_0884EA44(D_eboot_089C7504);
            int handle = sceKernelSuspendDispatchThread();
            cache->loading_flag = false;
            cache->remaining_bytes = cache->volatile_memory_size;
            cache->write_head = cache->volatile_memory;
            for (int i = 0; i < 16; ++i) {
                cache->entries[0x15 + i].flags &= ~3;
            }
            func_eboot_088BB548(D_eboot_089C7504, true);
            cache->release_volatile_memory();
            sceKernelResumeDispatchThread(handle);
        }
    }
}
#pragma op_unroll_loops reset

void on_power_up(int error, event_t event, file_cache *cache) {
    if (event == POWER_RESUME_COMPLETE) {
        D_eboot_089C7508->flag_0x2A &= 0xF0;
        D_eboot_089C7508->flag_0x6AF0C = true;
        cache->ready_flag = true;
    }
}

extern "C" {
    struct short_pair {
        short left;
        short right;
    };

    extern struct short_pair D_game_sub_09CDF7B8[267];
    extern struct short_pair D_eboot_089A6470[267];

    void func_eboot_088711B8(struct global_08a5dd4c *, u16 st_id);
}

void file_cache::cache_stage(u16 st_id) {
    load(0x15, st_id + 0x167C, 0); // st pac
    load(0x29, st_id + 0x1572, 0); // st ovl
    func_eboot_088711B8(D_eboot_08A5DD4C, st_id);
    load(0x26, D_game_sub_09CDF7B8[st_id].left, 0);
    load(0x27, D_eboot_089A6470[st_id].left, 0);
    load(0x28, D_eboot_089A6470[st_id].right, 0);
}

bool file_cache::some_test() {
    return D_eboot_089C7508->flag_0x6ADDD && !D_eboot_089C7508->flag_f0x480;
}

extern "C" {
INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0AAC);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0AC0);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0AD0);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0B20);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0B50);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0B90);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0C04);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0D2C);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0EC0);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E0FDC);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1084);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1108);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E11F8);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1238);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E127C);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E12D0);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E12FC);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1348);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E13A0);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1514);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E15C0);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E16BC);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1740);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E17A4);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1808);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1848);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1988);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E19D4);

INCLUDE_ASM("/home/tclamb/mhp2g-decomp/asm/eboot/nonmatchings/file_cache", func_eboot_088E1AB0);
}
