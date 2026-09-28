#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern prop_params D_stage078_09D5E530;
extern prop_params D_stage078_09D5E5A0;
extern stage_definitions D_stage078_09D5E3D0;
extern stage_draw_commands D_stage078_09D5E520;

struct Stage078 : StageBase {
    Stage078();
    virtual ~Stage078();
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

Stage078::Stage078() {}
Stage078::~Stage078() {}
void Stage078::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage078_09D5E530);
    StageManager::objectPtr->push_prop_089B961C(&D_stage078_09D5E5A0);
    StageBase::vtable_0x24();
}
stage_definitions *Stage078::definitions() { return &D_stage078_09D5E3D0; }
void Stage078::operator delete(void *) {}
stage_draw_commands *Stage078::vtable_0x48() { return &D_stage078_09D5E520; }
