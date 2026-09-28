// Original conditional byte store callback.
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;

extern "C" void func_em07_09D1C5B8(u8 *actor, u8 value) {
    u8 *record = actor + 0x790;
    if (actor[0x6DB]) *record = value;
}
