#pragma once

#include "common.h"

struct ResultCheck {
    u8 pad_0x0[0x130];
};

extern "C" {
    // print centered
    void func_eboot_0887CDC8(u16 x, u16 top, char *str, int fontWidth);
}
