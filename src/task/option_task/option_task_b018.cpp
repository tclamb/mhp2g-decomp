#include "common.h"
#include "cockpit.hpp"
#include "game_sys.hpp"
#include "system_font.hpp"

struct OptionTask {
    u8 unk00[0x1C];
    s32 unk1C;
    s32 unk20;
};

extern u16 D_eboot_089A9F50[];
extern u16 D_eboot_089A9F52[];
extern u16 *D_option_task_09A5BD9C[7];
extern char D_option_task_09A5BDC0[];

extern "C" bool func_option_task_09A5B5D0(OptionTask *this_);
extern "C" void func_option_task_09A5B678(OptionTask *this_, s32 x, s32 y, s32 color, s8 value);
extern "C" void func_eboot_08838B84(Cockpit *cockpit, s32, s32, s32, s32, u32, s32);

extern "C" void func_option_task_09A5B018(OptionTask *this_) {
    s8 *settings = (s8 *)&GameSys::objectPtr->pad_0x6ADD8;
    s32 y = 0x38;
    s32 j = 1;
    s32 i;
    s8 color;
    char *str;
    s32 x;
    s32 idx;

    SystemFont::objectPtr->setLayer(0);
    for (i = 0; i < 7; i++, j++, y += 0x12) {
        SystemFont::objectPtr->setFontSize(14, 14);
        if (i == this_->unk20 && this_->unk1C < 5) {
            color = 3;
        } else if (j == 7) {
            if (func_option_task_09A5B5D0(this_) == 1) {
                color = 9;
            } else {
                color = 0;
            }
        } else {
            color = 0;
        }
        SystemFont::objectPtr->printfUtf8(0x3A, y, color, GameSys::objectPtr->method_0885143C(D_eboot_089A9F52[i]));
        if (D_option_task_09A5BD9C[i] != NULL) {
            if (j == 1) {
                str = GameSys::objectPtr->method_0885143C(D_option_task_09A5BD9C[i][settings[j] - 1]);
            } else if (j == 2 || j == 3) {
                str = GameSys::objectPtr->method_0885143C(D_option_task_09A5BD9C[i][0]);
            } else {
                str = GameSys::objectPtr->method_0885143C(D_option_task_09A5BD9C[i][settings[j]]);
            }
            SystemFont::objectPtr->setFontSize(0x1C, 14);
            x = 0x159 - SystemFont::objectPtr->halfWidths((u8 *)str) * 14 / 2;
            if (j == 2 || j == 3) {
                SystemFont::objectPtr->printfUtf8(0xF7, y, color, GameSys::objectPtr->method_0885143C(D_option_task_09A5BD9C[i][0]));
                SystemFont::objectPtr->printfUtf8(0x191, y, color, GameSys::objectPtr->method_0885143C(D_option_task_09A5BD9C[i][1]));
                func_option_task_09A5B678(this_, 0x128, y, color, settings[j]);
            } else {
                SystemFont::objectPtr->printfUtf8(x, y, color, str);
            }
        }
    }
    SystemFont::objectPtr->setFontColor(0);
    if (this_->unk20 != 6) {
        idx = 8;
    } else {
        idx = 9;
    }
    func_eboot_08838B84(Cockpit::objectPtr, 0x148, 0xAD, 0x9A, 0x11, 0x801D3905, 0x81);
    SystemFont::objectPtr->printfUtf8x(0x168, 0xAF, D_option_task_09A5BDC0, GameSys::objectPtr->method_0885143C(D_eboot_089A9F50[idx]));
}
