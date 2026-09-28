#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern stage_definitions D_stage101_09D5E360;
extern stage_draw_commands D_stage101_09D5E420;
extern prop_params D_stage101_09D5E430;

struct Stage101 : StageBase {
    Stage101();
    virtual ~Stage101();
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

Stage101::Stage101() {}
Stage101::~Stage101() {}
void Stage101::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B927C(&D_stage101_09D5E430);
    StageBase::vtable_0x24();
}
stage_definitions *Stage101::definitions() { return &D_stage101_09D5E360; }
void Stage101::operator delete(void *) {}
stage_draw_commands *Stage101::vtable_0x48() { return &D_stage101_09D5E420; }
