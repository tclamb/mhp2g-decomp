#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage244_09D5E1E8;
extern stage_draw_commands D_stage244_09D5E308;

struct Stage244 : StageBase {
    Stage244();
    virtual ~Stage244();
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

Stage244::Stage244() {}
Stage244::~Stage244() {}
void Stage244::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage244::definitions() { return &D_stage244_09D5E1E8; }
void Stage244::operator delete(void *) {}
stage_draw_commands *Stage244::vtable_0x48() { return &D_stage244_09D5E308; }
