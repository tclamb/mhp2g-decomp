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
    virtual u32 file_blocks_size(u16);
    virtual u32 file_size(u16);
    virtual void unknown_0x20();
    virtual void unknown_0x24();
    virtual void unknown_0x28();
    virtual void unknown_0x2c();
    virtual void unknown_0x30();
    virtual u32 load_async(void*, u32, u8, void*, u8);
    virtual void unknown_0x38();
    virtual int is_loaded(u16);
    virtual void unknown_0x40();
    virtual SceUID load_sce_font_library(u32, u32);
    virtual void unknown_0x48();
};

struct block_span {
    u16 first_block;
    u16 num_blocks;
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

    virtual void start_threads();
    virtual u32 file_blocks_size(u32 file_id);

    void calculate_file_block_spans();
    int file_has_sha1(u32 file_id);
    u32 next_decryption_key() volatile;
    void decrypt_buffer(u8 *data, s32 size, s32 prevSize) volatile;
    void set_decryption_key(u32 key) volatile;
    void initialize_fake_rofs_semaphore() volatile;
    void initialize_load_request_queue();
    char *data_bin_path();

    load_request load_request_ringbuf[128];
    u32 load_request_load_head;
    u32 load_request_write_head;
    u32 load_thread_status;
    SceUID file_descriptor;
    u8 unused_flag_0x1014;
    u8 padding_0x1015[3];
    u32 data_bin_first_sector;
    char sce_font_module_path[256];
    u8 blocking_access_flag;
    u8 padding_0x111d;
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
    volatile SceUID loader_thread_id;
    block_span file_id_to_block_span[6602];
    u32 unknown_0x[27];
    load_request *decrypting_request;
    u32 unknown_0x2f9e0[2];
    volatile SceUID sha1_event_flag_id;
    volatile SceUID sha1_thread_id;
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
    volatile SceUID transfer_event_flag_id;
    volatile SceUID transfer_thread_id;
};
