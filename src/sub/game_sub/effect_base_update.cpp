struct EffectMemberCallbackView {
    unsigned char reserved_00[0x4c];
    void (EffectMemberCallbackView::*callback)();
};
extern "C" void func_game_sub_09C51E58(EffectMemberCallbackView *self)
{
    if (self->callback != 0) {
        (self->*self->callback)();
    }
}
