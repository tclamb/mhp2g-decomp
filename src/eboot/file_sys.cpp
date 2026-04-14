#include "file_sys.hpp"
#include "memory_stick.hpp"
#include "singleton.hpp"
#include "system.hpp"
#include "pad.hpp"
#include "game_sys.hpp"

#include <pspge.h>
#include <pspthreadman.h>
#include <pspthreadman_kernel.h>
#include <pspmodulemgr.h>
#include <pspthreadman_kernel.h>
#include <pspumd.h>
#include <pspiofilemgr_stat.h>

#include "file_sys.file_sha1_digests.inc.cpp"
#include "file_sys.decrypt_table.inc.cpp"

extern "C" {
    int fake_rofs_loader(SceSize, void*);
    int sha1_thread(SceSize, void*);
    int transfer_thread(SceSize, void*);

    //#include <pspiofilemgr.h> # sceIoLseek needs to take 64-bit offset for byte-matching
    int sceIoLseek(SceUID fd, unsigned long long offset, int whence);
    int sceIoRead(SceUID fd, void *data, SceSize size);
    SceUID sceIoOpen(const char *file, int flags, SceMode mode);
    int sceIoClose(SceUID fd);
    int sceIoGetstat(const char *file, SceIoStat *stat);

    void *memset(void*, int,  long unsigned int);
}

void FileSys::initialize() {
    struct {
        SceIoStat sp10;
        u32 uVar3;
        u32 dummy;
        FileSys *local_4;
    } sp;
    System *puVar1;
    int SVar5;

    FileSysBase::initialize();
    sceUmdActivate(1, "disc0:");
loop_1:
    if (sceUmdGetErrorStat() != 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_1;
    }
    goto loop_6;
loop_4:
    sceKernelDelayThreadCB(10000);
loop_6:
    if (((0 < (u32)(sceUmdGetDriveStat() & 0x20)) ^ 1) != 0) {
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
    calculate_install_block_offsets();
    unused_flag_0x1014 = 3;
    clear();
    is_playing_movie = 0;
    loader_thread_id = sceKernelCreateThread("fakeRofsLoader", fake_rofs_loader, 0x30, 0x1000, 0, 0);
    sp.local_4 = this;
    sceKernelStartThread(loader_thread_id, 4, &sp.local_4);
    sha1_event_flag_id = sceKernelCreateEventFlag("sha1EventFlag", 0x200, 0, 0);
    sha1_thread_id = sceKernelCreateThread("sha1Thread", sha1_thread, 0x31, 0x1000, 0, 0);
    sp.local_4 = this;
    sceKernelStartThread(sha1_thread_id, 4, &sp.local_4);
    puVar1 = System::objectPtr;
    puVar1->sha1ThreadId = sha1_thread_id;
    puVar1->sha1ThreadStarted = true;
    transfer_event_flag_id = sceKernelCreateEventFlag("transferEventFlag", 0x200, 0, 0);
    transfer_thread_id = sceKernelCreateThread("transferThread", transfer_thread, 0x13, 0x1000, 0, 0);
    sp.local_4 = this;
    sceKernelStartThread(transfer_thread_id, 4, &sp.local_4);
    is_data_file_encrypted = 1;
    unknown_0x2fa04 = 1;
    unknown_0x2fa08 = 0;
    unknown_0x2fa0c = 0;
}

void FileSys::stop() {
    sceKernelDeleteThread(loader_thread_id);
    sceUmdDeactivate(2, "disc0:");

loop:
    if (sceUmdGetErrorStat() != 0) {
        sceKernelDelayThreadCB(10000);
        goto loop;
    }
}

// appears to be
void FileSys::draw_loading_screen() {
    static char buffer[0x400];
    s32 format_string_argument_id;
    s32 format_string_id;

    switch (unknown_0x2fa0c) {
    case 0:
        if ((MemoryStick::objectPtr->flag_0x970 == 0) && (MemoryStick::objectPtr->flag_0x971 == 0) && (unknown_0x2fa08 != 0)) {
            GameSys::objectPtr->flag_0x6ADDE = 0;
            if (unknown_0x2fa04 != 0) {
                switch (unknown_0x2fa08) {
                case 1:
                    format_string_argument_id = 0x41;
                    format_string_id = 0x40;
                    break;
                case 2:
                    format_string_argument_id = 0x42;
                    format_string_id = 0x40;
                    break;
                case 3:
                    format_string_argument_id = 0x43;
                    format_string_id = 0x40;
                    break;
                case 4:
                    format_string_argument_id = 0;
                    format_string_id = 0x40;
                    break;
                case 5:
                    format_string_argument_id = 0x45;
                    format_string_id = 0x44;
                    break;
                case 6:
                    format_string_argument_id = 0x46;
                    format_string_id = 0x44;
                    break;
                case 7:
                    format_string_argument_id = 0x47;
                    format_string_id = 0x44;
                    break;
                case 8:
                    format_string_argument_id = 0;
                    format_string_id = 0x44;
                    break;
                case 9:
                    format_string_argument_id = 0x45;
                    format_string_id = 0x48;
                    break;
                case 10:
                    format_string_argument_id = 0x46;
                    format_string_id = 0x48;
                    break;
                case 11:
                    format_string_argument_id = 0x47;
                    format_string_id = 0x48;
                    break;
                case 12:
                    format_string_argument_id = 0;
                    format_string_id = 0x48;
                    break;
                }
                if ((GameSys::objectPtr->flag_0x480 != 0) && (format_string_id == 0x44)) {
                    format_string_id = 0x65;
                }
                if (format_string_argument_id != 0) {
                    sprintf(buffer, func_eboot_088515A0(GameSys::objectPtr, format_string_id & 0xFFFF), func_eboot_088515A0(GameSys::objectPtr, format_string_argument_id & 0xFFFF));
                } else {
                    sprintf(buffer, func_eboot_088515A0(GameSys::objectPtr, format_string_id & 0xFFFF), "");
                }
                func_eboot_088C28C0(MemoryStick::objectPtr, buffer);
                Pad::objectPtr->flag_0x17 = true;
                unknown_0x2fa0c += 1;
            }
        }
        break;
    case 1:
        if (MemoryStick::objectPtr->flag_0x971 == 0) {
            unknown_0x2fa0c = 0;
            unknown_0x2fa08 = 0;
            Pad::objectPtr->flag_0x17 = false;
        }
        break;
    }
    draw_loading_screen_impl();
}

void FileSys::vtable_0x48() {
    // empty
}

char install_block_path_format[] = "ms0:\\.\\PSP\\SAVEDATA\\ULJM05500DAT\\%08d";
INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", fake_rofs_loader);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884E3BC);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", sha1_thread);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", transfer_thread);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884E670);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884E6CC);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884E740);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884E79C);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884E7C8);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884E8DC);

void FileSys::decrypt_buffer(u8 *data, s32 size, s32 prevSize) {
    u8 *var_s4;
    u8 *var_s3;
    u8 *var_s2;
    u8 *var_s1;
    s32 var_s0;

    var_s4 = data;
    var_s3 = data + 1;
    var_s2 = data + 2;
    var_s1 = data + 3;
    var_s0 = 0;
    if (size > 0) {
        do {
            *var_s4 = *(decrypt_table + *var_s4);
            *var_s3 = *(decrypt_table + *var_s3);
            *var_s2 = *(decrypt_table + *var_s2);
            *var_s1 = *(decrypt_table + *var_s1);


            *(s32*)var_s4 ^= next_decryption_key();
            var_s3 += 4;
            var_s2 += 4;
            var_s4 += 4;
            var_s1 += 4;
            var_s0 += 4;
        } while (var_s0 < size);
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884EA44);

void FileSys::set_decryption_key(u32 key) {
    key_lower = key & 0xffff;
    key_upper = (key >> 0x10) & 0xffff;
    if (key_lower == 0) {
        key_lower = 0x7f8d;
    }
    if (key_upper == 0) {
        key_upper = 0x2345;
    }
}

u32 FileSys::next_decryption_key() {
    u32 next_upper;
    key_lower = (key_lower * 0x7f8d) % 0xfff1;
    key_upper = (next_upper = (key_upper * 0x2345) % 0xffd9);
    return (next_upper << 0x10) + key_lower;
}

u32 FileSys::file_size(u32 file_id) {
    s32 temp_v1 = file_id & 0xffff;
    s32 var_a3;
    FileSys *var_a2;

    if (temp_v1 == 0xffff) {
        return 0;
    }

    var_a3 = 0;
    var_a2 = this;
loop_5:
    if (temp_v1 == var_a2->file_size_pairs[0].file_id) {
        return file_size_pairs[var_a3].size;
    }
    var_a3++;
    var_a2 = (FileSys*)((u8*)var_a2 + sizeof(file_size_pair));
    if (var_a3 >= 0x32C) {
        return (file_id_to_first_block[temp_v1 + 1] - file_id_to_first_block[(u16)file_id]) * 0x800;
    }
    goto loop_5;
}

u32 FileSys::file_blocks_size(u32 file_id) {
  u32 uVar1;
  u32 uVar2;

  uVar2 = file_id & 0xffff;
  if (uVar2 == 0xffff) {
    uVar1 = 0;
  }
  else {
    uVar1 = (file_id_to_first_block[uVar2 + 1] - file_id_to_first_block[uVar2]) * 0x800;
  }
  return uVar1;
}

inline static u32 inline_fn(u32 arg0) {
    return arg0;
}

u32 FileSys::load_file_blocking(s32 arg1, u8 *arg2, u32 arg3) {
    s32 temp_s0;
    s32 temp_s5;
    u32 var_s1;

    u8 *new_var2;
    u32 new_var3;

    sceKernelWaitSema(fake_rofs_semaphore, 1, 0);
    temp_s0 = file_id_to_first_block[arg1 & 0xFFFF] << 0xB;
    temp_s5 = ((u8)temp_s0) >> 0x1F;
loop_1:
    if (sceIoLseek(file_descriptor, temp_s0, temp_s5) != inline_fn(temp_s0)) {
        sceKernelDelayThreadCB(0x2710U);
        goto loop_1;
    }
    var_s1 = file_size(arg1);
    if (arg3 != -1U) {
        var_s1 = arg3;
    }
    new_var2 = arg2;
    new_var3 = var_s1;
loop_7:
    if (sceIoRead(file_descriptor, new_var2, new_var3) != inline_fn(new_var3)) {
        sceKernelDelayThreadCB(0x2710U);
        goto loop_7;
    }
    var_s1 = temp_s0 >> 0xB;
    if (temp_s0 < 0) {
        var_s1 = (temp_s0 + 0x7FF) >> 0xB;
    }
    set_decryption_key(var_s1);
    decrypt_buffer(new_var2, new_var3, 0);
    sceKernelSignalSema(fake_rofs_semaphore, 1);
    return new_var3;
}

int FileSys::load_file_async(s32 file_id, u8* buf, SceSize len, u8 unknown_flag, u32 *cancellation_ptr, u8 wakeup_loader_thread) {
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
        var_s3 = file_size(file_id);
    }
    sp34 = var_s3;
    var_s2 = 0;
    var_s1 = 1;
    var_s0 = 0;
    if ((is_data_file_encrypted != 0) && (((s32) (System::objectPtr->next_index(1) & 0xFFFF) % 100) < 5)) {
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

void FileSys::movie_open(u16 arg1) {
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_v0;
    s32 var_a1;
    s32 *temp_s1;
    s32 temp_a0;
    s32 temp_v1;

    is_playing_movie = 1;
    temp_v0 = arg1 & 0xFFFF;
    movie_file_id = (u16) arg1;
    temp_s1 = temp_v0 + (s32*)&file_id_to_first_block;
    temp_s2 = file_id_to_first_block[temp_v0];
    temp_s3 = temp_s2 << 0xB;
    temp_s2 = 0;
loop_1:
    temp_v0 = sceIoLseek(file_descriptor, (long long)temp_s3, temp_s2);
    temp_v0 = (s32) temp_v0;
    if ((u32)temp_v0 != temp_s3) {
        sceKernelDelayThreadCB(0x2710U);
        goto loop_1;
    }
    temp_a0 = *temp_s1;
    temp_v1 = temp_a0 * 0x800;
    movie_open_pos = temp_v1;
    var_a1 = (temp_a0 << 0xB) >> 0xB;
    if ((s32)(temp_v1) < 0) {
        var_a1 = (temp_v1 + 0x7FF) >> 0xB;
    }
    set_decryption_key(var_a1);
}

void FileSys::movie_read(u8* buf, SceSize len) {
    u32 uVar1;
    u8 *new_var;

    uVar1 = sceUmdGetDriveStat() & 0x20;
    while ((uVar1 > 0) ^ 1)
    {
        sceKernelDelayThreadCB(10000);
        uVar1 = sceUmdGetDriveStat() & 0x20;
    }

    new_var = buf;
loop_2:
    uVar1 = sceIoRead(file_descriptor, new_var, len);
    if (uVar1 != len)
    {
        sceKernelDelayThreadCB(10000);
        goto loop_2;
    }

    movie_open_pos += len;
}

void FileSys::movie_seek(int arg1) {
    s32 new_var;
    s32 temp_s1;
    s32 temp_s2;

    new_var = arg1;
    temp_s1 = file_id_to_first_block[movie_file_id] << 0xB;
    temp_s2 = new_var + temp_s1;
    new_var = temp_s2;
    temp_s1 = (u16)new_var >> 0x1F;
loop_1:
    if (sceIoLseek(file_descriptor, new_var, temp_s1) == inline_fn(new_var)) {
        movie_open_pos = new_var;
        return;
    }

    sceKernelDelayThreadCB(0x2710U);
    goto loop_1;
}

void FileSys::movie_close() {
    is_playing_movie = 0;
    movie_pos = 0;
}

SceUID FileSys::load_libfont(u32 file_id, SceUID param_3) {
    SceUID fid;
    u32 first_block;
    SceUID modid;
    long long status;
    SceKernelLMOption opt;

    memset(&opt, 0, 0x14);
    opt.size = 0x14;
    opt.access = 1;
    opt.mpidtext = param_3;
    opt.mpiddata = param_3;
    opt.position = 0;
    first_block = file_id_to_first_block[file_id & 0xffff];
    sprintf(libfont_path, "disc0:/sce_lbn0x%x_size0x%x", data_bin_first_sector + first_block, file_size(file_id));
loop_1:
    fid = sceIoOpen(libfont_path, 1, 0);
    if (fid < 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_1;
    }
loop_2:
modid = sceKernelLoadModuleByID(fid, 0, &opt);
    if (modid < 0) {
        sceKernelDelayThreadCB(10000);
        goto loop_2;
    }
    sceKernelStartModule(modid, 0, NULL, ((int*)&status+2), NULL);
    sceIoClose(fid);
    return modid;
}


int FileSys::file_has_sha1(u32 file_id) {
    static u8 null_sha1_digest[20];
    int iVar1;
    iVar1 = memcmp(file_sha1_digests[file_id & 0xffff], &null_sha1_digest, 20);
    return iVar1 != 0;
}

void FileSys::calculate_install_block_offsets() {
  u32 s16;
  s32 iVar1;
  u32 wVar2;
  u32 wVar3;
  u32 uVar5;
  install_block_offset *pdVar5;
  u32 uVar4;

  wVar3 = 0;
  uVar5 = 0;
  pdVar5 = file_id_to_install_block_offset;
  for (uVar4 = 0; uVar4 < 0x19ca; uVar4++) {
    s16 = file_has_sha1(uVar4 & 0xffff);
    if (s16 != 0) {
      iVar1 = file_blocks_size((u16)uVar4);
      if (uVar5 + iVar1 > 0x370000) {
        wVar3 = wVar3 + 1;
        uVar5 = 0;
        wVar3 = (u16) wVar3;
      }
      wVar2 = (uVar5 >> 0xb);
      pdVar5->block = wVar3;
      pdVar5->offset = wVar2;
      uVar5 = uVar5 + iVar1;
    } else {
      pdVar5->block = 0xffff;
      pdVar5->offset = 0;
    }
    pdVar5 = pdVar5 + 1;
  }
}

bool FileSys::is_loading() {
    if (unknown_0x2fa04 != 0 && unknown_0x2fa08 != 0) {
        return true;
    }
    return FileSysBase::is_loading();
}

bool FileSys::is_loading(u16 file_id) {
    if (unknown_0x2fa04 != 0 && unknown_0x2fa08 != 0) {
        return true;
    }
    return FileSysBase::is_loading(file_id);
}

bool FileSys::is_loading(u8 unknown_flag) {
    if (unknown_0x2fa04 != 0 && unknown_0x2fa08 != 0) {
        return true;
    }
    return FileSysBase::is_loading(unknown_flag);
}


char install_folder_path[] = "ms0:\\.\\PSP\\SAVEDATA\\ULJM05500DAT";
INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884F538);

INCLUDE_ASM("asm/eboot/nonmatchings/file_sys", func_eboot_0884F5B4);

const char* FileSys::data_bin_path() {
    return "disc0:/PSP_GAME/USRDIR/DATA.BIN";
}

FileSys *FileSys::objectPtr;
