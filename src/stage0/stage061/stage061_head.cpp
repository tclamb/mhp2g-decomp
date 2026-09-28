#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern stage_definitions D_stage061_09D5E268;
extern stage_draw_commands D_stage061_09D5E3C0;
extern prop_params D_stage061_09D5E3D0;

struct Stage061 : StageBase {
    Stage061();
    virtual ~Stage061();
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

Stage061::Stage061() {}
Stage061::~Stage061() {}
void Stage061::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B927C(&D_stage061_09D5E3D0);
    StageBase::vtable_0x24();
}
stage_definitions *Stage061::definitions() { return &D_stage061_09D5E268; }
void Stage061::operator delete(void *) {}
stage_draw_commands *Stage061::vtable_0x48() { return &D_stage061_09D5E3C0; }
