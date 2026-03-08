#pragma once

#include "common.h"
#include "model.hpp"

struct stage_fog_params {
    float color;
    float near;
    float far;
};

struct stage_unk1_params {

};

struct stage_unk2_params {

};

struct stage_environment_params {
    u16 type;
    u8 data[0];
};

struct stage_depth_buffer_params {
    float *tristrip_vertices;
    u16 unknown_0x4;
    u16 unknown_0x6;
    u16 unknown_0x8;
    u16 unknown_0xA;
};

struct stage_sound {
    u32 unknown_0x0;
    u32 unknown_0x4;
    u32 unknown_0x8;
    u32 unknown_0xC;
    ScePspFVector4 position;
};

struct stage_exit {
    u16 destination_stage_id;
    s16 shape;
    ScePspFVector3 p;
    float size;
    float height;
    ScePspFVector3 q;
    u8 undefined_0x24[0x10];
};

struct stage_definitions {
    u32 unknown_0x0;
    u32 unknown_0x4;
    u32 unknown_0x8;
    u8 *unknown_0xC;
    u16 *unknown_0x10;
    u16 unknown_0x14;
    u8 undefined_0x16[0x10];
    u16 unknown_0x26;
    u8 undefined_0x28[0x8];
    stage_exit *exits;
    stage_sound *sounds;
    u32 *unknown_0x38;
    u8 unknown_0x3C;
    u8 exit_count;
    s8 unknown_0x3E;
    u8 sound_count;
    u8 unknown_0x40;
    u8 unknown_0x41;
    u16 unknown_0x42;
    u8 undefined_0x44[0xFC];
};

struct stage_draw_command {
    u8 opcode;
    u8 flags;
    u8 alpha_threshold;
    u8 mesh_index;
    void *data;
};


struct stage_commands {
    s16 model_commands_length;
    s16 draw_commands_length;
    stage_draw_command *model_commands;
    stage_draw_command *draw_commands;
};

struct base_stage : model {
    typedef void (base_stage::*ptmf)(void);

    base_stage();
    virtual ~base_stage();
    virtual void draw();

    virtual void clear();
    virtual void call_ptmf_0x3D8();
    virtual void destroy();
    virtual void vtable_0x1C();
    virtual void vtable_0x20();
    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual stage_exit *exits(u32 map_id);
    virtual s8 exit_count(u32 map_id);
    virtual void vtable_0x34();
    virtual void vtable_0x38();
    virtual void vtable_0x3C();
    virtual void vtable_0x40();
    virtual void vtable_0x44();
    virtual stage_commands *vtable_0x48();
    virtual void vtable_0x4C();
    virtual void vtable_0x50();
    virtual void vtable_0x54(pmo *, void *, u8);
    virtual void vtable_0x58(pmo *, void *, u8);
    virtual void vtable_0x5C(pmo *, void *, u8);
    virtual void vtable_0x60(pmo *, void *, u8);
    virtual void vtable_0x64(pmo *, void *, u8);
    virtual void vtable_0x68(pmo *, void *, u8);
    virtual void vtable_0x6C(pmo *, void *, u8);
    virtual void vtable_0x70(pmo *, void *, u8);
    virtual void vtable_0x74(pmo *, void *, u8);
    virtual void vtable_0x78(pmo *, void *, u8);
    virtual void vtable_0x7C(pmo *, void *, u8);
    virtual void vtable_0x80(pmo *, void *, u8);
    virtual void vtable_0x84(pmo *, void *, u8);
    virtual void vtable_0x88(pmo *, void *);
    virtual void vtable_0x8C(pmo *, void *, u8);
    virtual void vtable_0x90(pmo *, void *, u8);
    virtual void vtable_0x94(pmo *, void *, u8);
    virtual void vtable_0x98(pmo *, void *);
    virtual void vtable_0x9C();
    virtual void vtable_0xA0();
    virtual void vtable_0xA4();
    virtual u32 vtable_0xA8() { return 0; }
    virtual u32 vtable_0xAC();
    virtual void vtable_0xB0();
    virtual void vtable_0xB4();
    virtual void vtable_0xB8();

    static void operator delete(void *p);

    u16 unknown_0x1C0;
    u16 unknown_0x1C2;
    u8 undefined_0x1C4[12];
    pmo prop_pmo;
    skeleton prop_skeleton;
    u8 unknown_0x330;
    u8 undefined_0x331[3];
    u8 struct_0x334[0x6C];
    u16 unknown_0x3A0;
    u16 unknown_0x3A2;
    u8 struct_0x3A4[0x30];
    u8 unknown_0x3D4;
    u8 undefined_0x3D5[3];
    ptmf ptmf_0x3D8;
    stage_depth_buffer_params depth_buffer_params[4];
    u8 undefined_0x414[4];
    u16 unknown_0x418;
    u16 unknown_0x41A;
    u16 unknown_0x41C;
    u8 undefined_0x41E[6];
    u16 unknown_0x424;
    u16 unknown_0x426;
    u16 unknown_0x428;
    u8 undefined_0x42A[6];
    u16 unknown_0x430;
    u8 undefined_0x432[10];
    u16 unknown_0x43C;
    u16 unknown_0x43E;
    u16 unknown_0x440;
    u8 undefined_0x442[2];
    u32 unknown_0x444;
    u8 struct_0x448[0xC];

    void method_088CA25C();
    void method_088CA624();
    void method_088CDCAC();
    void method_088CEA2C();
    void compile_environment_params(stage_environment_params *);
    stage_unk1_params *compile_fog_params(stage_fog_params *);
    stage_unk2_params *compile_unk1_params(stage_unk1_params *);
    void compile_unk2_params(stage_unk2_params *);


protected:
    inline void set_ptmf_0x3D8(ptmf x) {
        if (x != 0) {
            ptmf_0x3D8 = x;
        }
    }
};
