#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage107_09D5E298;
extern stage_draw_commands D_stage107_09D5E360;

struct Stage107 : StageBase {
    Stage107();
    virtual ~Stage107();
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

Stage107::Stage107() {}
Stage107::~Stage107() {}
void Stage107::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage107::definitions() { return &D_stage107_09D5E298; }
void Stage107::operator delete(void *) {}
stage_draw_commands *Stage107::vtable_0x48() { return &D_stage107_09D5E360; }
