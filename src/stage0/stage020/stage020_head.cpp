#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern stage_definitions D_stage020_09D5E330;
extern stage_draw_commands D_stage020_09D5E3D0;

struct Stage020 : StageBase {
    Stage020();
    virtual ~Stage020();
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

Stage020::Stage020() {}
Stage020::~Stage020() {}
void Stage020::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B951C(0);
    StageBase::vtable_0x24();
}
stage_definitions *Stage020::definitions() { return &D_stage020_09D5E330; }
void Stage020::operator delete(void *) {}
stage_draw_commands *Stage020::vtable_0x48() { return &D_stage020_09D5E3D0; }
