#include "shell_model_draw.hpp"

extern "C" void func_game_task_09B88208(ShellWithModel *self, u8 state) {
    ShellDrawObj *d = self->draw_obj;
    if (d != 0) {
        if ((bool)(d->flags & Draw::REGISTERED) == false) {
            DrawManager::objectPtr->add(5, d, &d->position, false);
            if (state == 0) {
                self->draw_obj->flags |= Draw::REGISTERED;
            }
        }
    }
    func_game_task_09B62458(self, state);
}
