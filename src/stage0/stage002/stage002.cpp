#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

struct Stage002 : StageBase {
    Stage002();
    virtual ~Stage002();

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

// The unknown vector and exit fields retain their original 32-bit patterns.
u32 D_stage002_09D5E200[2] = {0, 1};

stage_definitions_0x28_t D_stage002_09D5E208[2] = {
    {{0, 0, 0}, 0, 0, 0, 0},
    {{10000.0f, 0, 10000.0f}, 11900.0f, 12000.0f, 1, 2}
};

u32 D_stage002_09D5E238[24] = {
    0x3F000000, 0x3F000000, 0x3F000000, 0x00000000,
    0x3F800000, 0x3F800000, 0x3F800000, 0x00000000,
    0x3E8F5C29, 0x3E99999A, 0x3EA3D70A, 0x00000000,
    0x3E8F5C29, 0x3E99999A, 0x3EA3D70A, 0x00000000,
    0x3DA3D70A, 0x3DCCCCCD, 0x3DF5C28F, 0x00000000,
    0x3E3851EC, 0x3E4CCCCD, 0x3E6147AE, 0x00000000
};

u32 D_stage002_09D5E298[42] = {
    0x00000001, 0x45D42000, 0xC47A0000, 0x464A0000,
    0x44BB8000, 0x44FA0000, 0x00000000, 0x00000000,
    0x00000000, 0x464FD000, 0xC1600000, 0x46309000,
    0x0000C000, 0x00000003, 0x468C7000, 0x00000000,
    0x45A46800, 0x44BB8000, 0x44FA0000, 0x00000000,
    0x00000000, 0x00000000, 0x46610000, 0xC1200000,
    0x46629000, 0x00008AAB, 0x00000013, 0x461C8C00,
    0x00000000, 0x444DC000, 0x44BB8000, 0x44FA0000,
    0x00000000, 0x00000000, 0x00000000, 0x4641C000,
    0xC3340000, 0x467A0000, 0x00008000, 0x00000000,
    0x00000000, 0x00000000
};

u32 D_stage002_09D5E340[16] = {
    0x0000002A, 0x00000006, 0x0000004D, 0x00000018,
    0x460CA133, 0x448BD333, 0x43480000, 0x00000000,
    0x0000002A, 0x00000006, 0x0000004D, 0x00000009,
    0x46081933, 0x448BD333, 0x45CB2000, 0x00000000
};

stage_definitions D_stage002_09D5E380 = {
    0, 1, 0,
    (u8 *)D_stage002_09D5E200,
    NULL,
    0,
    {0x0F, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    D_stage002_09D5E208,
    (stage_definitions_0x2C_t *)D_stage002_09D5E238,
    (stage_exit *)D_stage002_09D5E298,
    (stage_sound *)D_stage002_09D5E340,
    NULL,
    0, 3, 0, 2, 0, 0, 0
};

u8 padding_09D5E3C4[0xC] = {};

vtable_0x54_params D_stage002_09D5E3D0 = {
    2, 0x2000, {10000, 0, 10000, 0}
};

stage_draw_command D_stage002_09D5E3F0[7] = {
    {2, 6, 0, 0, &D_stage002_09D5E3D0},
    {1, 0, 0x80, 2},
    {1, 0, 0x80, 3},
    {1, 0, 0x80, 4},
    {1, 0, 0x80, 5},
    {1, 0, 0x80, 6},
    {1, 0, 0x80, 7}
};

stage_draw_commands D_stage002_09D5E428 = {
    7, 0, D_stage002_09D5E3F0, NULL
};

u8 padding_09D5E434[4] = {};

prop_params D_stage002_09D5E438 = {
    {0xF, 7, 0, 0},
    {0, 65535.0f, 65535.0f, 0}
};

u32 D_stage002_09D5E450[8] = {
    0xFF08FFFF, 0x09FFFFFF, 0xFFFF0607, 0x0EFF050A,
    0xFF0B0505, 0xFFFF0C0D, 0x000000FF, 0x00000000
};

struct Stage002PropData {
    u32 values[7];
    u32 *lookup;
    u32 unused[28];
};

Stage002PropData D_stage002_09D5E470 = {
    {0x45FA0000, 0x457A0000, 0x44FA0000, 0x44FA0000,
     0x4530C000, 0x457A0000, 0x00050005},
    D_stage002_09D5E450,
    {}
};

Stage002::Stage002() {
}

Stage002::~Stage002() {
}

#if defined(BUILD_NONMATCHING)
void Stage002::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage002_09D5E438);
    StageManager::objectPtr->push_prop_089B94BC(NULL, (u32)&D_stage002_09D5E470);
    StageBase::vtable_0x24();
}
#else
INCLUDE_ASM("asm/stage0/stage002/nonmatchings/stage002", vtable_0x24__8Stage002Fv);
#endif

stage_definitions *Stage002::definitions() {
    return &D_stage002_09D5E380;
}

void Stage002::operator delete(void *) {
}

stage_draw_commands *Stage002::vtable_0x48() {
    return &D_stage002_09D5E428;
}

bool Stage002::vtable_0xA4() {
    return true;
}

bool Stage002::vtable_0xA8() {
    return true;
}

bool Stage002::vtable_0xA0() {
    return true;
}

int Stage002::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage002::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage002::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage002::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
