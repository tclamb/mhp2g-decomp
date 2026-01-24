#pragma once

#include "common.h"

#include <pspge.h>
#include <pspthreadman.h>
#include <pspthreadman_kernel.h>
#include <pspmodulemgr.h>
#include <pspthreadman_kernel.h>
#include <pspumd.h>
#include <pspiofilemgr_stat.h>

extern "C" {
    //#include <pspiofilemgr.h> # sceIoLseek needs to take 64-bit offset for byte-matching
    int sceIoLseek(SceUID fd, unsigned long long offset, int whence);
    int sceIoRead(SceUID fd, void *data, SceSize size);
    SceUID sceIoOpen(const char *file, int flags, SceMode mode);
    int sceIoClose(SceUID fd);
    int sceIoGetstat(const char *file, SceIoStat *stat);

    int memcmp(const void*, const void*, long unsigned int);
    void *memset(void*, int,  long unsigned int);
    int sprintf(char*, const char*, ...);
}

typedef struct data_loader_vtable data_loader_vtable;
typedef struct data_loader data_loader;
typedef struct load_request load_request;
typedef struct block_offset block_offset;

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

struct data_loader_base {
    virtual void unknown_0x4() = 0;
    virtual void unknown_0x8() = 0;
    virtual void start_threads() = 0;
    virtual void unknown_0x10() = 0;
    virtual u32 file_blocks_size(u32) = 0;
    virtual u32 file_size(u32) = 0;
    virtual SceSize load_file_blocking(s32, u8*, SceSize) = 0;
    virtual void movie_open(u16) = 0;
    virtual void movie_read(u8*, SceSize) = 0;
    virtual void movie_close() = 0;
    virtual void movie_seek(int) = 0;
    virtual int load_file_async(s32, u8*, SceSize, u8, u32*, u8) = 0;
    virtual void unknown_0x38() = 0;
    virtual int is_loaded(u16) = 0;
    virtual void unknown_0x40() = 0;
    virtual SceUID load_libfont(u32, SceUID) = 0;
    virtual void unknown_0x48() = 0;
};

struct install_block_offset {
    u16 block;
    u16 offset;
};

struct file_size_pair {
    u32 file_id;
    u32 size;
};

struct data_loader : data_loader_base {
    static u8 decrypt_table[];
    static u8 file_sha1_digests[][20];
    static u8 null_sha1_digest[];
    static char fake_rofs_semaphore_name[];
    static char data_bin_path_string[];
    static char umd_disc_drive_name[];
    static char loader_thread_name[];
    static char sha1_event_flag_name[];
    static char sha1_thread_name[];
    static char transfer_event_flag_name[];
    static char transfer_thread_name[];
    static char null_utility_message_dialog_header[];
    static char install_block_path_format[];
    static char lba_umd_access_path_format[];
    static char install_folder_path[];

    virtual void unknown_0x4() = 0;
    virtual void unknown_0x8() = 0;
    virtual void start_threads();
    virtual void unknown_0x10() = 0;
    virtual u32 file_blocks_size(u32);
    virtual u32 file_size(u32);
    virtual SceSize load_file_blocking(s32, u8*, SceSize);
    virtual void movie_open(u16);
    virtual void movie_read(u8*, SceSize);
    virtual void movie_close();
    virtual void movie_seek(int);
    virtual int load_file_async(s32, u8*, SceSize, u8, u32*, u8);
    virtual void unknown_0x38() = 0;
    virtual int is_loaded(u16) = 0;
    virtual void unknown_0x40() = 0;
    virtual SceUID load_libfont(u32, SceUID);
    virtual void unknown_0x48() = 0;

    void calculate_install_block_offsets();
    int file_has_sha1(u32 file_id);
    u32 next_decryption_key();
    void decrypt_buffer(u8 *data, s32 size, s32 prevSize);
    void set_decryption_key(u32 key);
    void initialize_fake_rofs_semaphore();
    void initialize_load_request_queue();
    char *data_bin_path();

    load_request load_request_ringbuf[128];
    u32 load_request_load_head;
    u32 load_request_write_head;
    u32 load_thread_status;
    volatile SceUID file_descriptor;
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
    volatile void *ge_edram_start;
    volatile void *ge_edram_end;
    u32 unknown_flags;
    u32 unknown_0x2920c[4];
    SceUID fake_rofs_semaphore;
    u32 unknown_0x29220[8];
    s32 movie_pos;
    volatile SceUID loader_thread_id;
    install_block_offset file_id_to_install_block_offset[6602];
    u8 sha1[20];
    u32 unknown_0x2f984;
    u32 sha1_state[8];
    u8 unknown_0x2f988[52];
    load_request *decrypting_request;
    u32 unknown_0x2f9e0[2];
    volatile SceUID sha1_event_flag_id;
    volatile SceUID sha1_thread_id;
    load_request *blocking_request;
    u8 is_data_file_encrypted;
    u8 unknown_0x2f9f5[3];
    u32 movie_open_pos;
    volatile u32 key_lower;
    volatile u32 key_upper;
    u8 unknown_0x2fa04;
    u8 unknown_0x2fa05[3];
    u32 unknown_0x2fa08;
    u32 unknown_0x2fa0c;
    volatile SceUID transfer_event_flag_id;
    volatile SceUID transfer_thread_id;
};
