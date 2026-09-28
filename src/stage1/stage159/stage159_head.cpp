#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern prop_params D_stage159_09D5E3E8;
extern prop_params D_stage159_09D5E440;
extern stage_definitions D_stage159_09D5E320;
extern stage_draw_commands D_stage159_09D5E3D8;

struct Stage159 : StageBase {
    Stage159();
    virtual ~Stage159();
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

Stage159::Stage159() {}
Stage159::~Stage159() {}
void Stage159::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage159_09D5E3E8);
    StageManager::objectPtr->push_prop_089B95FC(&D_stage159_09D5E440);
    StageBase::vtable_0x24();
}
stage_definitions *Stage159::definitions() { return &D_stage159_09D5E320; }
void Stage159::operator delete(void *) {}
stage_draw_commands *Stage159::vtable_0x48() { return &D_stage159_09D5E3D8; }
