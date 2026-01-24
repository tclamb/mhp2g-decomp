#include "io.hpp"

SceUID data_loader::load_libfont(u32 file_id, SceUID param_3) {
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
    sprintf(libfont_path, lba_umd_access_path_format, data_bin_first_sector + first_block, file_blocks_size(file_id));
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