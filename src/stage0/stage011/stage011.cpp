#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern u8 D_stage011_09D5E870[0x10];

struct Stage011 : StageBase {
    Stage011();
    virtual ~Stage011();

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
u32 D_stage011_09D5E200[14] = {
    0x0000000A, 0x467B9000, 0xC2480000, 0x46B90000,
    0x43480000, 0x43480000, 0x00000000, 0x00000000,
    0x00000000, 0x458E3000, 0x40200000, 0x45FB9000,
    0x00004000, 0x00000000
};

stage_definitions D_stage011_09D5E238 = {
    0, 1, 0,
    D_stage011_09D5E870,
    NULL,
    0xFFFF,
    {0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    NULL, NULL, (stage_exit *)D_stage011_09D5E200, NULL, NULL,
    0, 1, 0, 0, 0, 0, 0
};

u8 padding_09D5E27C[4] = {};

struct Stage011ModelBlock { stage_draw_command value[3]; u8 pad[8]; };
Stage011ModelBlock D_stage011_09D5E280 = {{
    {1, 6, 0x80, 0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2}
}, {0}};

u32 D_stage011_09D5E2A0[8] = {
    0x00000002, 0x3FC90FDB, 0x00000000, 0x00000000,
    0x4639F000, 0x45480000, 0x46A5A000, 0x00000000
};

u32 D_stage011_09D5E2C0[8] = {
    0x00000002, 0x3FE231D6, 0x00000000, 0x00000000,
    0x4640F800, 0x45480000, 0x46D48000, 0x00000000
};

u32 D_stage011_09D5E2E0[8] = {
    0x00000002, 0x40490FDB, 0x00000000, 0x00000000,
    0x4688B800, 0x454E4000, 0x46F61800, 0x00000000
};

stage_draw_command D_stage011_09D5E300[3] = {
    {4, 0, 0x80, 2, D_stage011_09D5E2A0},
    {4, 0, 0x80, 2, D_stage011_09D5E2C0},
    {4, 0, 0x80, 2, D_stage011_09D5E2E0}
};

stage_draw_commands D_stage011_09D5E318 = {
    3, 3, D_stage011_09D5E280.value, D_stage011_09D5E300
};

u8 padding_09D5E324[4] = {};

u32 D_stage011_09D5E328[6] = {
    0x00080000, 0x00000000, 0x00000000, 0x477FFF00,
    0x477FFF00, 0x00000000
};

u32 D_stage011_09D5E340[16] = {
    0x46AB1800, 0x00000000, 0x46872800, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

Stage011::Stage011() {
}

Stage011::~Stage011() {
}

void Stage011::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C((prop_params *)D_stage011_09D5E328);
    StageManager::objectPtr->push_prop_089B94FC(0, (ScePspFVector4 *)D_stage011_09D5E340, 0x400, 0, 0, 3, 1);
    StageBase::vtable_0x24();
}

stage_definitions *Stage011::definitions() {
    return &D_stage011_09D5E238;
}

void Stage011::operator delete(void *) {
}

stage_draw_commands *Stage011::vtable_0x48() {
    return &D_stage011_09D5E318;
}

bool Stage011::vtable_0xA0() {
    return true;
}

bool Stage011::vtable_0xA4() {
    return false;
}

bool Stage011::vtable_0xA8() {
    return false;
}

int Stage011::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage011::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage011::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage011::vtable_0xB8() {
    return definitions()->unknown_0x38;
}

