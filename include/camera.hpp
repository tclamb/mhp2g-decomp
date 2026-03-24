#pragma once

#include "common.h"
#include "singleton.hpp"

struct SubCameraData {
    u8 pad_0x0[0x4E];
    u8 unknown_0x4E;
    u8 pad_0x4F;
};

struct SubCamera {
    SubCamera() {
        unknown_0x90 = 0;
        unknown_0x92 = 0;
    }
    ~SubCamera() {}

    u8 pad_0x0[0x90];
    u8 unknown_0x90;
    bool unknown_0x91;
    u8 unknown_0x92;
    u8 pad_0x93[0xA0 - 0x93];
    SubCameraData unknown_0xA0;
    u8 unknown_0xF0[0x13A - 0xF0];
    bool unknown_0x13A;
    u8 pad_0x13B[0x1A0 - 0x13B];
};

struct Camera : Singleton<Camera> {
    float near_z;
    float far_z;
    u8 padding_0x8[0x4];
    float unknown_0xC;
    float unknown_0x10;
    u8 padding_0x14[0xB0 - 0x14];
    SubCamera subCameras[6];
    u8 padding_0xA70[0xAAA - 0xA70];
    bool zClipping;
    u8 padding_0xAAB[0xB34 - 0xAAB];
    float unknown_0xB34;
    u8 padding_0xB38[0xB70 - 0xB38];
    float unknown_0xB70;
    float unknown_0xB74;
    u8 padding_0xB78[0xB80 - 0xB78];
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
    ~Camera() {};
};

extern "C" {
    // clipping test; objects are clipped when false
    int func_eboot_08816EA8(Camera *, ScePspFVector4 *position, float clipping_distance);
    int func_eboot_08816E20(Camera *, ScePspFVector4 *, float);
    void func_eboot_08814E84(Camera *, ScePspFVector4 *);
    void func_eboot_088157D4(Camera *, void *);
}
