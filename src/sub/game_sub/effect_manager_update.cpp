#include "cache.hpp"

struct EffectUpdateNode {
    virtual ~EffectUpdateNode();
    virtual void update();
    EffectUpdateNode *next;
    EffectUpdateNode *previous;
    unsigned char reserved_0c[0x3c];
    unsigned int flags;
};
struct EffectUpdateManager {
    unsigned char reserved_00[0x19004];
    cache storage;
    unsigned char reserved_19020[0x10];
    EffectUpdateNode *head;
    int count;
};

extern "C" void func_game_sub_09C282D0(EffectUpdateManager *self)
{
    if (self->count != 0) {
        EffectUpdateNode *effect = self->head;
        while (effect != 0) {
            effect->update();
            EffectUpdateNode *dead = effect;
            effect = effect->next;
            if (((dead->flags & 1) != 0) == 0) {
                EffectUpdateNode *previous = dead->previous;
                if (effect == 0 && previous == 0) {
                    if (self->head != dead) goto release;
                    self->head = 0;
                }
                if (effect != 0) {
                    if (previous != 0) {
                        effect->previous = previous;
                    } else {
                        self->head = effect;
                        effect->previous = 0;
                    }
                }
                if (previous != 0) {
                    if (effect != 0) previous->next = effect;
                    else previous->next = 0;
                }
                self->count--;
            release:
                delete dead;
                self->storage.free(dead);
            }
        }
    }
}
