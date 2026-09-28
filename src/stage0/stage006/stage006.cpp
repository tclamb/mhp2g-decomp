#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern u8 D_stage006_09D5E870[0x10];

struct Stage006 : StageBase {
    Stage006();
    virtual ~Stage006();

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

// The fields of this exit have not been identified; retain their exact words.
u32 D_stage006_09D5E200[13] = {
    0x0000005F, 0x45E74000, 0x44548000, 0x4636D000,
    0x43FA0000, 0x43FA0000, 0x00000000, 0x00000000,
    0x00000000, 0x463F6800, 0x455AC000, 0x45F23000,
    0x0000C000
};

u32 padding_09D5E234 = 0;

stage_definitions D_stage006_09D5E238 = {
    0, 1, 0,
    D_stage006_09D5E870,
    NULL,
    0xFFFF,
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0x09, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},
    7,
    NULL,
    NULL,
    (stage_exit *)D_stage006_09D5E200,
    NULL,
    NULL,
    0, 1, 0, 0, 0, 0, 0
};

u32 padding_09D5E27C = 0;

stage_draw_command D_stage006_09D5E280[10] = {
    {1, 6, 0x80, 0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2},
    {1, 0, 0x80, 3},
    {1, 0, 0x80, 4},
    {1, 0, 0x80, 5},
    {1, 0, 0x80, 6},
    {1, 0, 0x80, 7},
    {1, 0, 0, 8},
    {1, 0, 0x80, 9}
};

struct Stage006UvWords { u32 first[3]; u32 second[3]; u32 third[2]; };
Stage006UvWords D_stage006_09D5E2D0 = {
    {0x08000000, 0, 0x00000800},
    {0x02000000, 0, 0x00000200},
    {0x00400000, 0}
};

stage_draw_command D_stage006_09D5E2F0[5] = {
    {6, 2, 0, 0, D_stage006_09D5E2D0.first},
    {6, 2, 0, 1, (u8 *)D_stage006_09D5E2D0.first + 6},
    {6, 0, 0, 2, D_stage006_09D5E2D0.second},
    {6, 0, 0, 3, (u8 *)D_stage006_09D5E2D0.second + 6},
    {6, 0, 0, 8, D_stage006_09D5E2D0.third}
};

struct Stage006DrawBlock { stage_draw_commands value; u8 pad[8]; };
Stage006DrawBlock D_stage006_09D5E318 = {{
    10, 5, D_stage006_09D5E280, D_stage006_09D5E2F0
}, {0}};

// Two props use the same 0x50-byte parameter block.
u32 D_stage006_09D5E330[20] = {
    0x45CB2000, 0xC2C80000, 0x46473800, 0x00000000,
    0x460CA000, 0x44FA0000, 0x466A6000, 0x00000000,
    0x45CB2000, 0x43160000, 0x46473800, 0x00000000,
    0x00000800, 0x00000000, 0x00000800, 0x00000000,
    0x00000009, 0x00000000, 0x00000000, 0x00000000
};

Stage006::Stage006() {
}

Stage006::~Stage006() {
}

void Stage006::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B95DC((prop_params *)D_stage006_09D5E330);
    StageManager::objectPtr->push_prop_089B95DC((prop_params *)D_stage006_09D5E330);
    StageBase::vtable_0x24();
}

stage_definitions *Stage006::definitions() {
    return &D_stage006_09D5E238;
}

void Stage006::operator delete(void *) {
}

stage_draw_commands *Stage006::vtable_0x48() {
    return &D_stage006_09D5E318.value;
}

bool Stage006::vtable_0xA0() {
    return true;
}

bool Stage006::vtable_0xA4() {
    return false;
}

bool Stage006::vtable_0xA8() {
    return false;
}

int Stage006::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage006::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage006::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage006::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
