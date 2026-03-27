#pragma once

#include "model_base.hpp"

// stub for type with vtable at D_eboot_089B5C10
struct ObjBase : ModelBase {
    typedef void (ObjBase::*MemFn)();

    ObjBase();
    virtual ~ObjBase();
    virtual void draw();
    virtual void vtable_0x10();
    virtual void vtable_0x14();
    virtual void vtable_0x18();
    virtual void vtable_0x1C();
    virtual void vtable_0x20();
    virtual void vtable_0x24();
    virtual void vtable_0x28();
    virtual void vtable_0x2C();
    virtual void vtable_0x30();
    virtual void vtable_0x34();
    virtual void vtable_0x38() = 0;
    virtual void vtable_0x3C();

    static void *operator new(u32, void *);
    static void operator delete(void *);

    u32 unknown_0x1C0;
    ObjBase *nextObj;
    ObjBase *prevObj;
    u8 padding_0x1C[8];
    u8 state_0x1D4;
    u8 unknown_0x1D5;
    u8 unknown_0x1D6;
    u8 unknown_0x1D7;
    u16 unknown_0x1D8;
    ScePspFVector4 (*diffuse_light_color_override)[3];
    u32 unknown_0x1E0;
    u8 unknown_0x1E4;
    u16 pl_id;
    u8 pl_type;
    u8 kind;
    ScePspFVector4 unknown_0x1F0;
    ScePspFVector4 position;
    ScePspFVector4 unknown_0x210;
    ScePspFVector4 scale;
    u8 alpha;
    u8 padding_0x231[0x260 - 0x231];
    ScePspFVector3 unknown_0x260;
    u8 padding_0x26C;
    float unknown_0x270;
    s16 unknown_0x274;
    u8 padding_0x276[10];
    u8 unknown_0x280;
    u8 padding_0x281[4];
    u8 unknown_0x285;
    u32 unknown_0x288;
    u32 unknown_0x28C;
    u8 unknown_0x290;
    u8 padding_0x291[7];
    u8 unknown_0x298;
    u8 animationIndex;
    u16 stageId;
    u8 padding_0x29C[0x2AC - 0x29C];
    ScePspUnion32 diffuse_light_colors[3];
    u8 padding_0x2B4[0x3FC - 0x2B8];
    MemFn memFn;
    u32 unknown_0x408;
    u32 unknown_0x40C;
    u32 unknown_0x410;
    u32 unknown_0x414;
    u8 padding_0x418[0x480 - 0x418];

protected:
    inline void setMemFn(MemFn f) {
        if (f != 0) {
            memFn = f;
        }
    }
};
