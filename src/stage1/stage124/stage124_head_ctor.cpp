#include "common.h"
#include "stage_base.hpp"

struct Stage124 : StageBase {
    Stage124();
    virtual ~Stage124();
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

    virtual void vtable_0x98(pmo *, void *);

    static void operator delete(void *);
};

Stage124::Stage124() {}
Stage124::~Stage124() {}
