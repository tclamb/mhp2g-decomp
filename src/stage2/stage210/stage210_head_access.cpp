#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage210_09D5FB10;
extern stage_draw_commands D_stage210_09D5FBD0;

struct Stage210 : StageBase {
    Stage210();
    virtual ~Stage210();
    virtual void vtable_0x24();
    virtual void vtable_0x1C();
    virtual void vtable_0x20();
    virtual void draw_sky_gradient();
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

stage_definitions *Stage210::definitions() { return &D_stage210_09D5FB10; }
void Stage210::operator delete(void *) {}
stage_draw_commands *Stage210::vtable_0x48() { return &D_stage210_09D5FBD0; }
