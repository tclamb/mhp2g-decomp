#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern stage_definitions D_stage018_09D5E3F0;
extern stage_draw_commands D_stage018_09D5E558;
extern prop_params D_stage018_09D5E568;

struct Stage018 : StageBase {
    Stage018();
    virtual ~Stage018();
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

Stage018::Stage018() {}
Stage018::~Stage018() {}
void Stage018::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage018_09D5E568);
    StageBase::vtable_0x24();
}
stage_definitions *Stage018::definitions() { return &D_stage018_09D5E3F0; }
void Stage018::operator delete(void *) {}
stage_draw_commands *Stage018::vtable_0x48() { return &D_stage018_09D5E558; }
