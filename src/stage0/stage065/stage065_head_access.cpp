#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage065_09D5E310;
extern stage_draw_commands D_stage065_09D5E3D8;

struct Stage065 : StageBase {
    Stage065();
    virtual ~Stage065();
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

stage_definitions *Stage065::definitions() { return &D_stage065_09D5E310; }
void Stage065::operator delete(void *) {}
stage_draw_commands *Stage065::vtable_0x48() { return &D_stage065_09D5E3D8; }
