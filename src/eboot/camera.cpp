#include "camera.hpp"
#include <game_sys.hpp>

#define DEGREES_TO_VFPU(degrees) ((degrees) / 90.0f)
#define PI 3.141592653589793238462643383279502884

Camera *Singleton<Camera>::objectPtr;

Camera::Camera() {
    zClipping = true;
    unknown_0xB70 = 1808;
    unknown_0xB74 = 1912;
    unknown_0xC = DEGREES_TO_VFPU(78.5);
    unknown_0x10 = -999;
    unknown_0xB34 = 0;
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088137C8);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088138DC);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_0881395C);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813990);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813B78);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813CD8);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813D24);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813D50);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813DE4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813E84);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813F18);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813F48);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813F78);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08813FBC);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088140E0);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814148);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814258);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814280);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088142A8);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814318);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814320);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814334);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814354);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088148F4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088148FC);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814A4C);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814A88);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814AB8);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814B00);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814B5C);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814B6C);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814B98);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814BD0);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814BD8);

extern "C"
bool func_eboot_08814BE0(Camera *this_) {
    if (this_->subCameras[5].unknown_0x91) {
        switch (this_->subCameras[5].unknown_0xA0.unknown_0x4E) {
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
            return true;
        default:
            break;
        }
    }
    return false;
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814C30);

extern "C"
bool func_eboot_08814C50(Camera *this_) {
    if (Singleton<GameSys>::objectPtr->allow_hidden_flag == true) {
        return true;
    }
    SubCameraData &unknown = this_->subCameras[5].unknown_0xA0;
    if (this_->subCameras[5].unknown_0x13A) {
        return true;
    }
    switch (unknown.unknown_0x4E) {
    case 0x3:
    case 0x4:
    case 0x5:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
        return true;
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    default:
        return false;
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814CC4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814E08);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814E40);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814E84);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814EA4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814EC4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814ED4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815028);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815274);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815384);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815434);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815588);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815744);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088157D4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088159A0);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815B44);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815BD8);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815DC4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815DE4);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08815EC8);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816020);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816108);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816144);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088164C0);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816698);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816724);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816B24);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816B44);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816BE8);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816C5C);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816E20);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816EA8);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816EB0);

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08817024);

ScePspFVector4 D_eboot_089310B0 = {0, 0, -50, 0};
ScePspFVector4 D_eboot_089310C0 = {0, 1, 0, 0};
float D_eboot_089310D0 = 0.075;
u8 D_eboot_089310D4[3] = {0, 2, 20};
u16 D_eboot_089310D8[8] = {10, 20, 30, 40, 50, 60, 70, 80};
float D_eboot_089310E8[3] = {2000, 3000, 4000};
ScePspFVector4 D_eboot_08931100 = {0, 0, 320, 0};
ScePspFVector4 D_eboot_08931110 = {0, 0, -1000, 0};

struct SmallCameraData {
    float unknown_0x0;
    float unknown_0x4;
    float unknown_0x8;
    float unknown_0xC;
    float unknown_0x10;
    float unknown_0x14;
    float unknown_0x18;
};

struct BigCameraData {
    float unknown_0x0[8];
    SmallCameraData unknown_0x20 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0x40 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0x60 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0x80 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0xA0 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0xC0 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0xE0 __attribute__((aligned(0x10)));
};

BigCameraData D_eboot_08931120 = {
    {5 * PI / 18},
    {0, 300, 160, 0, 184, 0, 80},
    {0, 270, 490, 0, 170, 0, 80},
    {0, 140, 490, 0, 170, 0, 80},
    {0, 60, 450, 0, 210, 0, 60},
    {0, 400, 220, 0, 195, 0, 80}
};

SmallCameraData D_eboot_08931220[2] = {
    {0, 280, 500, 0, 170, 0, 80},
    {0, 250, 100, 0, 0, 0, 80},
};

ScePspFVector4 D_eboot_08931260[8] = {
    {0, 0, 30},
    {0, 0, -1000},
    {0, 100, 30},
    {0, 0, -1000},
    {0, 150, -50},
    {0, 0, -1000},
    {100, 350, 400},
    {100, -550, -1000}
};

struct CameraPairData {
    ScePspFVector3 left;
    ScePspFVector3 right;
};

CameraPairData D_eboot_089312E0[39] = {
    {{84, 380, -200}, {10, 21, 127}},
    {{46, 143, 690}, {-21, 154, 176}},
    {{46, 90, 640}, {-21, 104, 176}},
    {{84, 380, -200}, {10, 21, 127}},
    {{300, 320, 40}, {0.0, 50, 160}},
    {{84, 380, -200}, {10, 21, 127}},
    {{125, 461, -263}, {34, 40, 127}},
    {{221, 34, 1050}, {-220, 120, 161}},
    {{-418, -82, -73}, {46, 40, 400}},
    {{84, 380, -200}, {10, 21, 127}},
    {{281, 157, 687}, {-88, -33, 261}},
    {{191, 192, 670}, {-81, -65, 268}},
    {{84, 360, -240}, {10, 55, 222}},
    {{84, 360, -240}, {10, 55, 222}},
    {{-132, 297, -128}, {7, 92, 152}},
    {{90, 297, -174}, {7, 82, 152}},
    {{84, 360, -240}, {10, 55, 222}},
    {{84, 360, -240}, {10, 55, 222}},
    {{525, 410, 365}, {55, 120, 296}},
    {{-294, 90, 1130}, {99, -140, 135}},
    {{-257, 340, 700}, {-16, 120, 299}},
    {{274, 274, 666}, {69, 40, 262}},
    {{84, 360, -240}, {10, 55, 222}},
    {{445, 310, 500}, {-15, 5, 280}},
    {{84, 360, -240}, {10, 55, 222}},
    {{46, 143, 690}, {-21, 154, 176}},
    {{46, 90, 640}, {-21, 104, 176}},
    {{-638, 553, 858}, {844, -621, -210}},
    {{84, 380, -200}, {10, 21, 127}},
    {{125, 461, -263}, {34, 40, 127}},
    {{84, 380, -200}, {10, 21, 127}},
    {{191, 192, 670}, {-81, -65, 268}},
    {{281, 157, 687}, {-88, -33, 261}},
    {{221, 34, 1050}, {-220, 120, 161}},
    {{-418, -82, -73}, {46, 40, 400}},
    {{84, 360, -240}, {10, 55, 222}},
    {{84, 360, -240}, {10, 55, 222}},
    {{-293, 471, -247}, {373, -233, 1020}},
    {{-345, 228, 926}, {380, -80, 110}}
};

float D_eboot_08931688[39] = {
    [0 ... 38] = 11 * PI / 36,
    [21] = 12 * PI / 36,
    [23] = 12 * PI / 36,
};

// maps stageId into D_eboot_089312E0/08931688
u8 D_eboot_08931724[267] = {
    [0 ... 266] = -1,
    [1] = 13,
    [4] = 14,
    [5] = 15,
    [16] = 23,
    [19] = 16,
    [21] = 0,
    [22] = 1,
    [23] = 2,
    [42] = 3,
    [46] = 4,
    [48] = 20,
    [52] = 5,
    [54] = 6,
    [59] = 7,
    [61] = 8,
    [67] = 9,
    [68] = 24,
    [74] = 10,
    [75] = 11,
    [99] = 12,
    [108] = 12,
    [110] = 13,
    [113] = 14,
    [114] = 15,
    [116] = 16,
    [121] = 17,
    [130] = 18,
    [142] = 20,
    [144] = 21,
    [146] = 22,
    [151] = 23,
    [154] = 24,
    [179] = 19,
    [184] = 0,
    [195] = 3,
    [201] = 37,
    [203] = 38,
    [208] = 25,
    [209] = 26,
    [214] = 27,
    [218] = 28,
    [220] = 29,
    [229] = 30,
    [236] = 31,
    [237] = 32,
    [239] = 33,
    [241] = 34,
    [249] = 35,
    [251] = 36,
    [259] = 35,
    [261] = 36,
};

#include "../../assets/eboot/D_eboot_08931830.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089319A0.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089319E8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931A2C.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931A70.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931BF4.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931CC0.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931DEC.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931F74.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932004.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932308.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932568.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932860.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932E58.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089333DC.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08933850.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08933D4C.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089340C0.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934660.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934674.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089347D8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089348E8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089349F8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934B08.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934C18.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934D28.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934E38.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934F44.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089350B8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089351D0.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08935344.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089354CC.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08935640.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_0893575C.cameraop.inc.hpp"

typedef u32 CameraScript[];
extern CameraScript
    D_em02_09D27700, D_em02_09D28270, D_em02_09D28538, D_em02_09D28740, D_em02_09D28D90, D_em02_09D2A1F0,
    D_em07_09D21DC8, D_em07_09D24ED8, D_em07_09D25378, D_em07_09D25848,
    D_em14_09D2AB68, D_em14_09D2AED8,
    D_em15_09D2C3D0,
    D_em17_09D2CBD0,
    D_em20_09D34540, D_em20_09D34E20,
    D_em21_09D29450, D_em21_09D295E0,
    D_em33_09D20C90,
    D_em40_09D35790,
    D_em54_09D3C8E8, D_em54_09D3CB40, D_em54_09D3CF68, D_em54_09D3D358, D_em54_09D3D7F0, D_em54_09D3DAA8, D_em54_09D3DC40,
    D_em55_09D25F08, D_em55_09D26948, D_em55_09D26C90,
    D_em58_09D23010,
    D_em59_09D25308, D_em59_09D25710,
    D_em75_09D56958, D_em75_09D56D70, D_em75_09D57088, D_em75_09D57488, D_em75_09D57490,
    D_lobby_task_09B06098, D_lobby_task_09B06258, D_lobby_task_09B06318, D_lobby_task_09B06420, D_lobby_task_09B06528,
    D_lobby_task_09B06680, D_lobby_task_09B06988, D_lobby_task_09B069F0, D_lobby_task_09B06CA8, D_lobby_task_09B06E30,
    D_lobby_task_09B07250, D_lobby_task_09B072C0, D_lobby_task_09B07388, D_lobby_task_09B073F0, D_lobby_task_09B074D0,
    D_lobby_task_09B078C0, D_lobby_task_09B079D8, D_lobby_task_09B07C60, D_lobby_task_09B07D20, D_lobby_task_09B07DE0,
    D_lobby_task_09B07EA0, D_lobby_task_09B07F60, D_lobby_task_09B08020, D_lobby_task_09B080E0, D_lobby_task_09B081A0;

u32 *D_eboot_08935874[107] = {
    0,
    &D_eboot_08931830[0],
    &D_eboot_08931F74[0],
    &D_eboot_089319A0[0],
    &D_eboot_089319E8[0],
    &D_eboot_08931A2C[0],
    &D_eboot_08931CC0[0],
    &D_eboot_08931BF4[0],
    &D_eboot_08931DEC[0],
    &D_eboot_08931A70[0],
    &D_em21_09D29450[0],
    &D_em07_09D24ED8[0],
    &D_em02_09D28270[0],
    &D_em02_09D28538[0],
    &D_em55_09D26948[0],
    &D_em54_09D3DAA8[0],
    &D_em59_09D25710[0],
    &D_em54_09D3DC40[0],
    &D_em75_09D57488[0],
    &D_em07_09D25378[0],
    &D_em02_09D28740[0],
    &D_em55_09D26C90[0],
    &D_em75_09D57490[0],
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    &D_eboot_08932004[0],
    &D_eboot_08932308[0],
    &D_eboot_08932568[0],
    &D_em20_09D34540[0],
    &D_eboot_08932860[0],
    &D_em20_09D34E20[0],
    &D_eboot_08932E58[0],
    &D_eboot_089333DC[0],
    &D_em54_09D3C8E8[0],
    &D_em15_09D2C3D0[0],
    &D_eboot_08933850[0],
    &D_em21_09D295E0[0],
    &D_eboot_08933D4C[0],
    &D_em14_09D2AB68[0],
    &D_em54_09D3CB40[0],
    &D_em54_09D3CF68[0],
    &D_em54_09D3D358[0],
    &D_em17_09D2CBD0[0],
    &D_em54_09D3D7F0[0],
    &D_em33_09D20C90[0],
    &D_em14_09D2AED8[0],
    &D_em59_09D25308[0],
    &D_eboot_089340C0[0],
    &D_em40_09D35790[0],
    &D_em07_09D21DC8[0],
    &D_em07_09D25848[0],
    &D_em55_09D25F08[0],
    &D_em75_09D56958[0],
    &D_em75_09D56D70[0],
    &D_em02_09D27700[0],
    &D_em02_09D28D90[0],
    &D_em02_09D2A1F0[0],
    &D_em75_09D57088[0],
    &D_em58_09D23010[0],
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    &D_eboot_08934674[0],
    &D_eboot_089347D8[0],
    &D_eboot_089348E8[0],
    &D_eboot_089349F8[0],
    &D_eboot_08934B08[0],
    &D_eboot_08934C18[0],
    &D_eboot_08934D28[0],
    &D_eboot_08934E38[0],
    &D_eboot_08935344[0],
    &D_eboot_089354CC[0],
    &D_eboot_08935640[0],
    &D_eboot_0893575C[0],
    &D_eboot_08934F44[0],
    &D_eboot_089351D0[0],
    &D_eboot_089350B8[0],
    (u32 *)0xFFFFFFFF,
    &D_lobby_task_09B06098[0],
    &D_lobby_task_09B06258[0],
    &D_lobby_task_09B06318[0],
    &D_lobby_task_09B06420[0],
    &D_lobby_task_09B06528[0],
    &D_lobby_task_09B06680[0],
    &D_lobby_task_09B06988[0],
    &D_lobby_task_09B069F0[0],
    &D_lobby_task_09B06CA8[0],
    &D_lobby_task_09B06E30[0],
    &D_lobby_task_09B07250[0],
    &D_lobby_task_09B072C0[0],
    &D_lobby_task_09B07388[0],
    &D_lobby_task_09B073F0[0],
    &D_lobby_task_09B074D0[0],
    (u32 *)0xFFFFFFFF,
    &D_lobby_task_09B078C0[0],
    &D_lobby_task_09B079D8[0],
    &D_lobby_task_09B07C60[0],
    &D_lobby_task_09B07D20[0],
    &D_lobby_task_09B07DE0[0],
    &D_lobby_task_09B07EA0[0],
    &D_lobby_task_09B07F60[0],
    &D_lobby_task_09B08020[0],
    &D_lobby_task_09B080E0[0],
    &D_lobby_task_09B081A0[0],
    &D_eboot_08934660[0],
};

// maps emId to cameraScriptId; death cutscene?
s8 D_eboot_08935A20[90] = {
    [0 ... 89] = 6,
    [0] = -1,
    [3 ... 4] = -1,
    [10] = -1,
    [12] = -1,
    [18] = -1,
    [25] = -1,
    [29] = -1,
    [32] = -1,
    [56] = -1,
    [58] = -1,
    [68 ... 70] = -1,
    [72] = -1,
    [74] = -1,
    [2] = 20,
    [5] = 7,
    [7] = 19,
    [9] = 7,
    [13] = 8,
    [16] = 8,
    [19] = 7,
    [23 ... 24] = 7,
    [27 ... 28] = 8,
    [30 ... 31] = 8,
    [33] = 8,
    [35] = 8,
    [36] = 20,
    [50] = 19,
    [55] = 21,
    [57] = 7,
    [61 ... 63] = 9,
    [66] = 9,
    [68] = 8,
    [71] = 20,
    [73] = 9,
    [76] = 22,
    [77] = 8,
    [79 ... 80] = 8,
    [88] = 23,
};

struct CameraScriptExt {
    u16 cameraScriptIndex;
    u16 stagePacIndexOffset;
    u16 flags;
};

CameraScriptExt D_eboot_08935A7C[10] = {
    {95, 0, 0},
    {79, 0, 0},
    {60, 0, 0},
    {23, 1, 4 | 1},
    {61, 0, 0},
    {62, 0, 0},
    {63, 0, 0},
    {24, 0, 4 | 1},
    {25, 1, 4 | 1},
    {-1},
};
