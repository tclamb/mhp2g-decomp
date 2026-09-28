#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern prop_params D_stage079_09D5E4C0;
extern prop_params D_stage079_09D5E540;
extern stage_definitions D_stage079_09D5E370;
extern stage_draw_commands D_stage079_09D5E4B0;

struct Stage079 : StageBase {
    Stage079();
    virtual ~Stage079();
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

Stage079::Stage079() {}
Stage079::~Stage079() {}
void Stage079::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage079_09D5E4C0);
    StageManager::objectPtr->push_prop_089B961C(&D_stage079_09D5E540);
    StageBase::vtable_0x24();
}
stage_definitions *Stage079::definitions() { return &D_stage079_09D5E370; }
void Stage079::operator delete(void *) {}
stage_draw_commands *Stage079::vtable_0x48() { return &D_stage079_09D5E4B0; }
