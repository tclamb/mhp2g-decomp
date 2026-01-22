#include "io.hpp"

u32 data_loader::next_decryption_key() {
    u32 next_upper;
    key_lower = (key_lower * 0x7f8d) % 0xfff1;
    key_upper = (next_upper = (key_upper * 0x2345) % 0xffd9);
    return (next_upper << 0x10) + key_lower;
}
