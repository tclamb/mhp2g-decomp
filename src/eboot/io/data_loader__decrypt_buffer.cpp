#include "io.hpp"

extern "C" u8 data_loader__decrypt_table[];
extern "C" u32 data_loader__next_decryption_key(volatile data_loader *);

extern "C" void data_loader__decrypt_buffer(volatile data_loader *self, u8 *data, s32 size) {
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
            *var_s4 = *(data_loader__decrypt_table + *var_s4);
            *var_s3 = *(data_loader__decrypt_table + *var_s3);
            *var_s2 = *(data_loader__decrypt_table + *var_s2);
            *var_s1 = *(data_loader__decrypt_table + *var_s1);
        
        
            *(s32*)var_s4 ^= data_loader__next_decryption_key(self);
            var_s3 += 4;
            var_s2 += 4;
            var_s4 += 4;
            var_s1 += 4;
            var_s0 += 4;
        } while (var_s0 < size);
    }
}