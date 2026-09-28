struct EffectRegistrationNode {
    virtual ~EffectRegistrationNode();
    virtual void update();
    virtual void draw(void *);
    virtual void registerDraw();
    EffectRegistrationNode *next;
    unsigned char reserved_08[0x40];
    unsigned int flags;
};
struct EffectRegistrationManager {
    unsigned char reserved_00[0x19030];
    EffectRegistrationNode *head;
    int count;
};
extern "C" void func_game_sub_09C28408(EffectRegistrationManager *self)
{
    if (self->count != 0) {
        EffectRegistrationNode *effect = self->head;
        while (effect != 0) {
            if (((effect->flags & 0x10) != 0) == 1) effect->registerDraw();
            effect = effect->next;
        }
    }
}
