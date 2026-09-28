#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage135_09D5E240;

struct Stage135 : StageBase {
    Stage135();
    virtual ~Stage135();
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

Stage135::Stage135() {}
Stage135::~Stage135() {}
void Stage135::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage135::definitions() { return &D_stage135_09D5E240; }
void Stage135::operator delete(void *) {}
