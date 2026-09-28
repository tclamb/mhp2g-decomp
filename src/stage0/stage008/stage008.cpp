#include "common.h"
#include "stage_base.hpp"

extern u8 D_stage008_09D5E870[0x10];

struct Stage008 : StageBase {
    Stage008();
    virtual ~Stage008();

    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual stage_exit *exits(u32 map_id);
    virtual s8 exit_count(u32 map_id);
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

stage_exit D_stage008_09D5E200[1] = {
    {0x58, 0, {13250, -100, 8550}, 500, 2000, {},
     {10100, -100, 14700}, {0, 0x80, 0, 0}}
};

u8 padding_09D5E234[4] = {};

stage_exit D_stage008_09D5E238[1] = {
    {0xF6, 0, {13250, -100, 8550}, 500, 2000, {},
     {10100, 0, 14700}, {0, 0x80, 0, 0}}
};

u8 padding_09D5E26C[4] = {};

stage_definitions D_stage008_09D5E270 = {
    0, 0, 0,
    D_stage008_09D5E870,
    NULL,
    0xFFFF,
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    NULL, NULL, NULL, NULL, NULL,
    0, 0, 0, 0, 0, 0, 0,
};

u8 padding_09D5E2B4[0xC] = {};

vtable_0x54_params D_stage008_09D5E2C0[1] = {
    {2, 0x2000, {13600, 0, 10600, 0}}
};

vtable_0x64_params D_stage008_09D5E2E0[2] = {
    {0, 0x400, 0},
    {0, 0x200, 0}
};

u8 padding_09D5E2EC[4] = {};

stage_draw_command D_stage008_09D5E2F0[5] = {
    {2, 6, 0x80, 0, &D_stage008_09D5E2C0[0]},
    {6, 0, 0, 1, &D_stage008_09D5E2E0[0]},
    {6, 0, 0, 2, &D_stage008_09D5E2E0[1]},
    {1, 0, 0x80, 3},
    {1, 0, 0x80, 4}
};

stage_draw_commands D_stage008_09D5E318 = {
    5, 0, D_stage008_09D5E2F0, NULL
};

u8 padding_09D5E324[0x5C] = {};

Stage008::Stage008() {
}

Stage008::~Stage008() {
}

void Stage008::vtable_0x24() {
    StageBase::vtable_0x24();
}

stage_definitions *Stage008::definitions() {
    return &D_stage008_09D5E270;
}

#if defined(BUILD_NONMATCHING)
stage_exit *Stage008::exits(u32 map_id) {
    if (map_id == 0x1D) {
        return D_stage008_09D5E238;
    }
    if (map_id == 8) {
        return D_stage008_09D5E200;
    }
    return NULL;
}

s8 Stage008::exit_count(u32 map_id) {
    if (map_id == 0x1D || map_id == 8) {
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/stage0/stage008/nonmatchings/stage008", exits__8Stage008FUi);
INCLUDE_ASM("asm/stage0/stage008/nonmatchings/stage008", exit_count__8Stage008FUi);
#endif

void Stage008::operator delete(void *) {
}

stage_draw_commands *Stage008::vtable_0x48() {
    return &D_stage008_09D5E318;
}

bool Stage008::vtable_0xA0() {
    return true;
}

bool Stage008::vtable_0xA4() {
    return false;
}

bool Stage008::vtable_0xA8() {
    return false;
}

int Stage008::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage008::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage008::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage008::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
