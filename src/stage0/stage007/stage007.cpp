#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

struct Stage007 : StageBase {
    Stage007();
    virtual ~Stage007();

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

u32 D_stage007_09D5E200[2] = {0, 1};

stage_definitions_0x28_t D_stage007_09D5E208[2] = {
    {{0, 0, 0}, 0, 0, 0, 0},
    {{10000.0f, 0, 10000.0f}, 9000.0f, 10000.0f, 1, 2}
};

ScePspFVector2 D_stage007_09D5E238[3][4] = {
    {{1, 1}, {1, 0}, {0.2f, 0.2f}, {0.2f, 0}},
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
    {{0.5f, 0.5f}, {0.5f, 0}, {0.5f, 0.5f}, {0.5f, 0}}
};

stage_exit D_stage007_09D5E298[1] = {
    {0x91, 0, {13348, -100, 9710}, 400, 2000, {}, {8000, 0, 16600}, {0x39, 0x4E, 0, 0}}
};

u8 padding_09D5E2CC[0x4] = {};

stage_definitions D_stage007_09D5E2D0 = {
    0, 0, 0,
    (u8 *)D_stage007_09D5E200,
    NULL,
    0xFFFF,
    {0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    3,
    D_stage007_09D5E208,
    D_stage007_09D5E238,
    D_stage007_09D5E298,
    NULL,
    NULL,
    0, 1, 0, 0, 0, 0, 0
};

u8 padding_09D5E314[0x4] = {};

stage_draw_command D_stage007_09D5E318[6] = {
    {1, 6, 0x80, 0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2},
    {1, 0, 0, 3},
    {1, 0, 0x80, 4},
    {1, 0, 0, 5}
};

u8 padding_09D5E348[0x8] = {};

vtable_0x54_params D_stage007_09D5E350 = {2, 0x2000, {10000, 0, 10000, 0}};
vtable_0x54_params D_stage007_09D5E370 = {2, 0x800, {9500, 0, 13180, 0}};

stage_draw_command D_stage007_09D5E390[2] = {
    {2, 0, 0, 0, &D_stage007_09D5E350},
    {2, 0, 0x80, 2, &D_stage007_09D5E370}
};

stage_draw_commands D_stage007_09D5E3A0 = {
    sizeof(D_stage007_09D5E318) / sizeof(D_stage007_09D5E318[0]),
    sizeof(D_stage007_09D5E390) / sizeof(D_stage007_09D5E390[0]),
    D_stage007_09D5E318,
    D_stage007_09D5E390
};

u8 padding_09D5E3AC[0x4] = {};

// The fields of this 0x50-byte prop record are still unknown.
u32 D_stage007_09D5E3B0[0x14] = {
    0, 0, 0x477FFF00, 0x477FFF00, 0x3F333333, 1
};

Stage007::Stage007() {
}

Stage007::~Stage007() {
}

#if defined(BUILD_NONMATCHING)
void Stage007::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B92BC(0, (u32)D_stage007_09D5E3B0);
    StageBase::vtable_0x24();
}
#else
INCLUDE_ASM("asm/stage0/stage007/nonmatchings/stage007", vtable_0x24__8Stage007Fv);
#endif

stage_definitions *Stage007::definitions() {
    return &D_stage007_09D5E2D0;
}

void Stage007::operator delete(void *) {
}

stage_draw_commands *Stage007::vtable_0x48() {
    return &D_stage007_09D5E3A0;
}

bool Stage007::vtable_0xA0() {
    return true;
}

bool Stage007::vtable_0xA4() {
    return false;
}

bool Stage007::vtable_0xA8() {
    return false;
}

int Stage007::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage007::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage007::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage007::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
