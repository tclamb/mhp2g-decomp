#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage015_09D5E248;
extern stage_draw_commands D_stage015_09D5E320;

struct Stage015 : StageBase {
    Stage015();
    virtual ~Stage015();
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

Stage015::Stage015() {}
Stage015::~Stage015() {}
void Stage015::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage015::definitions() { return &D_stage015_09D5E248; }
void Stage015::operator delete(void *) {}
stage_draw_commands *Stage015::vtable_0x48() { return &D_stage015_09D5E320; }
