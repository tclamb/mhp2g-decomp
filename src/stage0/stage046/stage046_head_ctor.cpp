#include "common.h"
#include "stage_base.hpp"

struct Stage046 : StageBase {
    Stage046();
    virtual ~Stage046();
    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    virtual void vtable_0x1C();

    virtual void vtable_0x20();

    virtual void *vtable_0x3C();

    virtual void *vtable_0x40();

    virtual ScePspFVector4 *vtable_0x44();

    virtual stage_draw_commands *vtable_0x48();

    virtual void vtable_0x50();

    virtual void vtable_0x98(pmo *, void *);

    static void operator delete(void *);
    u16 unknown_0x460;
};

Stage046::Stage046() : unknown_0x460(0) {}
Stage046::~Stage046() {}
