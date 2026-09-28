#include "shell_model_draw.hpp"

extern "C" void func_game_task_09B72DB0(ShellWithModel *self, u8 state) {
    ShellDrawObj *d = self->draw_obj;
    if (d != 0) {
        if ((bool)(d->flags & Draw::REGISTERED) == false) {
            DrawManager::objectPtr->add(7, d, &d->position, false);
            if (state == 0) {
                self->draw_obj->flags |= Draw::REGISTERED;
            }
        }
    }
    d = self->draw_obj_114;
    if (d != 0) {
        if ((bool)(d->flags & Draw::REGISTERED) == false) {
            DrawManager::objectPtr->add(7, d, &d->position, false);
            if (state == 0) {
                self->draw_obj_114->flags |= Draw::REGISTERED;
            }
        }
    }
    func_game_task_09B62458(self, state);
}
