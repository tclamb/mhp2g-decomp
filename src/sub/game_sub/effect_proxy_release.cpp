struct EffectProxyReleaseView {
    unsigned char bytes[0x30];
};
extern "C" void func_game_sub_09C51D98(void *);

extern "C" void func_game_sub_09C2E860(unsigned char *self, void *proxy,
                                     unsigned char count)
{
    short start = -1;
    unsigned char *occupied;
    EffectProxyReleaseView *proxies;
    int i;
    unsigned char *cursor;
    for (i = 0, cursor = self; i < 82; i++, cursor += 0x30) {
        if (proxy == cursor + 0x19BC0) {
            occupied = self + 0x19A60;
            start = i;
            proxies = (EffectProxyReleaseView *)(self + 0x19BC0);
            break;
        }
    }
    // The original scans both banks, including after a first-bank hit.
    for (i = 0, cursor = self; i < 256; i++, cursor += 0x30) {
        if (proxy == cursor + 0x1AB20) {
            occupied = self + 0x19AB2;
            start = i;
            proxies = (EffectProxyReleaseView *)(self + 0x1AB20);
            break;
        }
    }
    for (int i = start; i < start + count; i++) {
        occupied[i] = 0;
        func_game_sub_09C51D98(&proxies[i]);
    }
}
