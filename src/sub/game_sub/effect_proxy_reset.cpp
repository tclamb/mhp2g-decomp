struct EffectResource {
    void *vtable;
    unsigned int state;
    unsigned int value_08;
    unsigned int value_0c;
    unsigned char reserved_10[0x14];
    void *owner;
};
extern "C" void func_game_sub_09C51D98(EffectResource *self)
{
    self->state = 3;
    self->value_08 = 0;
    self->value_0c = 0;
    self->owner = 0;
}
