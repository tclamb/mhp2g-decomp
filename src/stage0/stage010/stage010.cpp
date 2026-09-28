#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern u8 D_stage010_09D5E590[0x470];

struct Stage010 : StageBase {
    Stage010();
    virtual ~Stage010();

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

// Unknown layouts retain their exact 32-bit words from stage010.ovl.

u32 D_stage010_09D5E200[2] = {
    0x00000000, 0x00000001
};

u32 D_stage010_09D5E208[12] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x459C4000, 0x00000000,
    0x45EA6000, 0x44FA0000, 0x451C4000, 0x00020001
};

u32 D_stage010_09D5E238[24] = {
    0x3F400000, 0x3F400000, 0x3F400000, 0x00000000,
    0x3F400000, 0x3F400000, 0x3F400000, 0x00000000,
    0x3EB33333, 0x3EB33333, 0x3EB33333, 0x00000000,
    0x3EB33333, 0x3EB33333, 0x3EB33333, 0x00000000,
    0x3F0CCCCD, 0x3F0CCCCD, 0x3F0CCCCD, 0x00000000,
    0x3F0CCCCD, 0x3F0CCCCD, 0x3F0CCCCD, 0x00000000
};

u32 D_stage010_09D5E298[18] = {
    0x001E0000, 0x4596F000, 0x3F800000, 0x45CC6000,
    0x43960000, 0x00008000, 0x00180000, 0x45997000,
    0xC1400000, 0x45E33000, 0x43340000, 0x00007000,
    0x00210000, 0x45ADC000, 0xC1400000, 0x45E33000,
    0x43160000, 0x00000000
};

u32 D_stage010_09D5E2E0[66] = {
    0x0000000B, 0x45674000, 0xC1A00000, 0x45FBE000,
    0x43FA0000, 0x43480000, 0x00000000, 0x00000000,
    0x00000000, 0x46723000, 0x00000000, 0x46BAB800,
    0x00000000, 0x0000000E, 0x45C80000, 0xC1A00000,
    0x45DDE000, 0x43960000, 0x43960000, 0x00000000,
    0x00000000, 0x00000000, 0x46354000, 0x43C50000,
    0x464B2000, 0x0000638E, 0x0000001C, 0x45A28000,
    0xC1A00000, 0x46115000, 0x43480000, 0x43480000,
    0x00000000, 0x00000000, 0x00000000, 0x46921800,
    0x00000000, 0x46B92800, 0x0000DC72, 0x0000001E,
    0x45C4E000, 0xC1A00000, 0x46043000, 0x43FA0000,
    0x43480000, 0x00000000, 0x00000000, 0x00000000,
    0x468DA400, 0x3F000000, 0x46A5DC00, 0x00000000,
    0x0000001F, 0x4570A000, 0xC1A00000, 0x45DAC000,
    0x43960000, 0x43960000, 0x00000000, 0x00000000,
    0x00000000, 0x465AC000, 0x42740000, 0x4649E000,
    0x0000C000, 0x00000000
};

stage_definitions D_stage010_09D5E3E8 = {
    0, 0, 0,
    (u8 *)D_stage010_09D5E200,
    (u16 *)D_stage010_09D5E298,
    0xFFFF,
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    (stage_definitions_0x28_t *)D_stage010_09D5E208,
    (stage_definitions_0x2C_t *)D_stage010_09D5E238,
    (stage_exit *)D_stage010_09D5E2E0,
    NULL,
    NULL,
    0, 5, 3, 0, 0, 0, 0
};

u32 padding_09D5E42C = 0;

stage_draw_command D_stage010_09D5E430[4] = {
    {1, 6, 0, 0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2},
    {1, 0, 0x80, 3}
};

stage_draw_commands D_stage010_09D5E450 = {
    4, 0, D_stage010_09D5E430, NULL
};

u32 padding_09D5E45C = 0;

u32 D_stage010_09D5E460[16] = {
    0x43C80000, 0x00000000, 0x00000000, 0x00000000,
    0xC3C80000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x43C80000, 0x00000000,
    0x00000000, 0x00000000, 0xC3C80000, 0x00000000
};

// This 0x60-byte record contains a pointer at offset 0x14.
struct Stage010PropRecord {
    u32 header[5];
    u32 *positions;
    u32 reserved[18];
};

Stage010PropRecord D_stage010_09D5E4A0 = {
    {0x459F6000, 0, 0x45EA6000, 0, 0x00040000},
    D_stage010_09D5E460,
    {}
};

Stage010::Stage010() {
}

Stage010::~Stage010() {
}

void Stage010::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B93DC((prop_params *)&D_stage010_09D5E4A0);
    StageBase::vtable_0x24();
}

stage_definitions *Stage010::definitions() {
    return &D_stage010_09D5E3E8;
}

void Stage010::operator delete(void *) {
}

stage_draw_commands *Stage010::vtable_0x48() {
    return &D_stage010_09D5E450;
}

bool Stage010::vtable_0xA0() {
    return true;
}

bool Stage010::vtable_0xA4() {
    return false;
}

bool Stage010::vtable_0xA8() {
    return false;
}

int Stage010::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage010::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage010::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage010::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
