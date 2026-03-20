#pragma once

#include "common.h"
#include "singleton.hpp"

struct CameraComponent {
    ~CameraComponent();
    u8 pad_0x0[0x1A0];
};

struct Camera : NoInlineConstructorSingleton<Camera> {
    float near_z;
    float far_z;
    u8 padding_0x8[0xB0 - 0x8];
    CameraComponent components_0xB0[6];
    u8 padding_0xA70[0xB80 - 0xA70];
    ScePspFMatrix4 perspective; // unsure
    ScePspFMatrix4 world;
    ScePspFMatrix4 projection;
    u8 padding_0xC40[0xCA0 - 0xC40];
    ScePspFVector4 position;
    u8 padding_0xCB0[0xDA0 - 0xCB0];
    ScePspVector3 viewport_scale;
    ScePspVector3 viewport_center;
    u8 padding_0xDB8[0xDD0 - 0xDB8];

    Camera();
};

extern "C" {
    // clipping test; objects are clipped when false
    int func_eboot_08816EA8(Camera *, ScePspFVector4 *position, float clipping_distance);
    int func_eboot_08816E20(Camera *, ScePspFVector4 *, float);
    void func_eboot_08814E84(Camera *, ScePspFVector4 *);
    void func_eboot_088157D4(Camera *, void *);
}
