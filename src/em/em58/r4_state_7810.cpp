// Original state save timer callback.
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;

extern "C" void func_em58_09D1C910(u8 *actor, u8 *record) {
    record[0x1F] = actor[0x790];
    record[0x1E] = ((s8 *)actor)[0x791];
    *(s16 *)(actor + 0x792) = 150;
}
