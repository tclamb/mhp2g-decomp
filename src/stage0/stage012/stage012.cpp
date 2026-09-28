#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern u8 D_stage012_09D5E970[0x10];

struct Stage012 : StageBase {
    Stage012();
    virtual ~Stage012();

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

// Unknown records retain their original 32-bit patterns.
u32 D_stage012_09D5E280[30] = {
    0x001F0000, 0x45CCD800, 0x44D48000, 0x463FB800,
    0x43480000, 0x00000000, 0x001F0001, 0x45FA1800,
    0x44D48000, 0x463FB800, 0x43480000, 0x00010000,
    0x00220000, 0x45D93000, 0x44D48000, 0x463EA000,
    0x43480000, 0x00000000, 0x00220001, 0x45EE2000,
    0x44D48000, 0x463EA000, 0x43480000, 0x00000000,
    0x00230000, 0x45E3D000, 0x44D48000, 0x463AE000,
    0x43340000, 0x00000000
};

u32 D_stage012_09D5E2F8[14] = {
    0x0000001F, 0x46040800, 0x44C80000, 0x46354000,
    0x43480000, 0x43480000, 0x00000000, 0x00000000,
    0x00000000, 0x46261800, 0x42740000, 0x4624D800,
    0x0000199A, 0x00000000
};

stage_definitions D_stage012_09D5E330 = {
    0, 1, 0,
    D_stage012_09D5E970,
    (u16 *)D_stage012_09D5E280,
    0xFFFF,
    {0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    NULL,
    NULL,
    (stage_exit *)D_stage012_09D5E2F8,
    NULL,
    NULL,
    0, 1, 5, 0, 0, 0, 0
};

u8 padding_09D5E374[4] = {};

stage_draw_command D_stage012_09D5E378[3] = {
    {1, 6, 0x80, 0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2}
};

stage_draw_commands D_stage012_09D5E390 = {
    3, 0, D_stage012_09D5E378, NULL
};

u8 padding_09D5E39C[4] = {};

u32 D_stage012_09D5E3A0[16] = {
    0x45CCF800, 0x44148000, 0x4644B800, 0x00000000,
    0x45FA2800, 0x44148000, 0x4644B800, 0x00000000,
    0x45EF5000, 0x44854000, 0x46412C00, 0x00000000,
    0x45D7E000, 0x44854000, 0x46412C00, 0x00000000
};

struct Stage012PropData {
    u32 values[10];
    u32 *vector_data;
    u32 unused;
};

Stage012PropData D_stage012_09D5E3E0 = {
    {0x00030000, 0x00000007, 0x00000000, 0x00000000,
     0x45E42000, 0x00000000, 0x463B8000, 0x00000000,
     0x04020000, 0x3F800000},
    D_stage012_09D5E3A0,
    0
};

u32 D_stage012_09D5E410[4] = {
    0x45866000, 0x00000000, 0x46BD1000, 0x00000000
};

u32 D_stage012_09D5E420[8] = {
    0x00000000, 0x00060000, 0x0000543A, 0x00000000,
    0x45CCD800, 0x44D4E000, 0x463FB800, 0x00000000
};

u32 D_stage012_09D5E440[16] = {
    0x00000000, 0x00060000, 0x0000553B, 0x00000000,
    0x45FA1800, 0x44D4E000, 0x463FB800, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

Stage012::Stage012() {
}

Stage012::~Stage012() {
}

void Stage012::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B933C((prop_params *)&D_stage012_09D5E3E0);
    StageManager::objectPtr->push_prop_089B94FC(
        0, (ScePspFVector4 *)D_stage012_09D5E410, 0x400, 0, 0, 5, 1);
    StageManager::objectPtr->push_prop_089B965C((prop_params *)D_stage012_09D5E420);
    StageManager::objectPtr->push_prop_089B965C((prop_params *)D_stage012_09D5E440);
    StageBase::vtable_0x24();
}

stage_definitions *Stage012::definitions() {
    return &D_stage012_09D5E330;
}

void Stage012::operator delete(void *) {
}

stage_draw_commands *Stage012::vtable_0x48() {
    return &D_stage012_09D5E390;
}

bool Stage012::vtable_0xA0() {
    return true;
}

bool Stage012::vtable_0xA4() {
    return false;
}

bool Stage012::vtable_0xA8() {
    return false;
}

int Stage012::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage012::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage012::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage012::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
