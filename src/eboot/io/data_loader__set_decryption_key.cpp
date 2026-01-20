#include "io.hpp"

void data_loader::set_decryption_key(u32 key) volatile {
    key_lower = key & 0xffff;
    key_upper = (key >> 0x10) & 0xffff;
    if (key_lower == 0) {
        key_lower = 0x7f8d;
    }
    if (key_upper == 0) {
        key_upper = 0x2345;
    }
}
