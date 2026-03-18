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

#include "common.h"

struct file_size_pair {
    u32 file_id;
    u32 size;
};

#include "../src/eboot/file_sys_impl.decrypt_table.inc.cpp"

struct FileSysImpl {
    std::ifstream &in;
    file_size_pair file_size_pairs[812];
    u32 file_id_to_first_block[6603];
    u32 key_lower;
    u32 key_upper;

    FileSysImpl(std::ifstream &in) : in(in) {}

    void initialize();
    void decrypt_buffer(u8 *, s32, s32);
    void set_decryption_key(u32);
    u32 load_file_blocking(s32 arg1, u8 *arg2, u32 arg3);
    u32 next_decryption_key();
    u32 file_size(u32 file_id);
};

void FileSysImpl::initialize() {
    in.read((char*)file_id_to_first_block, 0x672C);
    set_decryption_key(0);
    decrypt_buffer((u8*)file_id_to_first_block, 0x672C, 0);

    in.read((char*)file_size_pairs, 0x1960);
    decrypt_buffer((u8*)file_size_pairs, 0x1960, 0x672c);
}

void FileSysImpl::decrypt_buffer(u8 *data, s32 size, s32 prevSize) {
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

void FileSysImpl::set_decryption_key(u32 key) {
    key_lower = key & 0xffff;
    key_upper = (key >> 0x10) & 0xffff;
    if (key_lower == 0) {
        key_lower = 0x7f8d;
    }
    if (key_upper == 0) {
        key_upper = 0x2345;
    }
}

u32 FileSysImpl::next_decryption_key() {
    u32 next_upper;
    key_lower = (key_lower * 0x7f8d) % 0xfff1;
    key_upper = (next_upper = (key_upper * 0x2345) % 0xffd9);
    return (next_upper << 0x10) + key_lower;
}

u32 FileSysImpl::file_size(u32 file_id) {
    s32 temp_v1 = file_id & 0xffff;
    s32 var_a3;
    FileSysImpl *var_a2;

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
    var_a2 = (FileSysImpl*)((u8*)var_a2 + sizeof(file_size_pair));
    if (var_a3 >= 0x32C) {
        return (file_id_to_first_block[temp_v1 + 1] - file_id_to_first_block[(u16)file_id]) * 0x800;
    }
    goto loop_5;
}

u32 FileSysImpl::load_file_blocking(s32 arg1, u8 *arg2, u32 arg3) {
    s32 temp_s0;
    s32 temp_s5;
    u32 var_s1;

    u8 *new_var2;
    u32 new_var3;

    temp_s0 = file_id_to_first_block[arg1 & 0xFFFF] << 0xB;
    temp_s5 = ((u8)temp_s0) >> 0x1F;

    in.seekg(temp_s0, std::ios_base::beg);

    var_s1 = file_size(arg1);
    if (arg3 != -1U) {
        var_s1 = arg3;
    }
    new_var2 = arg2;
    new_var3 = var_s1;

    in.read((char*)arg2, new_var3);

    var_s1 = temp_s0 >> 0xB;
    if (temp_s0 < 0) {
        var_s1 = (temp_s0 + 0x7FF) >> 0xB;
    }
    set_decryption_key(var_s1);
    decrypt_buffer(new_var2, new_var3, 0);

    return new_var3;
}


int main(int argc, char** argv) {
    if (argc != 3 && argc != 4) {
        std::cerr << "incorrect number of args\n\nusage: data-extractor data.bin id [out]\n\n";
        return -1;
    }
    std::string data_bin_path{argv[1]};

    short file_id;
    std::stringstream{argv[2]} >> file_id;

    std::ostream *out;
    if (argc == 4) {
        out = new std::ofstream{argv[3], std::ios_base::binary};
    } else {
        out = &std::cout;
    }

    auto in = std::ifstream{data_bin_path, std::ios_base::in | std::ios_base::binary};
    auto loader = FileSysImpl{in};
    loader.initialize();
    u32 len = loader.file_size(file_id);
    auto buf = std::vector<u8>(len);
    loader.load_file_blocking(file_id, buf.data(), len);
    out->write((char*)buf.data(), len);
    out->flush();

    return 0;
}
