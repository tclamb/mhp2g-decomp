#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

struct Stage003 : StageBase {
    Stage003();
    virtual ~Stage003();

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
u32 D_stage003_09D5E200[2] = {0, 1};

stage_definitions_0x28_t D_stage003_09D5E208[2] = {
    {{0, 0, 0}, 0, 0, 0, 0},
    {{10000.0f, 0, 10000.0f}, 13000.0f, 13000.0f, 1, 1}
};

u32 D_stage003_09D5E238[16] = {
    0x3F19999A, 0x3F19999A, 0x3F19999A, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000,
    0x3ECCCCCD, 0x3ECCCCCD, 0x3ECCCCCD, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000
};

u32 D_stage003_09D5E278[6] = {
    0x00190000, 0x467F7800, 0x00000000, 0x46108800,
    0x43960000, 0x00002F00
};

u32 D_stage003_09D5E290[40] = {
    0x00000002, 0x46723000, 0xC3A50000, 0x46827800,
    0x44960000, 0x44BB8000, 0x00000000, 0x00000000,
    0x00000000, 0x46688C00, 0x00000000, 0x45D64000,
    0x0000D8E4, 0x00000004, 0x46040800, 0xC3960000,
    0x45C99000, 0x447A0000, 0x44BB8000, 0x00000000,
    0x00000000, 0x00000000, 0x4668D000, 0x00000000,
    0x46147000, 0x0000C000, 0x00000012, 0x462A5000,
    0x43700000, 0x4688B800, 0x44480000, 0x44BB8000,
    0x00000000, 0x00000000, 0x00000000, 0x46610000,
    0xC3200000, 0x460CA000, 0x0000C71C, 0x00000000
};

stage_definitions D_stage003_09D5E330 = {
    0, 0, 0,
    (u8 *)D_stage003_09D5E200,
    (u16 *)D_stage003_09D5E278,
    2,
    {0x06, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    5,
    D_stage003_09D5E208,
    (stage_definitions_0x2C_t *)D_stage003_09D5E238,
    (stage_exit *)D_stage003_09D5E290,
    NULL,
    NULL,
    0, 3, 1, 0, 0, 0, 0
};

u8 padding_09D5E374[0xC] = {};

vtable_0x54_params D_stage003_09D5E380 = {
    2, 0x2000, {10700, 0, 9600, 0}
};

u32 D_stage003_09D5E3A0[2] = {0x01000000, 0};

stage_draw_command D_stage003_09D5E3A8[11] = {
    {1, 4, 0x80, 0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2},
    {2, 0, 0, 3, &D_stage003_09D5E380},
    {1, 0, 0x80, 4},
    {1, 0, 0x80, 5},
    {1, 0, 0xA0, 6},
    {1, 0, 0x80, 7},
    {1, 0, 0x80, 8},
    {6, 0, 0, 9, D_stage003_09D5E3A0},
    {0, 0, 0, 0}
};

u32 D_stage003_09D5E400[4] = {
    0x462F0000, 0x00000000, 0x461C4000, 0x00000000
};

u32 D_stage003_09D5E410[4] = {
    0x464CB000, 0x00000000, 0x461C4000, 0x00000000
};

u32 D_stage003_09D5E420[4] = {
    0x462D7000, 0x00000000, 0x463B8000, 0x00000000
};

u32 D_stage003_09D5E430[4] = {
    0x464CB000, 0x00000000, 0x463B8000, 0x00000000
};

stage_draw_command D_stage003_09D5E440[4] = {
    {3, 0, 0x80, 1, D_stage003_09D5E400},
    {3, 0, 0x80, 1, D_stage003_09D5E410},
    {3, 0, 0x80, 1, D_stage003_09D5E420},
    {3, 0, 0x80, 1, D_stage003_09D5E430}
};

stage_draw_commands D_stage003_09D5E460 = {
    10, 4, D_stage003_09D5E3A8, D_stage003_09D5E440
};

u8 padding_09D5E46C[4] = {};

prop_params D_stage003_09D5E470 = {
    {6, 7, 0, 0},
    {0, 65535.0f, 65535.0f, 0}
};

u8 padding_09D5E488[0x78] = {};

Stage003::Stage003() {
}

Stage003::~Stage003() {
}

void Stage003::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage003_09D5E470);
    StageBase::vtable_0x24();
}

stage_definitions *Stage003::definitions() {
    return &D_stage003_09D5E330;
}

void Stage003::operator delete(void *) {
}

stage_draw_commands *Stage003::vtable_0x48() {
    return &D_stage003_09D5E460;
}

bool Stage003::vtable_0xA4() {
    return true;
}

bool Stage003::vtable_0xA8() {
    return true;
}

bool Stage003::vtable_0xA0() {
    return true;
}

int Stage003::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage003::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage003::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage003::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
