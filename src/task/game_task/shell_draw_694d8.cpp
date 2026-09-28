#define SHELL_BASE_STATE_INT
#include "shell_model_draw.hpp"

// two models: +0x12C in the sorted translucent group 7, +0x84 in the opaque group 5
extern "C" void func_game_task_09B694D8(ShellWithModel *self, int state) {
    ShellDrawObj **pp = &self->draw_obj_12C;
    u8 st = state;
    ShellDrawObj *d = *pp;
    if (d != 0) {
        if ((bool)(d->flags & Draw::REGISTERED) == false) {
            DrawManager::objectPtr->add(7, d, &d->position, false);
            if (!st) {
                (*pp)->flags |= Draw::REGISTERED;
            }
        }
    }
    d = self->draw_obj;
    if (d != 0) {
        if ((bool)(d->flags & Draw::REGISTERED) == false) {
            DrawManager::objectPtr->add(5, d, &d->position, false);
            if (!st) {
                self->draw_obj->flags |= Draw::REGISTERED;
            }
        }
    }
    func_game_task_09B62458(self, state);
}
