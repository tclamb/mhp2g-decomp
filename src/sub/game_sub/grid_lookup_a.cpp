#include "common.h"

extern "C" u32 func_game_sub_09C36610(void *object, void *position) {
    float x = *(float *)((u8 *)position + 0);
    float x_scale = *(float *)((u8 *)object + 0x218);
    float z = *(float *)((u8 *)position + 8);
    float z_scale = *(float *)((u8 *)object + 0x21C);
    u8 *grid = *(u8 **)((u8 *)object + 0x210);
    int width = *(int *)(grid + 0xC);
    u32 *cells = *(u32 **)(grid + 0x18);
    int x_index = (int)(x * x_scale);
    int z_index = (int)(z * z_scale);
    return cells[x_index * width + z_index];
}
