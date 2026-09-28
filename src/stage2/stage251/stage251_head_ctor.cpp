#include "common.h"
#include "stage_base.hpp"

struct Stage251 : StageBase {
    Stage251();
    virtual ~Stage251();
    virtual void vtable_0x24();
    virtual ScePspFVector4 *vtable_0x44();
    virtual void *vtable_0x40();
    virtual void *vtable_0x3C();
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

Stage251::Stage251() {}
Stage251::~Stage251() {}
