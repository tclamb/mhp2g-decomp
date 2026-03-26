#pragma once

#include "model_base.hpp"

// stub for type with vtable at D_eboot_089B5C10
struct ObjBase : ModelBase {
    u8 padding_0x1C0[0x1DC - 0x1C0];
    ScePspFVector4 (*diffuse_light_color_override)[3];
    u8 padding_0x1E0[0x200 - 0x1E0];
    ScePspFVector4 position;
    u8 padding_0x210[0x10];
    ScePspFVector4 scale;
    u8 alpha;
    u8 padding_0x231[0x2AC - 0x231];
    ScePspUnion32 diffuse_light_colors[3];
    u8 padding_0x2B4[0x480 - 0x2B8];
};
