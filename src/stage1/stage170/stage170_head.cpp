#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage170_09D5E248;

struct Stage170 : StageBase {
    Stage170();
    virtual ~Stage170();
    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    static void operator delete(void *);
};

Stage170::Stage170() {}
Stage170::~Stage170() {}
void Stage170::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage170::definitions() { return &D_stage170_09D5E248; }
void Stage170::operator delete(void *) {}
