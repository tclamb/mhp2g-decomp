#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage234_09D5E220;
extern stage_draw_commands D_stage234_09D5E3C0;

struct Stage234 : StageBase {
    Stage234();
    virtual ~Stage234();
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

Stage234::Stage234() {}
Stage234::~Stage234() {}
void Stage234::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage234::definitions() { return &D_stage234_09D5E220; }
void Stage234::operator delete(void *) {}
stage_draw_commands *Stage234::vtable_0x48() { return &D_stage234_09D5E3C0; }
