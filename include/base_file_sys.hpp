#include "common.h"

enum load_request_state {
    LOAD_REQUEST_COMPLETE,
    LOAD_REQUEST_INCOMPLETE,
};

struct load_request {
    u16 state;
    u16 file_id;
    void *buf;
    u32 block_len;
    u32 block_offset;
    u32 file_len;
    u32 *is_cancelled_ptr;
    u8 unknown_flag;
    u8 needs_seed;
    u8 is_final_block;
    u8 needs_decryption;
    u8 needs_hashing;
    u8 padding[3];
};

struct install_block_offset {
    u16 block;
    u16 offset;
};

struct file_size_pair {
    u32 file_id;
    u32 size;
};

struct BaseFileSys {
    virtual ~BaseFileSys() {}
    virtual void initialize();
    virtual void stop() = 0;
    virtual void draw_loading_screen() = 0;
    virtual u32 file_size(u32) = 0;
    virtual u32 file_blocks_size(u32) = 0;
    virtual SceSize load_file_blocking(s32, u8*, SceSize) = 0;
    virtual void movie_open(u16) = 0;
    virtual void movie_read(u8*, SceSize) = 0;
    virtual void movie_close() = 0;
    virtual void movie_seek(int) = 0;
    virtual int load_file_async(s32, u8*, SceSize, u8, u32*, u8) = 0;
    virtual bool is_loading();
    virtual bool is_loading(u16 file_id);
    virtual bool is_loading(u8 unknown_flag);
    virtual SceUID load_libfont(u32, SceUID) = 0;
    virtual void vtable_0x48() = 0;

    load_request load_request_ringbuf[128];
    u32 load_request_load_head;
    u32 load_request_write_head;
    u32 load_thread_status;
    SceUID file_descriptor;
    u8 unused_flag_0x1014;
    u8 padding_0x1015[3];
    u32 data_bin_first_sector;
    char libfont_path[256];
    u8 is_playing_movie;
    u8 padding_0x111d;
    u16 movie_file_id;
    u32 unknown_0x1120[8];
    file_size_pair file_size_pairs[812];
    u32 unknown_0x2aa0[8];
    u32 file_id_to_first_block[6603];
    u8 unknown_0x91ec[20];
    u8 read_buffer[0x20000];
    void *ge_edram_start;
    void *ge_edram_end;
    u32 flag_0x29208;
    u8 flag_0x2920C;
    u32 flag_0x29210;
    u32 flag_0x29214;
    u32 flag_0x29218;
    SceUID fake_rofs_semaphore;

    void unload_module(SceUID module);
    void cancel_all();
    void update_ringbuf(u8 unknown_flag);
    void cancel(int i);
    void set_flags(int flag, u32 argument);
    void clear_flags();

protected:
    void draw_loading_screen_impl();
    void clear();
    void complete_one();

    inline int next(int i) {
        return (i + 1) & 0x7F;
    }

    inline void advance(u32 &i) {
        i = ++i & 0x7F;
    }
};
