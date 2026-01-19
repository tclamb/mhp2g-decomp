#include "common.h"

extern "C" u32 data_loader__next_decryption_key(volatile data_loader *self) {
    u32 next_upper;
    self->key_lower = (self->key_lower * 0x7f8d) % 0xfff1;
    self->key_upper = (next_upper = (self->key_upper * 0x2345) % 0xffd9);
    return (next_upper << 0x10) + self->key_lower;
}
