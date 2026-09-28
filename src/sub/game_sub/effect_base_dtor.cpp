extern "C" unsigned int D_eboot_089B8C04[];
extern "C" void func_game_sub_09C51EC0(void *);
struct EffectDestructorView {
    void *vtable;
};
extern "C" EffectDestructorView *func_game_sub_09C51E10(EffectDestructorView *self, short deleting)
{
    if (self != 0) {
        self->vtable = D_eboot_089B8C04;
        if (deleting > 0) func_game_sub_09C51EC0(self);
    }
    return self;
}
