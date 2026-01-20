#include "io.hpp"

void data_loader::decrypt_buffer(u8 *data, s32 size) volatile {
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