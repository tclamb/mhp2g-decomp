#include "common.h"
#include "stage_base.hpp"

struct Stage210 : StageBase {
    Stage210();
    virtual ~Stage210();
    virtual void vtable_0x24();
    virtual void draw_sky_gradient();
    virtual void vtable_0x20();
    virtual void vtable_0x1C();
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

bool Stage210::vtable_0xA0() { return true; }
bool Stage210::vtable_0xA4() { return false; }
bool Stage210::vtable_0xA8() { return false; }
int Stage210::vtable_0xAC() { return definitions()->sound_count; }
stage_sound * Stage210::vtable_0xB0() { return definitions()->sounds; }
u8 Stage210::vtable_0xB4() { return definitions()->unknown_0x40; }
stage_definitions_0x38_t * Stage210::vtable_0xB8() { return definitions()->unknown_0x38; }

extern "C" void func_stage210_09D5F8F8();
extern "C" void func_stage210_09D5E5C8();
extern "C" void func_stage210_09D5E968();
extern "C" void func_stage210_09D5E9F0();
extern "C" void func_stage210_09D5EB18();
extern "C" void func_stage210_09D5F938();
extern "C" void func_stage210_09D5F978();
extern "C" void func_stage210_09D5F998();
extern "C" void func_stage210_09D5F9C8();

extern "C" void *__vt__13prop_089C41C8[8] __attribute__((section(".vtables"))) = {
    0, 0, (void *)func_stage210_09D5F8F8, (void *)func_stage210_09D5E5C8,
    (void *)func_stage210_09D5E968, (void *)func_stage210_09D5E9F0,
    (void *)func_stage210_09D5EB18, (void *)func_stage210_09D5F938
};
extern "C" void *D_eboot_089C41E8[4] __attribute__((section(".vtables"))) = {
    0, 0, (void *)func_stage210_09D5F978, 0
};
extern "C" void *D_eboot_089C41F8[8] __attribute__((section(".vtables"))) = {
    0, 0, (void *)func_stage210_09D5F998, 0,
    (void *)func_stage210_09D5F9C8, 0, 0, (void *)func_stage210_09D5F938
};
