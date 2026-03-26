#pragma once

#include "common.h"
#include "singleton.hpp"
#include "player.hpp"

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
    float unknown_0x8;
    float unknown_0xC;
    float unknown_0x10;
    u8 unknown_0x14;
    u8 padding_0x15[0xB0 - 0x15];
    SubCamera subCameras[6];
    u8 padding_0xA70[0xA7C - 0xA70];
    float unknown_0xA7C;
    u8 cameraScriptIndex;
    s8 unknown_0xA81;
    Player *player;
    s8 unknown_0xA88;
    u8 unknown_0xA89;
    s8 unknown_0xA8A;
    u8 padding_0xA8B[0xAA9 - 0xA8B];
    u8 unknown_0xAA9;
    bool zClipping;
    u8 padding_0xAAB[0xAC0 - 0xAAB];
    s8 unknown_0xAC0;
    u8 padding_0xAC1[0xAE0 - 0xAC1];
    s8 unknown_0xAE0;
    u8 padding_0xAE1[0xB00 - 0xAE1];
    s8 unknown_0xB00;
    u8 padding_0xB01[0xB10 - 0xB01];
    u32 unknown_0xB10;
    u8 padding_0xB14[0xB34 - 0xB14];
    float unknown_0xB34;
    u8 padding_0xB38[0xB3C - 0xB38];
    s8 unknown_0xB3C;
    u8 padding_0xB3D[0xB70 - 0xB3D];
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
    u8 padding_0xDB8[8];
    float unknown_0xDC0;
    u8 padding_0xDC4[0xDD0 - 0xDC4];

    Camera();
    ~Camera() {};

    void method_088137C8();

    void method_088138DC();
    void method_0881395C();
    void method_08813990();
    void method_08813B78();
    void method_08813CD8();
    void method_08813D24();
    void method_08813D50();
    void method_08813DE4();
    void method_08813E84();
    void method_08813F18();
    void method_08813F48();
    void method_08813F78();
    void method_08813FBC();
    void method_088140E0();
    void method_08814148();
    void method_08814258();
    void method_08814280();
    void method_088142A8();
    void method_08814318();
    void method_08814320();
    void method_08814334();
    void method_08814354();
    void method_088148F4();
    void method_088148FC();
    void method_08814A4C();
    void method_08814A88();
    void method_08814AB8();
    void method_08814B00();
    void method_08814B5C();
    void method_08814B6C();
    void method_08814B98();
    void method_08814BD0();
    void method_08814BD8();
    void method_08814C30();
    void method_08814CC4();
    void method_08814E08();
    void method_08814E40();
    void method_08814E84();
    void method_08814EA4();
    void method_08814EC4();
    void method_08814ED4();
    void method_08815028(ScePspFVector4 *, ScePspFVector4 *, ScePspFVector4 *);
    void method_08815274();
    void method_08815384();
    void method_08815434();
    void method_08815588();
    void method_08815744();
    void method_088157D4();
    void method_088159A0();
    void method_08815B44();
    void method_08815BD8();
    void method_08815DC4();
    void method_08815DE4();
    void method_08815EC8();
    void method_08816020();
    void method_08816108();
    void method_08816144();
    void method_088164C0();
    void method_08816698();
    void method_08816724();
    void method_08816B24();
    void method_08816B44();
    void method_08816BE8();
    void method_08816C5C();
    void method_08816E20();
    void method_08816EA8();
    void method_08816EB0();
    void method_08817024();
};

extern "C" {
    // clipping test; objects are clipped when false
    int func_eboot_08816EA8(Camera *, ScePspFVector4 *position, float clipping_distance);
    int func_eboot_08816E20(Camera *, ScePspFVector4 *, float);
    void func_eboot_08814E84(Camera *, ScePspFVector4 *);
    void func_eboot_088157D4(Camera *, void *);
}
