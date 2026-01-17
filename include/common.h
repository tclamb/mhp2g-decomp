typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int64_t;
typedef unsigned long uint64_t;

#include <psptypes.h>
#include <pspkerneltypes.h>

typedef struct data_loader_vtable data_loader_vtable;
typedef struct data_loader data_loader;
typedef struct load_request load_request;

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

struct data_loader_vtable {
    u32 unknown_0x0;
    u32 unknown_0x4;
    void *unknown_0x8;
    void (*data_loader__start_threads)(data_loader*);
    void *unknown_0x10;
    void *unknown_0x14;
    s32 (*data_loader__get_size)(data_loader*, u16);
    s32 (*data_loader__get_offset)(data_loader*, u16);
    void *unknown_0x20[5];
    s32 (*data_loader__load_async)(data_loader*, u16, void*, u32, u8, void*, u8);
    void *unknown_0x38;
    s32 (*data_loader__is_loaded)(data_loader*, u16);
    void *unknown_0x40;
    SceUID (*data_loader__load_sce_font_library)(data_loader*, u32, u32);
    void *unknown_0x48;
};

struct data_loader {
    data_loader_vtable *vtable;
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
    u8 data_index_1[0x672c];
    u32 unknown_0x91ec[2];
    u8 read_buffer[0x20000];
    u32 unknown_0x291f4[3];
    void *ge_edram_start;
    void *ge_edram_end;
    u32 unknown_0x29208;
    u32 unknown_0x2920c[4];
    SceUID data_loader_semaphore;
    u32 unknown_0x29220[8];
    s32 file_position;
    SceUID data_loader_thread;
    u32 unknown_0x29248[6629];
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
