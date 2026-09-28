struct EffectResourceView {
    unsigned char reserved_00[0x2c];
    void *resource;
    void *texture;
    void *model;
};
extern "C" void func_game_sub_09C51EA8(EffectResourceView *self, void *resource, void *model, void *texture)
{
    self->resource = resource;
    self->model = model;
    self->texture = texture;
}
