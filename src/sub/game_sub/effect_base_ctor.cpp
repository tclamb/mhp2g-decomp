extern "C" unsigned int D_eboot_089B8C04[];
struct EffectBaseView {
    void *vtable;
    unsigned char reserved_04[0x3d];
    unsigned char state;
    unsigned char reserved_42[2];
    void *draw;
};
extern "C" EffectBaseView *func_game_sub_09C51DF0(EffectBaseView *self)
{
    self->vtable = D_eboot_089B8C04;
    self->state = 0;
    self->draw = 0;
    return self;
}
