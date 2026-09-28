#include "common.h"

struct OptionTask {
    u8 unk00[0x1C];
    s32 unk1C;
    u8 unk20[0xF];
    u8 unk2F;
};

extern "C" void func_option_task_09A5AFA8(OptionTask *this_);
extern "C" void func_option_task_09A5B018(OptionTask *this_);
extern "C" void func_option_task_09A5B368(OptionTask *this_);
extern "C" void func_option_task_09A5B498(OptionTask *this_);
extern "C" void func_eboot_088BD264(s32, s32, s32, s32);
extern "C" void func_eboot_088BD198(s32, s32, s32);

extern "C" void func_option_task_09A5AF28(OptionTask *this_) {
    func_option_task_09A5AFA8(this_);
    func_option_task_09A5B018(this_);
    func_option_task_09A5B368(this_);
    if (this_->unk1C == 2) {
        func_eboot_088BD264(0x10, 0x54, 0x1C0, 0x68);
        func_eboot_088BD198(0xF0, 0x60, 0);
    }
    if (this_->unk2F == 1) {
        func_option_task_09A5B498(this_);
    }
}
