// Original state restore callback.
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;

extern "C" void func_em15_09D22AA8(u8 *actor, u8 *record) {
    actor[0x7E0] = record[0x1F];
    actor[0x7E1] = ((s8 *)record)[0x1E];
}
