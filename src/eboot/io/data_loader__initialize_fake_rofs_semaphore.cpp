#include "io.hpp"

#include <pspge.h>
#include <pspthreadman.h>

void data_loader::initialize_fake_rofs_semaphore() {
    ge_edram_start = sceGeEdramGetAddr();
    ge_edram_end = (void*)((u32)ge_edram_start + sceGeEdramGetSize());
    unknown_flags = 0;
    fake_rofs_semaphore = sceKernelCreateSema(fake_rofs_semaphore_name, 0x100, 1, 1, NULL);
}
