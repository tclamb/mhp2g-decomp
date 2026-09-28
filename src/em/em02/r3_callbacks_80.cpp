// Monster lifecycle and state callbacks.

// Clears actor byte at 0x800.
extern "C" void func_em02_09D15180(unsigned char *actor) { actor[0x800] = 0; }
