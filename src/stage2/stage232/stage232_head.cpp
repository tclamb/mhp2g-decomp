#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern prop_params D_stage232_09D5E2D0;
extern prop_params D_stage232_09D5E330;
extern stage_definitions D_stage232_09D5E260;
extern stage_draw_commands D_stage232_09D5E2C0;

struct Stage232 : StageBase {
    Stage232();
    virtual ~Stage232();
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

Stage232::Stage232() {}
Stage232::~Stage232() {}
void Stage232::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage232_09D5E2D0);
    StageManager::objectPtr->push_prop_089B96FC(&D_stage232_09D5E330);
    StageBase::vtable_0x24();
}
stage_definitions *Stage232::definitions() { return &D_stage232_09D5E260; }
void Stage232::operator delete(void *) {}
stage_draw_commands *Stage232::vtable_0x48() { return &D_stage232_09D5E2C0; }
