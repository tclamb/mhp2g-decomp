#pragma once

#include "vtable_14_lifecycle.hpp"

// These actors share the list prefix used by the vtable_14 lifecycle.
struct Em75V13Actor : Em75V14Actor {
    u8 type;
    u8 mode;
};

extern "C" u32 D_eboot_089BA480[];
extern "C" void func_eboot_08865F1C(u8 *, float *, int);
