// Original byte compare callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;

extern "C" bool func_em15_09D22A30(const u8 *actor, u8 value) {
    return actor[0x7E1] != value;
}
