#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage123_09D5E2E0;
extern stage_draw_commands D_stage123_09D5E388;

struct Stage123 : StageBase {
    Stage123();
    virtual ~Stage123();
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

Stage123::Stage123() {}
Stage123::~Stage123() {}
void Stage123::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage123::definitions() { return &D_stage123_09D5E2E0; }
void Stage123::operator delete(void *) {}
stage_draw_commands *Stage123::vtable_0x48() { return &D_stage123_09D5E388; }
