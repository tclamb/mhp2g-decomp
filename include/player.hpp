#pragma once

#include "obj_base.hpp"

// stub for type with vtable at D_eboot_089B5E78
struct Player : ObjBase {
    u8 padding_0x480[0x614 - 0x480];
    u32 lighting_flags;
};
