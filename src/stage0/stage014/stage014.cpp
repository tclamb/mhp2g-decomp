#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern u8 D_stage014_09D5E870[0x10];

// The call uses a mode word followed by a data pointer at the ABI level.
extern "C" void push_prop_089B931C__12StageManagerFP11prop_paramsUiUsUsUiUs(
    StageManager *, u32, void *, u16, u16, u32, u16
);

struct Stage014 : StageBase {
    Stage014();
    virtual ~Stage014();

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

// Unknown records remain word arrays so their original bits are preserved.
u32 D_stage014_09D5E200[16] = {
    0x0000000A, 0x46318000, 0x43BE0000, 0x46516000,
    0x43480000, 0x43480000, 0x00000000, 0x00000000,
    0x00000000, 0x45B22000, 0x41300000, 0x45DDE000,
    0x0000C000, 0x00000000, 0x00000000, 0x00000000
};

u32 D_stage014_09D5E240[8] = {
    0x00000002, 0x00001000, 0x00000000, 0x00000000,
    0x46386000, 0x00000000, 0x463B8000, 0x00000000
};

u32 D_stage014_09D5E260[8] = {
    0x00000002, 0x00000800, 0x00000000, 0x00000000,
    0x462D7000, 0x00000000, 0x465D4000, 0x00000000
};

stage_draw_command D_stage014_09D5E280[6] = {
    {2, 6, 0x80, 0, D_stage014_09D5E240},
    {1, 0, 0x80, 1},
    {2, 0, 0, 2, D_stage014_09D5E240},
    {1, 0, 0x80, 3},
    {2, 1, 0x80, 4, D_stage014_09D5E260},
    {1, 0, 0x80, 5}
};

stage_draw_commands D_stage014_09D5E2B0 = {
    6, 0, D_stage014_09D5E280, NULL
};

u8 padding_09D5E2BC[4] = {};

stage_definitions D_stage014_09D5E2C0 = {
    0, 0, 0,
    D_stage014_09D5E870,
    NULL,
    0xFFFF,
    {0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    NULL, NULL, (stage_exit *)D_stage014_09D5E200, NULL, NULL,
    0, 1, 0, 0, 0, 0, 0
};

u8 padding_09D5E304[0xC] = {};

u32 D_stage014_09D5E310[28] __attribute__((aligned(16))) = {
    0x46462000, 0x442E0000, 0x46475000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

Stage014::Stage014() {
}

Stage014::~Stage014() {
}

void Stage014::vtable_0x24() {
    push_prop_089B931C__12StageManagerFP11prop_paramsUiUsUsUiUs(
        StageManager::objectPtr, 2, D_stage014_09D5E310, 0x10, 2, 0, 0
    );
    StageBase::vtable_0x24();
}

stage_definitions *Stage014::definitions() {
    return &D_stage014_09D5E2C0;
}

void Stage014::operator delete(void *) {
}

stage_draw_commands *Stage014::vtable_0x48() {
    return &D_stage014_09D5E2B0;
}

bool Stage014::vtable_0xA0() {
    return true;
}

bool Stage014::vtable_0xA4() {
    return false;
}

bool Stage014::vtable_0xA8() {
    return false;
}

int Stage014::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage014::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage014::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage014::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
