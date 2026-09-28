#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage165_09D5E310;
extern stage_draw_commands D_stage165_09D5E478;

struct Stage165 : StageBase {
    Stage165();
    virtual ~Stage165();
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

stage_definitions *Stage165::definitions() { return &D_stage165_09D5E310; }
void Stage165::operator delete(void *) {}
stage_draw_commands *Stage165::vtable_0x48() { return &D_stage165_09D5E478; }
