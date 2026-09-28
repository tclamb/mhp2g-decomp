struct EffectProxyAllocView {
    unsigned char bytes[0x30];
};
extern "C" void func_game_sub_09C51D98(void *);

extern "C" void *func_game_sub_09C2E6E0(unsigned char *self,
                                     unsigned char count,
                                     unsigned char tag,
                                     unsigned char bank)
{
    short start = -1;
    short run = 0;
    int capacity;
    bool found = false;
    unsigned char *occupied;
    EffectProxyAllocView *proxies;
    if (bank == 1) {
        capacity = 82;
        occupied = self + 0x19A60;
        proxies = (EffectProxyAllocView *)(self + 0x19BC0);
    } else {
        capacity = 256;
        occupied = self + 0x19AB2;
        proxies = (EffectProxyAllocView *)(self + 0x1AB20);
    }
    for (int i = 0; i < capacity; i++) {
        if (occupied[i] == 0) {
            if (start < 0) start = i;
            run++;
        } else if (start >= 0) {
            start = -1;
            run = 0;
        }
        if (run == count) {
            found = true;
            break;
        }
    }
    if (found == false) return 0;
    for (int i = start; i < start + count; i++) {
        occupied[i] = tag | 0x80;
        func_game_sub_09C51D98(&proxies[i]);
    }
    return &proxies[start];
}
