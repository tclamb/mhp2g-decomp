#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage171_09D5E248;

struct Stage171 : StageBase {
    Stage171();
    virtual ~Stage171();
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

Stage171::Stage171() {}
Stage171::~Stage171() {}
void Stage171::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage171::definitions() { return &D_stage171_09D5E248; }
void Stage171::operator delete(void *) {}
