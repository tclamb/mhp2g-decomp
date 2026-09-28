#include "common.h"
#include "stage_base.hpp"

extern u8 D_stage000_09D5E6F0[0x10];

struct Stage000 : StageBase {
    Stage000();
    virtual ~Stage000();

    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    static void operator delete(void *);
};

// Two 0x18-byte records, retained as halfwords until their fields are known.
u16 D_stage000_09D5E180[0x18] = {
    0x0000, 0x0017, 0xE800, 0x460F, 0x0000, 0xC1A0, 0x1800, 0x4603,
    0x0000, 0x43AF, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBF80,
    0x0000, 0x0000, 0x6800, 0x4621, 0x0000, 0x42A0, 0x0000, 0x0000,
};

stage_definitions D_stage000_09D5E1B0 = {
    0, 0, 0,
    D_stage000_09D5E6F0,
    D_stage000_09D5E180,
    0xFFFF,
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    NULL, NULL, NULL, NULL, NULL,
    0, 0, 0, 0, 0, 0, 0,
};

Stage000::Stage000() {
}

Stage000::~Stage000() {
}

void Stage000::vtable_0x24() {
    StageBase::vtable_0x24();
}

stage_definitions *Stage000::definitions() {
    return &D_stage000_09D5E1B0;
}

void Stage000::operator delete(void *) {
}

bool Stage000::vtable_0xA0() {
    return true;
}

bool Stage000::vtable_0xA4() {
    return false;
}

bool Stage000::vtable_0xA8() {
    return false;
}

int Stage000::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage000::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage000::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage000::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
