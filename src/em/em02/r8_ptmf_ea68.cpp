// Install the original pointer-to-member callback when valid.
typedef unsigned char u8;
struct Ptmf { float word[3]; };
extern "C" int __ptmf_test(Ptmf *);
extern "C" float D_em02_09D276F0[2];
extern "C" float D_em02_09D276F8;

extern "C" void func_em02_09D23B68(u8 *actor) {
    actor[0x14] = 0;
    Ptmf pm;
    pm.word[0] = D_em02_09D276F0[0];
    pm.word[1] = D_em02_09D276F0[1];
    pm.word[2] = D_em02_09D276F8;
    if (__ptmf_test(&pm) != 0) {
        *(Ptmf *)(actor + 0x78) = pm;
    }
}
