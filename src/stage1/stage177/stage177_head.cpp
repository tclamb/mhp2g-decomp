#include "common.h"
#include "stage_base.hpp"

extern stage_definitions D_stage177_09D5E300;
extern stage_draw_commands D_stage177_09D5E3C0;

struct Stage177 : StageBase {
    Stage177();
    virtual ~Stage177();
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

Stage177::Stage177() {}
Stage177::~Stage177() {}
void Stage177::vtable_0x24() { StageBase::vtable_0x24(); }
stage_definitions *Stage177::definitions() { return &D_stage177_09D5E300; }
void Stage177::operator delete(void *) {}
stage_draw_commands *Stage177::vtable_0x48() { return &D_stage177_09D5E3C0; }
