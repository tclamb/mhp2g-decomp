#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage086_09D5E180;

struct Stage086 : StageBase {
    Stage086();
    virtual ~Stage086();
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

Stage086::Stage086() {}
Stage086::~Stage086() {}
void Stage086::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage086::definitions() { return &D_stage086_09D5E180; }
void Stage086::operator delete(void *) {}
