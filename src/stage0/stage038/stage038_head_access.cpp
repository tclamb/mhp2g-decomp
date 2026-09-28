#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage038_09D5E338;
extern stage_draw_commands D_stage038_09D5E5A8;

struct Stage038 : StageBase {
    Stage038();
    virtual ~Stage038();
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

stage_definitions *Stage038::definitions() { return &D_stage038_09D5E338; }
void Stage038::operator delete(void *) {}
stage_draw_commands *Stage038::vtable_0x48() { return &D_stage038_09D5E5A8; }
