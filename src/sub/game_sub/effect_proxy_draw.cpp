struct EffectDrawOwner {
    virtual ~EffectDrawOwner();
    virtual void update();
    virtual void draw(void *proxy);
};
struct EffectProxyDispatchView {
    unsigned char reserved_00[0x24];
    EffectDrawOwner *owner;
};
extern "C" void func_game_sub_09C51DB0(EffectProxyDispatchView *self)
{
    if (self->owner != 0) self->owner->draw(self);
}
