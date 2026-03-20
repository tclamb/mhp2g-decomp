#pragma once

#include "common.h"

struct Evdemo {
    u8 pad_0x0[0xA0];
} __attribute__((aligned(4)));

extern "C" {
    bool func_eboot_088D0824(Evdemo *, u32, u8);
}
