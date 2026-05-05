#pragma once

#include "file_sys_base.hpp"

struct FileSys : FileSysBase {
    static FileSys *objectPtr;

    virtual ~FileSys() {}
    virtual void initialize();
    virtual void stop();
    virtual void draw_loading_screen();
    virtual u32 file_size(u32);
    virtual u32 file_blocks_size(u32);
    virtual SceSize load_file_blocking(s32, u8*, SceSize);
    virtual void movie_open(u16);
    virtual void movie_read(u8*, SceSize);
    virtual void movie_close();
    virtual void movie_seek(int);
    virtual int load_file_async(s32, u8*, SceSize, u8, u32*, u8);
    virtual bool is_loading();
    virtual bool is_loading(u16 file_id);
    virtual bool is_loading(u8 unknown_flag);
    virtual SceUID load_libfont(u32, SceUID);
    virtual void vtable_0x48();

    void calculate_install_block_offsets();
    int file_has_sha1(u32 file_id);
    u32 next_decryption_key();
    void decrypt_buffer(u8 *data, s32 size, s32 prevSize);
    void set_decryption_key(u32 key);
    const char *data_bin_path();

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
    u32 movie_open_pos;
    volatile u32 key_lower;
    volatile u32 key_upper;
    u8 unknown_0x2fa04;
    u32 unknown_0x2fa08;
    u32 unknown_0x2fa0c;
    volatile SceUID transfer_event_flag_id;
    volatile SceUID transfer_thread_id;
    u8 padding_0x2FA18[0x28];
}  __attribute__((aligned(16)));
