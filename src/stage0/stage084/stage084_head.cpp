#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage084_09D5E280;
extern stage_draw_commands D_stage084_09D5E380;

struct Stage084 : StageBase {
    Stage084();
    virtual ~Stage084();
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

Stage084::Stage084() {}
Stage084::~Stage084() {}
void Stage084::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage084::definitions() { return &D_stage084_09D5E280; }
void Stage084::operator delete(void *) {}
stage_draw_commands *Stage084::vtable_0x48() { return &D_stage084_09D5E380; }
