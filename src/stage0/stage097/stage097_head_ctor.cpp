#include "common.h"
#include "stage_base.hpp"

struct Stage097 : StageBase {
    Stage097();
    virtual ~Stage097();
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

    static void operator delete(void *);
    u8 unknown_0x460;
};

Stage097::Stage097() : unknown_0x460(0) {}
Stage097::~Stage097() {}
