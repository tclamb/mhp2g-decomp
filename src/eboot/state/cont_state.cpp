#include "common.h"
#include "game.hpp"
#include "state.hpp"

extern "C"
{
extern void *D_eboot_089C70EC;

void func_eboot_0888FF00(game*, int, int);
void func_eboot_0884D658(void*, int, int, int);
void func_eboot_0888FE4C(game*, int, int);
SceBool func_eboot_0888FEE8(game*);

}

cont_state::cont_state() { status = 0; }

cont_state::~cont_state() {}

void cont_state::load() {
    switch (status) {
    case 0:
        func_eboot_0884D658(D_eboot_089C70EC, 1, 0, 1);
        func_eboot_0888FE4C(game::instance, 0x2D, 1);
        status = (s32) (status + 1);
        return;
    case 1:
        if (func_eboot_0888FEE8(game::instance) == 0) {
            func_eboot_0888FF00(game::instance, 0x2D, 1);
            status = 0;
            base_state::mem_fn next = (base_state::mem_fn)&cont_state::on_load;
            if (next != 0) {
                action = next;
            }
        }
        return;
    default:
        return;
    }
}
