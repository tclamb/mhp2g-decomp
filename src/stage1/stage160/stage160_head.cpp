#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern stage_definitions D_stage160_09D5E310;
extern stage_draw_commands D_stage160_09D5E588;
extern prop_params D_stage160_09D5E598;

struct Stage160 : StageBase {
    Stage160();
    virtual ~Stage160();
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

Stage160::Stage160() {}
Stage160::~Stage160() {}
void Stage160::vtable_0x24() {
    StageManager::objectPtr->push_prop_089B943C(&D_stage160_09D5E598);
    StageBase::vtable_0x24();
}
stage_definitions *Stage160::definitions() { return &D_stage160_09D5E310; }
void Stage160::operator delete(void *) {}
stage_draw_commands *Stage160::vtable_0x48() { return &D_stage160_09D5E588; }
