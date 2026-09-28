#include "common.h"
#include "cockpit.hpp"
#include "game_sys.hpp"
#include "system_font.hpp"

struct OptionTask {
    u8 unk00[0x1C];
    s32 unk1C;
    s32 unk20;
};

extern u16 D_eboot_089A9F80[12];
extern u16 D_eboot_089A9F98;
extern char D_option_task_09A5BDC0[];

extern "C" void func_option_task_09A5B368(OptionTask *this_) {
    Cockpit::objectPtr->method_0882DEE4(0x10, 0xC0, 0x1C0, 0x48, 0xFF);
    if (this_->unk1C < 5) {
        s32 idx;
        if (this_->unk20 == 0 && GameSys::objectPtr->byte_0x6ADD9 == 2) {
            idx = 10;
        } else if (this_->unk20 == 0 && GameSys::objectPtr->byte_0x6ADD9 == 3) {
            idx = 11;
        } else {
            idx = this_->unk20 + 1;
        }
        SystemFont::objectPtr->printfUtf8x(0x17, 0xD0, D_option_task_09A5BDC0, GameSys::objectPtr->method_0885143C(D_eboot_089A9F80[idx]));
    } else {
        SystemFont::objectPtr->printfUtf8x(0x17, 0xD0, D_option_task_09A5BDC0, GameSys::objectPtr->method_0885143C(D_eboot_089A9F98));
    }
}
