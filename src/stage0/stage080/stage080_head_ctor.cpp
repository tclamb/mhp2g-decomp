#include "common.h"
#include "stage_base.hpp"

struct Stage080 : StageBase {
    Stage080();
    virtual ~Stage080();
    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    virtual stage_draw_commands *vtable_0x48();

    virtual void vtable_0x50();

    virtual void vtable_0x98(pmo *, void *);

    static void operator delete(void *);
    u16 unknown_0x460[4];
};

Stage080::Stage080() {
    unknown_0x460[0] = 0;
    unknown_0x460[2] = 0;
    unknown_0x460[1] = 0;
    unknown_0x460[3] = 0;
}
Stage080::~Stage080() {}
