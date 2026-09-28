// Original byte compare callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;

extern "C" bool func_em07_09D1C5D0(const u8 *actor, u8 value) {
    return actor[0x790] != value;
}
