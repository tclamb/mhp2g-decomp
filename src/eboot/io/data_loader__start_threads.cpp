#include "io.hpp"

#include <pspiofilemgr.h>
#include <pspumd.h>
#include <pspthreadman_kernel.h>

extern "C" int fake_rofs_loader(SceSize, void*);
extern "C" int sha1_thread(SceSize, void*);
extern "C" int transfer_thread(SceSize, void*);

struct unknown {
    u8 padding[0xf7ee70];
    SceUID sha1_thread_id;
    u8 flag;
};

extern "C" unknown *D_08A5E270;

void data_loader::start_threads() {
    struct {
        SceIoStat sp10;
        u32 uVar3;
        u32 dummy;
        data_loader *local_4;
    } sp;
    register unknown *puVar1;
    register int SVar5;

    initialize_fake_rofs_semaphore();
    sceUmdActivate(1, umd_disc_drive_name);
loop_1:
    if (sceUmdGetErrorStat() != 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_1;
    }
loop_4:
    while ((0 < (u32)(sceUmdGetDriveStat() & 0x20)) ^ 1 != 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_4;
    }
loop_5:
    SVar5 = sceIoOpen(data_bin_path(), 1, 0);
    file_descriptor = SVar5;
    if (SVar5 < 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_5;
    }
    sceKernelDcacheInvalidateRange(file_id_to_first_block, 0x6740);
loop_8:
    if (sceIoRead(file_descriptor, file_id_to_first_block, 0x672C) < 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_8;
    }
    sceKernelDcacheWritebackRange(file_id_to_first_block, 0x6740);
    set_decryption_key(0);
    decrypt_buffer((u8*)file_id_to_first_block, 0x672C, 0);
    sceKernelDcacheInvalidateRange(file_size_pairs, 0x1980);
loop_13:
    if (sceIoRead(file_descriptor, file_size_pairs, 0x1960) < 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_13;
    }
    sceKernelDcacheWritebackRange(file_size_pairs, 0x1980);
    decrypt_buffer((u8*)file_size_pairs, 0x1960, 0x672c);
loop_18:
    if (sceIoGetstat(data_bin_path(), &sp.sp10) < 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_18;
    }
    data_bin_first_sector = sp.sp10.st_private[1];
    calculate_file_block_spans();
    unused_flag_0x1014 = 3;
    initialize_load_request_queue();
    blocking_access_flag = 0;
    loader_thread_id = sceKernelCreateThread(loader_thread_name, fake_rofs_loader, 0x30, 0x1000, 0, 0);
    sp.local_4 = this;
    sceKernelStartThread(loader_thread_id, 4, &sp.local_4);
    sha1_event_flag_id = sceKernelCreateEventFlag(sha1_event_flag_name, 0x200, 0, 0);
    sha1_thread_id = sceKernelCreateThread(sha1_thread_name, sha1_thread, 0x31, 0x1000, 0, 0);
    sp.local_4 = this;
    sceKernelStartThread(sha1_thread_id, 4, &sp.local_4);
    puVar1 = D_08A5E270;
    puVar1->sha1_thread_id = sha1_thread_id;
    puVar1->flag = 1;
    transfer_event_flag_id = sceKernelCreateEventFlag(transfer_event_flag_name, 0x200, 0, 0);
    transfer_thread_id = sceKernelCreateThread(transfer_thread_name, transfer_thread, 0x13, 0x1000, 0, 0);
    sp.local_4 = this;
    sceKernelStartThread(transfer_thread_id, 4, &sp.local_4);
    is_data_file_encrypted = 1;
    unknown_0x2fa04 = 1;
    unknown_0x2fa08 = 0;
    unknown_0x2fa0c = 0;
}