#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern prop_params D_stage073_09D5E348;
extern prop_params D_stage073_09D5E3A0;
extern stage_definitions D_stage073_09D5E290;
extern stage_draw_commands D_stage073_09D5E338;

struct Stage073 : StageBase {
    Stage073();
    virtual ~Stage073();
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

Stage073::Stage073() {}
Stage073::~Stage073() {}
void Stage073::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage073_09D5E348);
    StageManager::objectPtr->push_prop_089B95FC(&D_stage073_09D5E3A0);
    StageBase::vtable_0x24();
}
stage_definitions *Stage073::definitions() { return &D_stage073_09D5E290; }
void Stage073::operator delete(void *) {}
stage_draw_commands *Stage073::vtable_0x48() { return &D_stage073_09D5E338; }
