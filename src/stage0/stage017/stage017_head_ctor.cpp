#include "common.h"
#include "stage_base.hpp"

struct Stage017 : StageBase {
    Stage017();
    virtual ~Stage017();
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

    virtual void vtable_0x1C();

    virtual void vtable_0x20();

    virtual void vtable_0x98(pmo *, void *);

    static void operator delete(void *);
};

Stage017::Stage017() {}
Stage017::~Stage017() {}
