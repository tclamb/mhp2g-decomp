#include "common.h"
#include "stage_base.hpp"

struct Stage005 : StageBase {
    Stage005();
    virtual ~Stage005();

    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual stage_draw_commands *vtable_0x48();
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    static void operator delete(void *);
};

// Fields without a known layout retain their original 32-bit patterns.
u32 D_stage005_09D5E300[4] = {0, 1, 0x20, 0};

stage_definitions_0x28_t D_stage005_09D5E310[2] = {
    {{0, 0, 0}, 0, 0, 0, 0},
    {{10000.0f, 0, 10000.0f}, 11900.0f, 12000.0f, 1, 2}
};

u32 D_stage005_09D5E340[24] = {
    0x3F666666, 0x41100000, 0x3F800000, 0x00000000,
    0x3F19999A, 0x3F333333, 0x3F19999A, 0x00000000,
    0x3E99999A, 0x3ECCCCCD, 0x3ECCCCCD, 0x00000000,
    0x3E99999A, 0x3ECCCCCD, 0x3ECCCCCD, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000
};

u32 D_stage005_09D5E3A0[6] = {
    0x00170000, 0x45F6E000, 0xC2C80000,
    0x4607F000, 0x43480000, 0x0000E800
};

u32 D_stage005_09D5E3B8[42] = {
    0x00000001, 0x45F3C000, 0xC2960000, 0x466B2800,
    0x43FA0000, 0x44BB8000, 0x00000000, 0x00000000,
    0x00000000, 0x460CA000, 0xC2C80000, 0x46480000,
    0x00005555, 0x00000004, 0x45E01000, 0xC2960000,
    0x458F7000, 0x442F0000, 0x44480000, 0x00000000,
    0x00000000, 0x00000000, 0x45E10000, 0xC1C80000,
    0x4601B000, 0x00004000, 0x00000004, 0x45A5A000,
    0xC3480000, 0x459C4000, 0x442F0000, 0x44BB8000,
    0x00000000, 0x00000000, 0x00000000, 0x45EA6000,
    0xC2960000, 0x45E42000, 0x00002AAB, 0x00000000,
    0x00000000, 0x00000000
};

u32 D_stage005_09D5E460[16] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x45DAC000, 0xC22C0000, 0x458CA000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000002, 0x00000000, 0x00000000, 0x00000000
};

stage_definitions D_stage005_09D5E4A0 = {
    0, 0, 0,
    (u8 *)D_stage005_09D5E300,
    (u16 *)D_stage005_09D5E3A0,
    6,
    {0x0A, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    D_stage005_09D5E310,
    (stage_definitions_0x2C_t *)D_stage005_09D5E340,
    (stage_exit *)D_stage005_09D5E3B8,
    NULL,
    (stage_definitions_0x38_t *)D_stage005_09D5E460,
    0, 3, 1, 0, 1, 0, 0
};

u8 padding_09D5E4E4[0xC] = {};

vtable_0x54_params D_stage005_09D5E4F0 = {
    2, 0x2000, {10000, 0, 10000, 0}
};

stage_draw_command D_stage005_09D5E510[8] = {
    {2, 6, 0, 0, &D_stage005_09D5E4F0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2},
    {1, 0, 0x80, 3},
    {1, 0, 0x80, 4},
    {1, 0, 0x80, 5},
    {1, 0, 0x80, 6},
    {1, 0, 0x80, 7}
};

vtable_0x54_params D_stage005_09D5E550 = {
    2, 0x2000, {10000, 0, 10000, 0}
};

u32 D_stage005_09D5E570[2] = {0x02000000, 0};

stage_draw_command D_stage005_09D5E578[2] = {
    {2, 0, 0, 0, &D_stage005_09D5E550},
    {6, 0, 0, 5, D_stage005_09D5E570}
};

stage_draw_commands D_stage005_09D5E588 = {
    8, 2, D_stage005_09D5E510, D_stage005_09D5E578
};

u8 padding_09D5E594[0xC] = {};

u32 D_stage005_09D5E5A0[12] __attribute__((aligned(16))) = {
    0x45ACC4CD, 0xC2C80000, 0x466BA0CD, 0x00000000,
    0x45B190CD, 0xC2C80000, 0x461F3F33, 0x00000000,
    0x4562B800, 0xC2C80000, 0x45CF3C00, 0x00000000
};

u32 D_stage005_09D5E5D0[12] = {
    0x0007000A, 0x00000000, 0x00000000, 0x477FFF00,
    0x477FFF00, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

Stage005::Stage005() {
}

Stage005::~Stage005() {
}

// This setup uses VFPU stores and a ten-argument prop call.
INCLUDE_ASM("asm/stage0/stage005/nonmatchings/stage005", vtable_0x24__8Stage005Fv);

stage_definitions *Stage005::definitions() {
    return &D_stage005_09D5E4A0;
}

void Stage005::operator delete(void *) {
}

stage_draw_commands *Stage005::vtable_0x48() {
    return &D_stage005_09D5E588;
}

bool Stage005::vtable_0xA4() {
    return true;
}

bool Stage005::vtable_0xA8() {
    return true;
}

bool Stage005::vtable_0xA0() {
    return true;
}

int Stage005::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage005::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage005::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage005::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
