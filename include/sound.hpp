#pragma once

#include "common.h"
#include "singleton.hpp"

struct Sound : Singleton<Sound> {
    u8 padding[0x562480];

    Sound();
    ~Sound();
};

extern "C" {
    int func_eboot_08885198(Sound *, u32, u32, u32, u8);
    int func_eboot_08883858(Sound *, u32, u32, u32, u32, u32, ScePspFVector4*, u32, u32, u32, bool);
    int func_eboot_088852C8(Sound *, u32, u32, u32);
    void func_eboot_0888444C(Sound *, u32, u32, u32, u32, ScePspFVector4 *, u32, u32, u32);
    int func_eboot_08883CF4(Sound *, ScePspFVector4 *, u32);
    void func_eboot_088814EC(Sound *, int, u32);
    void func_eboot_08882620(Sound *, int);
    void func_eboot_08882B28(Sound *, int, int, int);
    void func_eboot_08882C5C(Sound *, int);
    void func_eboot_08885234(Sound *, int);
    void func_eboot_08885420(Sound *, int, int);
    void func_eboot_08885574(Sound *, int, int);
}
