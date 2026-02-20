#include <fstream>
#include <memory>
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory.h>
#include <stdio.h>

#define MHP2G_DECOMP__GAME_HPP
#define MHP2G_DECOMP__IO_HPP

#include "common.h"

#include <pspge.h>
#include <pspthreadman.h>
#include <pspthreadman_kernel.h>
#include <pspmodulemgr.h>
#include <pspthreadman_kernel.h>
#include <pspumd.h>
#include <pspiofilemgr_stat.h>
#include <pspiofilemgr.h>

struct game {
    static game *instance;
    SceUID sha1_thread_id;
    u8 unknown_flag;
};
game impl;
game *game::instance = &impl;

struct file_size_pair {
    u32 file_id;
    u32 size;
};

struct data_loader
{
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
    static char null_utility_message_dialog_header[];
    static char install_block_path_format[];
    static char lba_umd_access_path_format[];
    static char install_folder_path[];

    char *data_bin_path() { return nullptr; }
    void calculate_install_block_offsets() {}
    void initialize_load_request_queue() {}
    void initialize_fake_rofs_semaphore() {}

    u32 file_blocks_size(u32);
    u32 file_size(u32);
    SceSize load_file_blocking(s32, u8*, SceSize);
    void start_threads();
    u32 next_decryption_key();
    void set_decryption_key(u32);
    void decrypt_buffer(u8*, s32, s32);

    SceUID file_descriptor;
    u8 unused_flag_0x1014;
    u32 data_bin_first_sector;
    u8 is_playing_movie;
    file_size_pair file_size_pairs[812];
    u32 file_id_to_first_block[6603];
    SceUID fake_rofs_semaphore;
    SceUID loader_thread_id;
    SceUID sha1_event_flag_id;
    SceUID sha1_thread_id;
    u8 is_data_file_encrypted;
    u32 key_lower;
    u32 key_upper;
    u8 unknown_0x2fa04;
    u32 unknown_0x2fa08;
    u32 unknown_0x2fa0c;
    SceUID transfer_event_flag_id;
    SceUID transfer_thread_id;
};

extern "C" int fake_rofs_loader(SceSize, void*) { return 0; }
extern "C" int sha1_thread(SceSize, void*) { return 0; }
extern "C" int transfer_thread(SceSize, void*) { return 0; }

char loader_thread_name[] = {};
char transfer_thread_name[] = {};
char transfer_event_flag_name[] = {};
char sha1_thread_name[] = {};
char sha1_event_flag_name[] = {};
char umd_disc_drive_name[] = {};

#include "../src/eboot/io/data_loader__start_threads.cpp"
#include "../src/eboot/io/data_loader__next_decryption_key.cpp"
#include "../src/eboot/io/data_loader__file_blocks_size.cpp"
#include "../src/eboot/io/data_loader__file_size.cpp"
#include "../src/eboot/io/data_loader__decrypt_buffer.cpp"
#include "../src/eboot/io/data_loader__set_decryption_key.cpp"
#include "../src/eboot/io/data_loader__load_file_blocking.cpp"

class SceMock
{
private:
    SceUID next_uid = 3;
    std::unordered_map<SceUID, std::fstream> open_files;
    std::string data_bin_path = "./data.bin";

    SceMock() = default;

public:
    static SceMock *get() {
        static SceMock INSTANCE;
        return &INSTANCE;
    }

    int open_data_bin() {
        auto fd = next_uid++;
        open_files.emplace(fd,  std::fstream{data_bin_path, std::ios_base::in | std::ios_base::binary});
        return fd;
    }

    int read_data_bin(SceUID fd, void *buf, SceSize len) {
        open_files.find(fd)->second.read((char *)buf, len);
        return len;
    }

    int setpos_data_bin(SceUID fd, unsigned long long offset) {
        open_files.find(fd)->second.seekg(offset, std::ios_base::beg);
        return offset;
    }

    void close_data_bin(SceUID fd) {
        open_files.erase(fd);
    }

    void set_data_bin_path(std::string_view path) {
        data_bin_path = path;
    }
};

int sceIoOpen(const char *file, int flags, SceMode mode) {
    return SceMock::get()->open_data_bin();

}

int sceIoRead(SceUID fd, void *buf, SceSize len) {
    return SceMock::get()->read_data_bin(fd, buf, len);
}

SceOff sceIoLseek(SceUID fd, SceOff offset, int whence) {
    return SceMock::get()->setpos_data_bin(fd, offset);
}

int sceIoClose(SceUID fd) {
    SceMock::get()->close_data_bin(fd);
    return 0;
}

int sceUmdGetDriveStat(void) {
    return 0x20;
}

int sceIoGetstat(const char *file, SceIoStat *stat) { return 0; }
SceUID sceKernelCreateEventFlag(const char *name, int attr, int bits, SceKernelEventFlagOptParam *opt) { return 0; }
SceUID sceKernelCreateThread(const char *name, SceKernelThreadEntry entry, int initPriority, int stackSize, SceUInt attr, SceKernelThreadOptParam *option) { return 0; }
void sceKernelDcacheWritebackRange(const void *p, unsigned int size) { return; }
void sceKernelDcacheInvalidateRange(const void *p, unsigned int size) {}
int sceKernelDelayThreadCB(SceUInt delay) { return 0; }
int sceKernelSignalSema(SceUID semaid, int signal) { return 0; }
int sceKernelStartThread(SceUID thid, SceSize arglen, void *argp) { return 0; }
int sceKernelWaitSema(SceUID semaid, int signal, SceUInt *timeout) { return 0; }
int sceUmdActivate(int unit, const char *drive) { return 0; }
int sceUmdGetErrorStat(void) { return 0; }

int main(int argc, char** argv) {
    if (argc != 3 && argc != 4) {
        std::cerr << "incorrect number of args\n\nusage: data-extractor data.bin id [out]\n\n";
        return -1;
    }
    SceMock::get()->set_data_bin_path(argv[1]);

    short file_id;
    std::stringstream{argv[2]} >> file_id;

    std::ostream *out;
    if (argc == 4) {
        out = new std::ofstream{argv[3], std::ios_base::binary};
    } else {
        out = &std::cout;
    }

    data_loader loader;
    loader.start_threads();

    u32 len = loader.file_size(file_id);
    auto buf = std::vector<u8>(len);
    loader.load_file_blocking(file_id, buf.data(), len);
    out->write((char*)buf.data(), len);
    out->flush();

    return 0;
}
