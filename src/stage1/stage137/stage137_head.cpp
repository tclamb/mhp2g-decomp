#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage137_09D5E240;

struct Stage137 : StageBase {
    Stage137();
    virtual ~Stage137();
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

Stage137::Stage137() {}
Stage137::~Stage137() {}
void Stage137::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage137::definitions() { return &D_stage137_09D5E240; }
void Stage137::operator delete(void *) {}
