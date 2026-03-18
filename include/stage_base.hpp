#pragma once

#include "common.h"
#include "model.hpp"

struct stage_environment_params {
    u16 type;
    u8 data[0];
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
    ScePspFVector3 destination_position;
    u8 unknown_0x30[4];
};

struct stage_definitions_0x28_t {
    ScePspFVector3 position;
    float near;
    float far;
    u16 near_index;
    u16 far_index;
};

typedef ScePspFVector2 stage_definitions_0x2C_t[4];

struct stage_definitions_0x38_t;

struct stage_definitions {
    u32 unknown_0x0;
    u32 unknown_0x4;
    u32 unknown_0x8;
    u8 *unknown_0xC;
    u16 *unknown_0x10;
    u16 unknown_0x14;
    u8 undefined_0x16[0x10];
    s16 bug_mesh_index;
    stage_definitions_0x28_t *unknown_0x28;
    stage_definitions_0x2C_t *unknown_0x2C;
    stage_exit *exits;
    stage_sound *sounds;
    stage_definitions_0x38_t *unknown_0x38;
    u8 unknown_0x3C;
    u8 exit_count;
    s8 unknown_0x3E;
    u8 sound_count;
    u8 unknown_0x40;
    u8 unknown_0x41;
    u16 unknown_0x42;
};

struct stage_draw_command {
    u8 opcode;
    u8 flags;
    u8 alpha_threshold;
    u8 mesh_index;
    void *data;
};

struct stage_draw_commands {
    s16 model_commands_length;
    s16 prop_commands_length;
    stage_draw_command *model_commands;
    stage_draw_command *prop_commands;
};

struct stage_fog {
    float begin;
    float end;
    float color;
};

struct stage_inline_vertex_data {
    ScePspUnion32 color;
    s16 x, y, z;
};

struct stage_light {
    ScePspFVector3 position;
    ScePspFVector3 diffuse_color;
    ScePspFVector3 ambient_color;
};

struct StageBase : model {
    typedef void (StageBase::*ptmf)(void);

    StageBase();
    virtual ~StageBase();
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
    virtual stage_definitions_0x28_t *vtable_0x34();
    virtual void *vtable_0x38();
    virtual void *vtable_0x3C();
    virtual void *vtable_0x40();
    virtual ScePspFVector4 *vtable_0x44();
    virtual stage_draw_commands *vtable_0x48();
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
    virtual void draw_sky_gradient();
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8() { return false; }
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    static void operator delete(void *p);

    u16 unknown_0x1C0;
    u16 unknown_0x1C2;
    u8 undefined_0x1C4[12];
    pmo prop_pmo;
    skeleton prop_skeleton;
    u8 unknown_0x330;
    u8 undefined_0x331[3];
    stage_light lights[3];
    s16 flash_state;
    s16 flash_frames;
    ScePspFVector3 sky_gradient_origin;
    s16 sky_gradient_top;
    s16 sky_gradient_height; // in 480i scanlines: 448 for full screen height
    ScePspUnion32 sky_gradient_colors[8];
    bool flag_0x3D4;
    u8 undefined_0x3D5[3];
    ptmf ptmf_0x3D8;
    stage_inline_vertex_data sky_gradient_vdata[4];
    stage_inline_vertex_data flash_blend_vdata[2];
    stage_inline_vertex_data flash_blank_vdata[2];
    u32 unknown_0x444;
    stage_fog fog;

    void drawStg();
    void drawSet();
    void emit_fog();
    void method_088CD61C();
    void compile_environment_params(stage_environment_params *);
    bool method_088CDC74();
    void method_088CDCAC();
    void draw_flash();
    void method_088CEA2C();
    stage_definitions_0x28_t *method_088CEDC0();
    stage_definitions_0x2C_t *method_088CEDF0();
    float *compile_fog_params(float *);
    float *compile_lights(float *);
    float *compile_sky_gradient(float *);


protected:
    inline void set_ptmf_0x3D8(ptmf x) {
        if (x != 0) {
            ptmf_0x3D8 = x;
        }
    }

private:
    ScePspUnion32 lerp_bgra8888(u8 *, u8 *, float);
};


struct vtable_0x54_params {
    u16 flags;
    u32 period;
    ScePspFVector4 position;
};

struct vtable_0x5C_params {
    u16 flags;
    float angle;
    ScePspFVector4 position;
};

struct vtable_0x60_params {
    u16 flags;
    float angle;
    ScePspFVector4 position;
    ScePspFVector3 scale;
};

struct vtable_0x64_params {
    u16 flags;
    u16 u_period;
    u16 v_period;
};

struct vtable_0x68_params {
    u16 u_period;
    u16 u_frequency;
    float u_phase;
    float u_amplitude;
    u16 v_period;
    u16 v_frequency;
    float v_phase;
    float v_amplitude;
};

struct vtable_0x6C_params {
    u16 flags;
    u16 u_period;
    u16 v_period;
    s16 coarseness;
    ScePspFVector4 position;
};

struct vtable_0x70_params {
    u16 flags;
    u16 u_period;
    u16 v_period;
    ScePspFVector4 position;
};

struct vtable_0x74_params {
    u16 flags;
    u16 u_period;
    u16 v_period;
    float angle;
    ScePspFVector4 position;
};

struct vtable_0x7C_params {
    ScePspFVector4 position;
    ScePspFVector4 bbox_min;
    ScePspFVector4 bbox_max;
};

struct vtable_0x80_params {
    u16 period;
    ScePspFVector4 phase;
    ScePspFVector4 amplitude;
};

struct vtable_0x84_params {
    u16 period;
    ScePspFVector4 position;
    ScePspFVector4 phase;
    ScePspFVector4 amplitude;
};

struct vtable_0x88_params {
    s16 count;
    u8 far_mesh_index;
    u8 near_mesh_index;
    float cutoff;
    ScePspFVector4 *positions;
};

struct vtable_0x8C_params {
    u16 v_period;
    ScePspFVector4 position;
};

struct vtable_0x90_params {
    u16 flags;
    u16 phase;
    float amplitude;
    float angle;
    ScePspFVector4 position;
};

struct vtable_0x94_params {
    u8 a, b, c, d;
    s16 e;
};
