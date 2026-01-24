#include "io.hpp"

int data_loader::load_file_async(s32 file_id, u8* buf, SceSize len, u8 unknown_flag, u32 *cancellation_ptr, u8 wakeup_loader_thread) {
    u8 sp3F;
    s32 sp38;
    u32 sp34;
    struct load_request *temp_a0_2;
    u32 temp_a0;
    u32 temp_v0;
    u32 temp_v0_2;
    u32 var_s2;
    u32 var_s3;
    u8 var_s1;
    u8 var_s0;

    var_s3 = len;
    var_s2 = -1U;
    sp3F = wakeup_loader_thread;
    sp38 = sceKernelSuspendDispatchThread();
    if (var_s3 == var_s2) {
        var_s3 = file_blocks_size(file_id);
    }
    sp34 = var_s3;
    var_s2 = 0;
    var_s1 = 1;
    var_s0 = 0;
    if ((is_data_file_encrypted != 0) && (((s32) (game::instance->next_index(1) & 0xFFFF) % 100) < 5)) {
        var_s0 = 1;
    }
loop_1:
    temp_a0 = load_request_write_head;
    load_request_ringbuf[temp_a0].state = 1;
    load_request_ringbuf[temp_a0].block_offset = var_s2;
    load_request_ringbuf[temp_a0].file_id = (u16) file_id;
    load_request_ringbuf[temp_a0].buf = buf;
    load_request_ringbuf[temp_a0].unknown_flag = unknown_flag;
    load_request_ringbuf[temp_a0].is_cancelled_ptr = cancellation_ptr;
    load_request_ringbuf[temp_a0].needs_seed = var_s1;
    load_request_ringbuf[temp_a0].needs_decryption = is_data_file_encrypted;
    load_request_ringbuf[temp_a0].needs_hashing = var_s0;
    temp_a0_2 = &load_request_ringbuf[temp_a0];
    load_request_ringbuf[temp_a0].file_len = sp34;
    if ((s32) var_s3 <= 0x20000) {
        temp_a0_2->block_len = var_s3;
        temp_v0_2 = load_request_write_head + 1;
        load_request_write_head = temp_v0_2;
        load_request_write_head = temp_v0_2 & 0x7F;
        temp_a0_2->is_final_block = 1;
        goto loop_2;
    }
    temp_a0_2->block_len = 0x20000;
    var_s2 += 0x20000;
    temp_a0_2->is_final_block = 0;
    var_s3 += 0xFFFE0000;
    var_s1 = 0;
    temp_v0 = load_request_write_head + 1;
    load_request_write_head = temp_v0;
    load_request_write_head = temp_v0 & 0x7F;
    goto loop_1;

loop_2:
    sceKernelResumeDispatchThread(sp38);
    if (sp3F) {
        sceKernelWakeupThread(loader_thread_id);
    }
    return 1;
}
