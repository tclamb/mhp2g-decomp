#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage181_09D5E240;

struct Stage181 : StageBase {
    Stage181();
    virtual ~Stage181();
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

Stage181::Stage181() {}
Stage181::~Stage181() {}
void Stage181::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage181::definitions() { return &D_stage181_09D5E240; }
void Stage181::operator delete(void *) {}
