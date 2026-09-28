#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern u8 D_stage009_09D5E8F0[0x10];
extern u8 D_stage009_09D5E900[0x10];
extern u8 D_stage009_09D5E910[0x70];

struct Stage009 : StageBase {
    Stage009();
    virtual ~Stage009();

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

u32 D_stage009_09D5E280[2] = {0, 0x20};

// Two 0x34-byte exits; their remaining fields are not identified yet.
u32 D_stage009_09D5E288[26] = {
    0x0000009F, 0x45D1C000, 0x42DC0000, 0x45F97800,
    0x44160000, 0x44BB8000, 0x00000000, 0x00000000,
    0x00000000, 0x462ED800, 0xC1200000, 0x45F6E000,
    0x0000EAAB, 0x00000099, 0x46151400, 0xC1200000,
    0x4648DC00, 0x442F0000, 0x44BB8000, 0x00000000,
    0x00000000, 0x00000000, 0x463EA000, 0xC1880000,
    0x462F0000, 0x0000AAAB
};

stage_definitions D_stage009_09D5E2F0 = {
    0, 0, 0,
    (u8 *)D_stage009_09D5E280,
    NULL,
    0xFFFF,
    {0x03, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    NULL,
    NULL,
    (stage_exit *)D_stage009_09D5E288,
    NULL,
    NULL,
    0, 2, 0, 0, 0, 0, 0
};

u8 padding_09D5E334[4] = {};

stage_draw_command D_stage009_09D5E338[11] = {
    {1, 4, 0x80, 0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2},
    {1, 0, 0x80, 3},
    {1, 0, 0, 4},
    {1, 0, 0x80, 5},
    {1, 0, 0x80, 6},
    {1, 0, 0x80, 7},
    {1, 0, 0x80, 8},
    {1, 0, 0x80, 9},
    {1, 0, 0x80, 10}
};

stage_draw_commands D_stage009_09D5E390 = {
    11, 0, D_stage009_09D5E338, NULL
};

u8 padding_09D5E39C[4] = {};

prop_params D_stage009_09D5E3A0 = {
    {3, 2, 0, 0},
    {0, 65535.0f, 65535.0f, 0}
};

u8 padding_09D5E3B8[0x48] = {};

Stage009::Stage009() {
}

Stage009::~Stage009() {
}

void Stage009::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage009_09D5E3A0);
    StageManager::objectPtr->push_prop_089B94FC(
        0, (ScePspFVector4 *)D_stage009_09D5E8F0, 0x200, 0, 0, 0, 1);
    StageManager::objectPtr->push_prop_089B94FC(
        0x10, (ScePspFVector4 *)D_stage009_09D5E900, 0, 0, 0, 1, 1);
    StageManager::objectPtr->push_prop_089B94FC(
        0, (ScePspFVector4 *)D_stage009_09D5E910, 0, 0x80, 0, 2, 1);
    StageBase::vtable_0x24();
}

stage_definitions *Stage009::definitions() {
    return &D_stage009_09D5E2F0;
}

void Stage009::operator delete(void *) {
}

stage_draw_commands *Stage009::vtable_0x48() {
    return &D_stage009_09D5E390;
}

bool Stage009::vtable_0xA4() {
    return true;
}

bool Stage009::vtable_0xA8() {
    return true;
}

bool Stage009::vtable_0xA0() {
    return true;
}

int Stage009::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage009::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage009::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage009::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
