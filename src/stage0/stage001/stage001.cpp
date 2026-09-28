#include "common.h"
#include "stage_base.hpp"

// Layouts not identified yet are kept as exact 32-bit words from stage001.ovl.
extern u8 D_stage001_09D5EBF0[0x10];
extern u8 D_stage001_09D5EC00[0x10];
extern u8 D_stage001_09D5EC10[0x70];

struct Stage001 : StageBase {
    Stage001();
    virtual ~Stage001();

    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual void *vtable_0x3C();
    virtual void *vtable_0x40();
    virtual ScePspFVector4 *vtable_0x44();
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

u32 D_stage001_09D5E300[4] = {
    0x090F000F, 0x04170118, 0x070A030A, 0x00FF0B03
};

u32 D_stage001_09D5E310[4] = {
    0x090F000F, 0x04170118, 0x070A030A, 0x00FF0B03
};

u32 D_stage001_09D5E320[6] = {
    0x090A0014, 0x01120A05, 0x030B0412, 0x0D070708,
    0x00FF0B03, 0x00000000
};

u32 D_stage001_09D5E338[2] = {
    0x00FF0064, 0x00000000
};

u32 D_stage001_09D5E340[2] = {
    0x02320132, 0x000000FF
};

u32 D_stage001_09D5E348[6] = {
    0x090A0012, 0x01120A05, 0x030B0412, 0x0D070708,
    0x00FF0B05, 0x00000000
};

void *D_stage001_09D5E360[6] = {
    D_stage001_09D5E300, D_stage001_09D5E310,
    D_stage001_09D5E320, D_stage001_09D5E338,
    D_stage001_09D5E340, D_stage001_09D5E348
};

u32 D_stage001_09D5E378[4] = {
    0x00000000, 0x00000001, 0x00000020, 0x00000000
};

u32 D_stage001_09D5E388[12] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x46322000, 0x00000000,
    0x46309000, 0x460CA000, 0x461C4000, 0x00020001
};

u32 D_stage001_09D5E3B8[24] = {
    0x3F800000, 0x3F800000, 0x3F800000, 0x00000000,
    0x3F000000, 0x3F000000, 0x3F000000, 0x00000000,
    0x3E99999A, 0x3ED70A3D, 0x3EE147AE, 0x00000000,
    0x3E99999A, 0x3EA3D70A, 0x3EAE147B, 0x00000000,
    0x3E4CCCCD, 0x3E6147AE, 0x3E75C28F, 0x00000000,
    0x3DCCCCCD, 0x3DF5C28F, 0x3E0F5C29, 0x00000000
};

u32 D_stage001_09D5E418[24] = {
    0x00170000, 0x46322000, 0xC2480000, 0x4656B000,
    0x43C80000, 0x0000E000, 0x00180000, 0x461AB000,
    0xC2C80000, 0x463E2800, 0x43480000, 0x00000000,
    0x001E0000, 0x462DF000, 0x00000000, 0x464B5800,
    0x43960000, 0x0000DB07, 0x00210000, 0x4627C000,
    0xC2C80000, 0x463E7800, 0x43480000, 0x00000000
};

u32 D_stage001_09D5E478[42] = {
    0x00000002, 0x465F7000, 0xC37A0000, 0x46309000,
    0x43C80000, 0x44BB8000, 0x00000000, 0x00000000,
    0x00000000, 0x460B1800, 0x00000000, 0x46298000,
    0x00006000, 0x00000005, 0x45E74000, 0xC37A0000,
    0x464B2000, 0x43C80000, 0x44BB8000, 0x00000000,
    0x00000000, 0x00000000, 0x45F55000, 0xC2480000,
    0x465AC000, 0x00008000, 0x00000012, 0x462BE000,
    0x45192000, 0x45EA6000, 0x43C80000, 0x44BB8000,
    0x00000000, 0x00000000, 0x00000000, 0x4668D000,
    0xC2480000, 0x46309000, 0x0000A000, 0x00000000,
    0x00000000, 0x00000000
};

u32 D_stage001_09D5E520[16] = {
    0x00000020, 0x00000006, 0x00000049, 0x0000000B,
    0x4648D800, 0xC26C0000, 0x46897400, 0x00000000,
    0x00000020, 0x00000006, 0x0000004A, 0x0000000B,
    0x45B6B800, 0xC2940000, 0x467ACC00, 0x00000000
};

struct Stage001DefinitionsBlock { stage_definitions value; u8 pad[8]; };
Stage001DefinitionsBlock D_stage001_09D5E560 = {{
    0, 1, 0,
    (u8 *)D_stage001_09D5E378,
    (u16 *)D_stage001_09D5E418,
    0xFFFF,
    {0x09, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    (stage_definitions_0x28_t *)D_stage001_09D5E388,
    (stage_definitions_0x2C_t *)D_stage001_09D5E3B8,
    (stage_exit *)D_stage001_09D5E478,
    (stage_sound *)D_stage001_09D5E520,
    NULL,
    0, 3, 4, 2, 0, 0, 0
}, {0}};

u32 D_stage001_09D5E5B0[8] = {
    0x00000002, 0x00002000, 0x00000000, 0x00000000,
    0x461C4000, 0x00000000, 0x461C4000, 0x00000000
};

stage_draw_command D_stage001_09D5E5D0[6] = {
    {2, 6, 0, 0, D_stage001_09D5E5B0},
    {1, 0, 0x80, 1},
    {1, 0, 0, 2},
    {1, 0, 0x80, 3},
    {1, 0, 0x80, 4},
    {1, 0, 0x80, 5}
};

struct Stage001UvWords { u32 first[3]; u32 second[3]; };
Stage001UvWords D_stage001_09D5E600 = {
    {0x02000000, 0x00000000, 0x00000080},
    {0x00200000, 0x00000000, 0x02000100}
};

stage_draw_command D_stage001_09D5E618[6] = {
    {6, 0, 0, 3, D_stage001_09D5E600.first},
    {6, 0, 0, 4, (u8 *)D_stage001_09D5E600.first + 6},
    {6, 0, 0, 5, D_stage001_09D5E600.second},
    {6, 0, 0, 6, (u8 *)D_stage001_09D5E600.second + 6},
    {1, 0, 0x80, 7},
    {6, 0, 0, 8, (u8 *)D_stage001_09D5E600.second + 6}
};

stage_draw_commands D_stage001_09D5E648 = {
    6, 6, D_stage001_09D5E5D0, D_stage001_09D5E618
};

u8 padding_09D5E654[4] = {};

u32 D_stage001_09D5E658[6] = {
    0x00070009, 0x00000000, 0x00000000, 0x477FFF00,
    0x477FFF00, 0x00000000
};

u32 D_stage001_09D5E670[12] = {
    0x45C94D9A, 0xC2C60000, 0x4668B800, 0x00000000,
    0x461BBA66, 0xC2BC0000, 0x46694B9A, 0x00000000,
    0x4649279A, 0xC2E60000, 0x46592ACD, 0x00000000
};

u32 D_stage001_09D5E6A0[24] = {
    0x46302933, 0xC2D60000, 0x465CC466, 0x00000000,
    0x43C30000, 0x00000000, 0x00000005, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

Stage001::Stage001() {
}

Stage001::~Stage001() {
}

// The setup uses VFPU stores and still needs a compiler-matching C++ body.
INCLUDE_ASM("asm/stage0/stage001/nonmatchings/stage001", vtable_0x24__8Stage001Fv);

stage_definitions *Stage001::definitions() {
    return &D_stage001_09D5E560.value;
}

// This target reads the second argument, while StageBase currently declares none.
INCLUDE_ASM("asm/stage0/stage001/nonmatchings/stage001", vtable_0x3C__8Stage001Fv);

void *Stage001::vtable_0x40() {
    return (void *)1;
}

ScePspFVector4 *Stage001::vtable_0x44() {
    return (ScePspFVector4 *)D_stage001_09D5E6A0;
}

void Stage001::operator delete(void *) {
}

stage_draw_commands *Stage001::vtable_0x48() {
    return &D_stage001_09D5E648;
}

bool Stage001::vtable_0xA4() {
    return true;
}

bool Stage001::vtable_0xA8() {
    return true;
}

bool Stage001::vtable_0xA0() {
    return true;
}

int Stage001::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage001::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage001::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage001::vtable_0xB8() {
    return definitions()->unknown_0x38;
}

