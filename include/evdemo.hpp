#pragma once

#include "common.h"
#include "singleton.hpp"

struct Evdemo : Singleton<Evdemo> {
    u8 pad_0x0[0xA0];

    Evdemo();
} __attribute__((aligned(4)));

extern "C" {
    bool func_eboot_088D0824(Evdemo *, u32, u8);
}
