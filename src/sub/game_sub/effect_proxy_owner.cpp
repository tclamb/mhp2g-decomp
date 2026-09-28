struct EffectProxyOwnerView {
    unsigned char reserved_00[0x24];
    void *owner;
};
extern "C" void func_game_sub_09C51DE8(EffectProxyOwnerView *self, void *owner)
{
    self->owner = owner;
}
