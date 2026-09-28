#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern stage_definitions D_stage162_09D5E340;
extern stage_draw_commands D_stage162_09D5EAA8;
extern prop_params D_stage162_09D5EB20;

struct Stage162 : StageBase {
    Stage162();
    virtual ~Stage162();
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

Stage162::Stage162() {}
Stage162::~Stage162() {}
void Stage162::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B961C(&D_stage162_09D5EB20);
    StageBase::vtable_0x24();
}
stage_definitions *Stage162::definitions() { return &D_stage162_09D5E340; }
void Stage162::operator delete(void *) {}
stage_draw_commands *Stage162::vtable_0x48() { return &D_stage162_09D5EAA8; }
