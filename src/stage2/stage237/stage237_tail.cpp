#include "common.h"
#include "stage_base.hpp"

struct Stage237 : StageBase {
    Stage237();
    virtual ~Stage237();
    virtual void vtable_0x24();
    virtual void *vtable_0x3C();
    virtual void *vtable_0x40();
    virtual ScePspFVector4 *vtable_0x44();
    virtual stage_definitions *definitions();
    virtual stage_draw_commands *vtable_0x48();
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual int vtable_0xAC();
    virtual stage_sound * vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t * vtable_0xB8();

    static void operator delete(void *);
};

bool Stage237::vtable_0xA0() { return true; }
bool Stage237::vtable_0xA4() { return false; }
bool Stage237::vtable_0xA8() { return false; }
int Stage237::vtable_0xAC() { return definitions()->sound_count; }
stage_sound * Stage237::vtable_0xB0() { return definitions()->sounds; }
u8 Stage237::vtable_0xB4() { return definitions()->unknown_0x40; }
stage_definitions_0x38_t * Stage237::vtable_0xB8() { return definitions()->unknown_0x38; }
