#include "common.h"
#include "stage_base.hpp"

struct Stage263 : StageBase {
    Stage263();
    virtual ~Stage263();
    virtual void vtable_0x24();
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

bool Stage263::vtable_0xA0() { return true; }
bool Stage263::vtable_0xA4() { return false; }
bool Stage263::vtable_0xA8() { return false; }
int Stage263::vtable_0xAC() { return definitions()->sound_count; }
stage_sound * Stage263::vtable_0xB0() { return definitions()->sounds; }
u8 Stage263::vtable_0xB4() { return definitions()->unknown_0x40; }
stage_definitions_0x38_t * Stage263::vtable_0xB8() { return definitions()->unknown_0x38; }
