#include "shell.hpp"

extern "C" u8 func_game_task_09B62918(Shell *);

// ShellManager per-frame update (called from func_game_task_09A5DA28): updates every shell and
// removes the finished ones from the list, destroying them and returning their memory to the cache.
extern "C" void func_game_task_09B5DF38(ShellManagerView *self) {
    if (self->count != 0) {
        Shell *s = self->head;
        while (s != 0) {
            s->update();
            Shell *cur = s;
            s = s->next;
            if (func_game_task_09B62918(cur) == 1) {
                Shell *next = cur->next;
                Shell *prev = cur->prev;
                if (next == 0 && prev == 0) {
                    if (self->head != cur) {
                        goto destroy;
                    }
                    self->head = 0;
                }
                if (next != 0) {
                    if (prev != 0) {
                        next->prev = prev;
                    } else {
                        self->head = next;
                        next->prev = 0;
                    }
                }
                if (prev != 0) {
                    if (next != 0) {
                        prev->next = next;
                    } else {
                        prev->next = 0;
                        self->tail = prev;
                    }
                }
                self->count--;
            destroy:
                delete cur;
                self->shell_cache.free(cur);
            }
        }
    }
}
