struct EffectInitView {
    typedef void (EffectInitView::*Callback)();
    unsigned char reserved_00[0x41];
    unsigned char proxy_count;
    unsigned char reserved_42[2];
    void *proxy;
    unsigned int flags;
    Callback callback;
};
struct EffectInitCallbackPrefix {
    float word0;
    float word1;
};
union EffectInitCallbackCopy {
    struct {
        EffectInitCallbackPrefix prefix;
        float word2;
    } words;
    EffectInitView::Callback value;
};
extern "C" EffectInitCallbackPrefix D_game_sub_09CE0420;
extern "C" float D_game_sub_09CE0428;

extern "C" void func_game_sub_09C51EC8(EffectInitView *self)
{
    self->proxy_count = 0;
    self->proxy = 0;
    self->flags = 1;
    // Preserve the three-word member-pointer representation in the split data labels.
    EffectInitCallbackCopy callback;
    callback.words.prefix = D_game_sub_09CE0420;
    callback.words.word2 = D_game_sub_09CE0428;
    if (callback.value != 0) {
        self->callback = callback.value;
    }
}
