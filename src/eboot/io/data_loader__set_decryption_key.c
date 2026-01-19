#include "common.h"

extern "C" void data_loader__set_decryption_key(volatile data_loader *self, u32 key) {
    self->key_lower = key & 0xffff;
    self->key_upper = (key >> 0x10) & 0xffff;
    if (self->key_lower == 0) {
        self->key_lower = 0x7f8d;
    }
    if (self->key_upper == 0) {
        self->key_upper = 0x2345;
    }
}
