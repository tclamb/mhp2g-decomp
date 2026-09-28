#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage140_09D5E348;
extern stage_draw_commands D_stage140_09D5E458;

struct Stage140 : StageBase {
    Stage140();
    virtual ~Stage140();
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

stage_definitions *Stage140::definitions() { return &D_stage140_09D5E348; }
void Stage140::operator delete(void *) {}
stage_draw_commands *Stage140::vtable_0x48() { return &D_stage140_09D5E458; }
