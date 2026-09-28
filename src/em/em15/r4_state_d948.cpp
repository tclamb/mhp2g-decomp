// Original state save callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;

extern "C" void func_em15_09D22A48(u8 *actor, u8 *record) {
    record[0x1F] = actor[0x7E0];
    record[0x1E] = actor[0x7E1];
}
