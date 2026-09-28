#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern prop_params D_stage069_09D5E300;
extern prop_params D_stage069_09D5E350;
extern prop_params D_stage069_09D5E3A0;
extern prop_params D_stage069_09D5E400;
extern stage_definitions D_stage069_09D5E268;
extern stage_draw_commands D_stage069_09D5E2E8;

struct Stage069 : StageBase {
    Stage069();
    virtual ~Stage069();
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

Stage069::Stage069() {}
Stage069::~Stage069() {}
void Stage069::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B927C(&D_stage069_09D5E300);
    StageManager::objectPtr->push_prop_089B927C(&D_stage069_09D5E350);
    StageManager::objectPtr->push_prop_089B943C(&D_stage069_09D5E3A0);
    StageManager::objectPtr->push_prop_089B95FC(&D_stage069_09D5E400);
    StageBase::vtable_0x24();
}
stage_definitions *Stage069::definitions() { return &D_stage069_09D5E268; }
void Stage069::operator delete(void *) {}
stage_draw_commands *Stage069::vtable_0x48() { return &D_stage069_09D5E2E8; }
