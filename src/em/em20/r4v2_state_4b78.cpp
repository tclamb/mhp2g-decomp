// Original increment then tail callback.
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
extern "C" void func_em20_09D17550(u8 *);

extern "C" void func_em20_09D19C78(u8 *actor) {
    ++actor[0x1D5];
    func_em20_09D17550(actor);
}
