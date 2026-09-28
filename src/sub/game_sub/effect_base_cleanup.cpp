#include "singleton.hpp"
struct EffectManager;
struct EffectCleanupView {
    unsigned char reserved_00[0x41];
    unsigned char proxy_count;
    unsigned char reserved_42[2];
    void *proxy;
    unsigned int flags;
};
extern "C" void func_game_sub_09C2E860(EffectManager *, void *, unsigned char);

extern "C" void func_game_sub_09C51F58(EffectCleanupView *self)
{
    self->flags &= ~1;
    if (self->proxy_count != 0) {
        if (self->proxy != 0) {
            func_game_sub_09C2E860(Singleton<EffectManager>::objectPtr,
                                  self->proxy, self->proxy_count);
        }
    }
}
