#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern stage_definitions D_stage043_09D5E250;
extern stage_draw_commands D_stage043_09D5E340;
extern prop_params D_stage043_09D5E350;

struct Stage043 : StageBase {
    Stage043();
    virtual ~Stage043();
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

Stage043::Stage043() {}
Stage043::~Stage043() {}
void Stage043::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B927C(&D_stage043_09D5E350);
    StageBase::vtable_0x24();
}
stage_definitions *Stage043::definitions() { return &D_stage043_09D5E250; }
void Stage043::operator delete(void *) {}
stage_draw_commands *Stage043::vtable_0x48() { return &D_stage043_09D5E340; }
