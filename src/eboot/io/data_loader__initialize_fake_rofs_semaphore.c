#include "common.h"

#include <pspge.h>
#include <pspthreadman.h>

extern char data_loader__fake_rofs_semaphore_name[];

void data_loader__initialize_fake_rofs_semaphore(volatile data_loader *self) {
    self->ge_edram_start = sceGeEdramGetAddr();
    self->ge_edram_end = (void*)((u32)self->ge_edram_start + sceGeEdramGetSize());
    self->unknown_flags = 0;
    self->fake_rofs_semaphore = sceKernelCreateSema(data_loader__fake_rofs_semaphore_name, 0x100, 1, 1, NULL);
}
