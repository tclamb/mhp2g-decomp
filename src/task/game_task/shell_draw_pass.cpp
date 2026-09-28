// ShellManager per-frame draw pass (called from func_game_task_09A5DB68 with the GameSys state
// from func_eboot_088566DC). When the manager holds shells (+0x5034), each shell of the list at
// +0x502C gets its vtable slot 0x14 method, which registers its own draw.
#include "shell.hpp"

extern "C" void func_game_task_09B5E048(ShellManagerView *self, s32 game_state) {
    if (self->count != 0) {
        Shell *s = self->head;
        while (s != 0) {
            s->register_draw(game_state);
            s = s->next;
        }
    }
}
