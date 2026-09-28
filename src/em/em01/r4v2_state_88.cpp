// Original two call lifecycle callback.
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
extern "C" void func_game_sub_09C848D8(u8 *, int, int);
extern "C" void func_game_sub_09C52B90(u8 *, int);

extern "C" void func_em01_09D15188(u8 *actor) {
    func_game_sub_09C848D8(actor, 34, 0);
    func_game_sub_09C52B90(actor, 0);
}
