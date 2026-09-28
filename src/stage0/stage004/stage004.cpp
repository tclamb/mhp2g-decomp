#include "common.h"
#include "stage_base.hpp"

struct Stage004 : StageBase {
    Stage004();
    virtual ~Stage004();

    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual stage_draw_commands *vtable_0x48();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual bool vtable_0xA0();
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    static void operator delete(void *);
};

// Unknown layouts remain as exact 32-bit words from stage004.ovl.

u32 D_stage004_09D5E300[4] = {
    0x00000000, 0x00000001, 0x00000020, 0x00000000
};

u32 D_stage004_09D5E310[12] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x461C4000, 0x00000000,
    0x461C4000, 0x4639F000, 0x463B8000, 0x00020001
};

u32 D_stage004_09D5E340[24] = {
    0x3F666666, 0x3F800000, 0x3F666666, 0x00000000,
    0x3F19999A, 0x3F19999A, 0x3F333333, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000
};

u32 D_stage004_09D5E3A0[6] = {
    0x00170000, 0x46147000, 0xC2480000, 0x45D7A000,
    0x43480000, 0x00008000
};

u32 D_stage004_09D5E3B8[78] = {
    0x00000016, 0x461AB000, 0x437A0000, 0x4636D000,
    0x43FA0000, 0x43FA0000, 0x00000000, 0x00000000,
    0x00000000, 0x466BF000, 0x449F6000, 0x45228000,
    0x0000F1C7, 0x00000003, 0x4684D000, 0x00000000,
    0x46147000, 0x44160000, 0x44BB8000, 0x00000000,
    0x00000000, 0x00000000, 0x461C9000, 0xC3160000,
    0x45FD2000, 0x00002000, 0x00000005, 0x45BA9000,
    0x00000000, 0x4601B000, 0x43FA0000, 0x44480000,
    0x00000000, 0x00000000, 0x00000000, 0x45E42000,
    0xC2480000, 0x45BB8000, 0x00000000, 0x00000013,
    0x465AC000, 0x00000000, 0x464CB000, 0x44160000,
    0x44BB8000, 0x00000000, 0x00000000, 0x00000000,
    0x464B2000, 0xC2F00000, 0x45898000, 0x0000D8E4,
    0x00000017, 0x45A8C000, 0xC3480000, 0x462BE000,
    0x44160000, 0x43FA0000, 0x00000000, 0x00000000,
    0x00000000, 0x46271000, 0xC2480000, 0x45A23800,
    0x0000F777, 0x00000005, 0x45B86000, 0xC2C80000,
    0x45DAC000, 0x43FA0000, 0x44BB8000, 0x00000000,
    0x00000000, 0x00000000, 0x45BB8000, 0xC2C80000,
    0x45BEA000, 0x00001C72
};

u32 D_stage004_09D5E4F0[16] = {
    0x00000000, 0x00000006, 0x00000079, 0x0000001C,
    0x461AB000, 0x437A0000, 0x463B8000, 0x00000000,
    0x00000000, 0x00000006, 0x00000078, 0x0000001D,
    0x45A8C000, 0xC3480000, 0x462BE000, 0x00000000
};

u32 D_stage004_09D5E530[16] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x45B22000, 0x00000000, 0x4601B000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0x00000000, 0x00000000, 0x00000000
};

stage_definitions D_stage004_09D5E570 = {
    0, 0, 0,
    (u8 *)D_stage004_09D5E300,
    (u16 *)D_stage004_09D5E3A0,
    0,
    {0x0B, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    10,
    (stage_definitions_0x28_t *)D_stage004_09D5E310,
    (stage_definitions_0x2C_t *)D_stage004_09D5E340,
    (stage_exit *)D_stage004_09D5E3B8,
    (stage_sound *)D_stage004_09D5E4F0,
    (stage_definitions_0x38_t *)D_stage004_09D5E530,
    0, 6, 1, 2, 1, 0, 0
};

// The original section reserves another 0xC bytes before the next object.
u32 padding_09D5E5B4[3] = {};

u32 D_stage004_09D5E5C0[8] __attribute__((aligned(16))) = {
    0x00000002, 0x00002000, 0x00000000, 0x00000000,
    0x461C4000, 0x00000000, 0x461C4000, 0x00000000
};

stage_draw_command D_stage004_09D5E5E0[8] = {
    {2, 6, 0, 0, D_stage004_09D5E5C0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2},
    {1, 0, 0x80, 3},
    {1, 0, 0x80, 4},
    {1, 0, 0x80, 5},
    {1, 0, 0x80, 6},
    {0, 0, 0, 0}
};

u32 D_stage004_09D5E620[8] = {
    0x00000002, 0x00002000, 0x00000000, 0x00000000,
    0x461C4000, 0x00000000, 0x461C4000, 0x00000000
};

u32 D_stage004_09D5E640[4] = {
    0x45FA0000, 0x00000000, 0x461C4000, 0x00000000
};

u32 D_stage004_09D5E650[4] = {
    0x461C4000, 0x00000000, 0x46147000, 0x00000000
};

u32 D_stage004_09D5E660[4] = {
    0x463B8000, 0x00000000, 0x461C4000, 0x00000000
};

u32 D_stage004_09D5E670[4] = {
    0x465AC000, 0x00000000, 0x461C4000, 0x00000000
};

stage_draw_command D_stage004_09D5E680[5] = {
    {2, 0, 0, 3, D_stage004_09D5E620},
    {3, 0, 0xE0, 8, D_stage004_09D5E640},
    {3, 0, 0xE0, 8, D_stage004_09D5E650},
    {3, 0, 0xE0, 8, D_stage004_09D5E660},
    {3, 0, 0xE0, 8, D_stage004_09D5E670}
};

stage_draw_commands D_stage004_09D5E6A8 = {
    7, 5, D_stage004_09D5E5E0, D_stage004_09D5E680
};

u32 padding_09D5E6B4[3] = {};

u32 D_stage004_09D5E6C0[12] __attribute__((aligned(16))) = {
    0x4676A000, 0xC2480000, 0x45AAA333, 0x00000000,
    0x462BF666, 0xC2480000, 0x459C10CD, 0x00000000,
    0x45D47CCD, 0xC1F00000, 0x459AFC00, 0x00000000
};

u32 D_stage004_09D5E6F0[36] = {
    0x0007000B, 0x00000000, 0x00000000, 0x477FFF00,
    0x477FFF00, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

Stage004::Stage004() {
}

Stage004::~Stage004() {
}

// Setup uses VFPU vector stores; retain the original assembly for now.
INCLUDE_ASM("asm/stage0/stage004/nonmatchings/stage004", vtable_0x24__8Stage004Fv);

stage_definitions *Stage004::definitions() {
    return &D_stage004_09D5E570;
}

void Stage004::operator delete(void *) {
}

stage_draw_commands *Stage004::vtable_0x48() {
    return &D_stage004_09D5E6A8;
}

bool Stage004::vtable_0xA4() {
    return true;
}

bool Stage004::vtable_0xA8() {
    return true;
}

bool Stage004::vtable_0xA0() {
    return true;
}

int Stage004::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage004::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage004::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage004::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
