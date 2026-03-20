#include "file_sys_base.hpp"

#include "pac.hpp"

#include <pspge.h>
#include <pspthreadman.h>
#include <pspmodulemgr.h>

void FileSysBase::initialize() {
    ge_edram_start = sceGeEdramGetAddr();
    ge_edram_end = (void*)((u32)ge_edram_start + sceGeEdramGetSize());
    flag_0x29208 = 0;
    fake_rofs_semaphore = sceKernelCreateSema("fakeRofsSema", 0x100, 1, 1, NULL);
}

void FileSysBase::clear() {
    load_request *req = load_request_ringbuf;
    do {
        req++->state = 0;
    } while (req < load_request_ringbuf + sizeof(load_request_ringbuf)/sizeof(load_request));
    load_request_load_head = 0;
    load_request_write_head = 0;
    load_thread_status = 0;
}

void FileSysBase::complete_one() {
    load_request_ringbuf[load_request_load_head].state = LOAD_REQUEST_COMPLETE;
    advance(load_request_load_head);
    load_thread_status = 0;
}

bool FileSysBase::is_loading() {
    return load_request_ringbuf[load_request_load_head].state != LOAD_REQUEST_COMPLETE;
}

bool FileSysBase::is_loading(u16 file_id) {
    load_request *r = &load_request_ringbuf[0];
    for (int i = 0; i < 0x80; ++i, ++r) {
        if (r->state == LOAD_REQUEST_INCOMPLETE && r->file_id == file_id) {
            return true;
        }
    }
    return false;
}

bool FileSysBase::is_loading(u8 unknown_flag) {
    load_request *r = &load_request_ringbuf[0];
    for (int i = 0; i < 0x80; ++i, ++r) {
        if (r->state == LOAD_REQUEST_INCOMPLETE && r->unknown_flag == unknown_flag) {
            return true;
        }
    }
    return false;
}

void FileSysBase::unload_module(SceUID module) {
    if (module > 0) {
        int status;
        sceKernelStopModule(module, 0, 0, &status, 0);
        sceKernelUnloadModule(module);
    }
}

void FileSysBase::cancel_all() {
    int state = sceKernelSuspendDispatchThread();

    load_request *r = &load_request_ringbuf[0];
    for (int i = 0; i < 0x80; ++i, ++r) {
        if (i != load_request_load_head) {
            r->state = LOAD_REQUEST_COMPLETE;
        }
        if (r->is_cancelled_ptr) {
            *r->is_cancelled_ptr = true;
        }
    }

    if (load_request_ringbuf[load_request_load_head].state == LOAD_REQUEST_COMPLETE) {
        load_request_write_head = load_request_load_head;
    } else {
        load_request_write_head = (load_request_write_head = load_request_load_head + 1) & 0x7F;
    }

    sceKernelResumeDispatchThread(state);
}

void FileSysBase::update_ringbuf(u8 unknown_flag) {
    int state = sceKernelSuspendDispatchThread();

    load_request *r = &load_request_ringbuf[load_request_load_head];
    if (r->state != LOAD_REQUEST_COMPLETE) {
        if (r->unknown_flag == (u8)unknown_flag) {
            if (r->is_cancelled_ptr) {
                *r->is_cancelled_ptr = true;
            }
        }

        u32 i = next(load_request_load_head);
        for (int j = 0; j < 0x7F; ++j) {
            r = &load_request_ringbuf[i];
            if (r->state == LOAD_REQUEST_COMPLETE) {
                break;
            }
            if (r->unknown_flag == (u8)unknown_flag) {
                if (r->is_cancelled_ptr) {
                    *r->is_cancelled_ptr = true;
                }
                cancel(i);
            } else {
                i = next(i);
            }
        }

        load_request_write_head = load_request_load_head;
        for (int j = 0; j < 0x7F; ++j) {
            if (load_request_ringbuf[load_request_write_head].state == LOAD_REQUEST_COMPLETE) {
                break;
            }
            load_request_write_head = next(load_request_write_head);
        }
    }

    sceKernelResumeDispatchThread(state);
}

void FileSysBase::cancel(int i) {
    for (int j = next(i); j != load_request_load_head; i = next(i), j = next(j)) {
        load_request &cur = load_request_ringbuf[j];
        load_request &prev = load_request_ringbuf[i];
        prev.state            = cur.state;
        prev.file_id          = cur.file_id;
        prev.buf              = cur.buf;
        prev.block_len        = cur.block_len;
        prev.block_offset     = cur.block_offset;
        prev.file_len         = cur.file_len;
        prev.is_cancelled_ptr = cur.is_cancelled_ptr;
        prev.unknown_flag     = cur.unknown_flag;
        prev.needs_seed       = cur.needs_seed;
        prev.is_final_block   = cur.is_final_block;
        prev.needs_decryption = cur.needs_decryption;
        prev.needs_hashing    = cur.needs_hashing;
    }
    load_request_ringbuf[i].state = LOAD_REQUEST_COMPLETE;
}

s32 pac_header::count() {
    return block_count;
}

u8 *pac_header::data(int index) {
    if (index >= count()) {
        return 0;
    }
    // u32 offset = block_headers[index].offset;
    u32 offset = *(u32*)(((u8*)this + 8 * index) + 4);
    if (offset != 0) {
        return (u8*)this + block_headers[index].offset;
    }
    return 0;
}

u32 pac_header::size(int index) {
    if (index >= count()) {
        return -1;
    }
    // return block_headers[index].size;
    return *(u32*)(((u8*)this + 8 * index) + 8);
}

// render loading screen
INCLUDE_ASM("asm/eboot/nonmatchings/file_sys_base", draw_loading_screen_impl__11FileSysBaseFv);


void FileSysBase::set_flags(int flag, u32 argument) {\
    if (flag_0x2920C == 0) {
        flag_0x29210 = 0;
        flag_0x2920C = 1;
    }
    switch (flag) {
    default:
    case 0:
        break;
    case 1:
        flag_0x29208 |= 1;
        break;
    case 2:
        flag_0x29214 = argument;
        flag_0x29208 |= 2;
        break;
    case 3:
        flag_0x29208 |= 4;
        break;
    }
}

void FileSysBase::clear_flags() {
    flag_0x2920C = 0;
    flag_0x29208 = 0;
}

// NOW LOADING
u16 D_eboot_089AFB2C[] = {  0, 24,  48,  -2,
                           72, 24,  96, 120,
                          144,  0, 168,  -1 };

// LADEN…
u16 D_eboot_089AFB44[] = { 72,  96, 120, 216,
                            0, 360,  -1 };

// CHARGEMENT
u16 D_eboot_089AFB54[] = { 192, 240,  96, 312,
                           168, 216, 264, 216,
                             0, 336,  -1 };

// CARGANDO
u16 D_eboot_089AFB6C[] = { 192, 96,  312, 168,
                            96,  0,  120,  24,
                            -1 };

// CARICAMENTO
u16 D_eboot_089AFB80[] = { 192,    96,   312, 144,
                           192,    96,   264, 216,
                             0,   336,    24,  -1 };

// atlas indices for each language id
u16 *D_eboot_089AFB98[] = { D_eboot_089AFB2C,
                            D_eboot_089AFB2C,
                            D_eboot_089AFB44,
                            D_eboot_089AFB54,
                            D_eboot_089AFB6C,
                            D_eboot_089AFB80,
                            D_eboot_089AFB2C };


// angular frequency for each language id
u16 D_eboot_089AFBB4[] = {194, 194, 314, 218, 266, 194, 194};

// Sin[i * \pi / 6] truncated to thousands place
float D_eboot_089AFBC4[] = {
    0, 0.5, 0.866, 1, 0.866, 0.5, 0, -0.5, -0.866, -1, -0.866, -0.5
};

// frame buffers
u16 D_eboot_089AFBF4[] = {0x80, 0x00};
