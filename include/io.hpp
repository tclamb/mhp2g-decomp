#ifndef MHP2G_DECOMP__IO_HPP
#define MHP2G_DECOMP__IO_HPP

#include "common.h"

#include <pspge.h>
#include <pspthreadman.h>
#include <pspthreadman_kernel.h>
#include <pspmodulemgr.h>
#include <pspthreadman_kernel.h>
#include <pspumd.h>
#include <pspiofilemgr_stat.h>

#include "file_sys.hpp"

extern "C" {
    //#include <pspiofilemgr.h> # sceIoLseek needs to take 64-bit offset for byte-matching
    int sceIoLseek(SceUID fd, unsigned long long offset, int whence);
    int sceIoRead(SceUID fd, void *data, SceSize size);
    SceUID sceIoOpen(const char *file, int flags, SceMode mode);
    int sceIoClose(SceUID fd);
    int sceIoGetstat(const char *file, SceIoStat *stat);

    int memcmp(const void*, const void*, long unsigned int);
    void *memset(void*, int,  long unsigned int);
    int sprintf(char*, const char*, ...);
}

#endif
