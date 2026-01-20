#include "common.h"

typedef struct data_loader_vtable data_loader_vtable;
typedef struct data_loader data_loader;
typedef struct load_request load_request;
typedef struct block_span block_span;

struct load_request {
    u16 status;
    u16 file_id;
    void *dst;
    u32 len;
    u32 block_offset;
    u32 start_offset;
    s32 *unknown_0x14;
    u8 unknown0x18;
    u8 want_key;
    u8 is_last_block;
    u8 want_decryption;
    u8 want_checksum;
    u8 unknown_0x1d[3];
};

struct data_loader_base {
    virtual void unknown_0x4();
    virtual void unknown_0x8();
    virtual void start_threads();
    virtual void unknown_0x10();
    virtual s32 file_blocks_size(u16);
    virtual s32 file_size(u16);
    virtual void unknown_0x20();
    virtual void unknown_0x24();
    virtual void unknown_0x28();
    virtual void unknown_0x2c();
    virtual void unknown_0x30();
    virtual s32 load_async(void*, u32, u8, void*, u8);
    virtual void unknown_0x38();
    virtual s32 is_loaded(u16);
    virtual void unknown_0x40();
    virtual SceUID load_sce_font_library(u32, u32);
    virtual void unknown_0x48();
};

struct block_span {
    u16 first_block;
    u16 num_blocks;
};

struct data_loader : data_loader_base {
    load_request request_ringbuf[128];
    u32 request_read_index;
    u32 request_write_index;
    u32 thread_state;
    SceUID file_descriptor;
    u32 unknown_0x1014;
    u32 data_file_stat_private0;
    u16 unknown_0x101c[129];
    u16 blocking_read_index;
    u32 unknown_0x1120[8];
    u8 data_index_2[0x1960];
    u32 unknown_0x2aa0[8];
    u32 file_id_to_first_block[6603];
    u32 unknown_0x91ec[2];
    u8 read_buffer[0x20000];
    u32 unknown_0x291f4[3];
    void *ge_edram_start;
    void *ge_edram_end;
    u32 unknown_flags;
    u32 unknown_0x2920c[4];
    SceUID fake_rofs_semaphore;
    u32 unknown_0x29220[8];
    s32 file_position;
    SceUID data_loader_thread;
    block_span file_id_to_block_span[6602];
    u32 unknown_0x[27];
    load_request *decrypting_request;
    u32 unknown_0x2f9e0[2];
    SceUID decryption_event_flag;
    SceUID decryption_thread;
    load_request *blocking_request;
    u8 is_data_file_encrypted;
    u8 unknown_0x2f9f5[3];
    u32 blocking_position;
    u32 key_lower;
    u32 key_upper;
    u8 unknown_0x2fa04;
    u8 unknown_0x2fa05[3];
    u32 unknown_0x2fa08;
    u32 unknown_0x2fa0c;
    SceUID transfer_event_flag;
    SceUID transfer_thread;
};
