#pragma once

#include "common.h"
#include "singleton.hpp"

struct LanguageId {
    enum {
        DEFAULT,
        ENGLISH,
        GERMAN,
        FRENCH,
        SPANISH,
        ITALIAN,
    };
private: LanguageId() {}
};

struct PowerEventType {
    enum {
        SUSPENDING = 1,
        STANDBY,
        RESUME_COMPLETE = 4,
    };
    private: PowerEventType() {}
};

struct DataManager : Singleton<DataManager> {
    enum {
        NUM_ENTRIES = 0x2B,
    };

    struct entry {
        u16 flags;
        u16 file_id;
        u8 *buffer;
        u32 was_cancelled;

        void reset();
    };

    entry entries[NUM_ENTRIES];
    int volatile_memory_size;
    u8 *volatile_memory;
    u8 *write_head;
    int remaining_bytes;
    bool loading_flag;
    bool ready_flag;
    bool f0x216_flag;

    DataManager();
    ~DataManager();

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

    friend int on_power_down(int, int, DataManager*);
    friend int on_power_up(int, int, DataManager*);

private:
    inline void clear_entries();
    inline void reset_pointers();
    inline void clear_pacs_inner();
    inline s32 next_emmodel_index();
    inline s32 next_pac_index();
};
