#pragma once

#include "common.h"

#include "bone.hpp"

#include "drawable.hpp"

struct skeleton {
    inline skeleton() {
        for (int i = 0; i < 4; ++i) {
            roots[i] = 0;
        }
        for (int i = 0; i < 4; ++i) {
            unknown_0x128[i] = 0;
            unknown_0x130[i] = 0;
            unknown_0x138[i] = 0;
        }
        root_count = 0;
        chain_count = 0;
    }
    virtual ~skeleton() {}

    u8 unknown_0x4[0x10C];
    bone *roots[4];
    u16 root_count;
    u16 chain_count; // ??
    u32 bone_count;
    u8 unknown_0x128[4];
    u32 *motion_table;
    u16 unknown_0x130[4];
    u16 unknown_0x138[4];
};

struct tmh_block_header {
    u32 size;
    u32 type;
};

struct tmh_palette_header : tmh_block_header {
    u32 width;
    u32 height;
};

struct tmh_image_header : tmh_block_header {
    u32 format;
    u16 width;
    u16 height;
};

struct tmh_picture_header : tmh_block_header {
    u32 image_count;
    u32 palette_count;
};

struct tmh_header {
    u8 magic[0x4];
    u32 version;
    s32 picture_count;
    u32 padding_0xC;
};

struct tmh_fragment {
    u32 commands[8];
};

struct tmh {
    tmh_header *header;
    tmh_fragment *fragments;
    s8 picture_count;
    u8 unknown_0x9[7];

    int compile(void *buffer, tmh_header *header, u8 index);
};

struct pmo_material_data {
    ScePspUnion32 color;
    ScePspUnion32 shadow_color;
    u8 texture_index;
    u32 padding_0xC;
};

struct pmo_mesh_data {

};

struct pmo_mesh_header {
    ScePspIVector2 uv_scale;
    u32 lighting_flags;
    u32 blend_mode_cmd;
    u8 material_count;
    u16 cumulative_material_count;
    u16 tristrip_count;
    u16 cumulative_tristrip_count;
};

struct pmo_tristrip_header {
    s8 material_offset;
    u8 tristrip_count;
    u16 cumulative_tristrip_count;
    u32 mesh_offset;
    u32 vertex_offset;
    u32 index_offset;
};

struct pmo_mesh_lighting {
    enum {
        LIGHTING   = 1 << 0,
        FOG        = 1 << 1,
        ALPHABLEND = 1 << 2,
        ENABLE     = 1 << 31,
    };

    u32 flags;
    u32 blend_mode_cmd;
    void emit();
};

struct pmo_header {
    u8 magic[4];
    u32 version;
    u32 size;
    float clipping_distance;
    ScePspFVector3 scale;
    u16 mesh_count;
    u16 material_count_;
    u32 mesh_header_offset;
    u32 tristrip_header_offset;
    u32 material_remaps_offset;
    u32 bone_data_offset;
    u32 material_data_offset;
    u32 mesh_data_offset;

    pmo_mesh_data *mesh_data();
    u32 mesh_data_size();
    pmo_mesh_header *mesh_header(u32 mesh_index);
    u8 mesh_material_count(u32 mesh_index);
    u8 *material_remap(u32 mesh_index, u32 material);
    s32 material_count();
    pmo_tristrip_header *tristrip_header(u32 mesh_index, u32 tristrip_index);

    inline pmo_material_data *material_data(u32 material_index) {
        return (pmo_material_data *)(magic + material_data_offset) + material_index;
    }
};

struct pmo {
    pmo_header *header;
    pmo_mesh_data *mesh_data;
    pmo_material_data *material_data;
    pmo_mesh_lighting *mesh_lighting_data;
    ScePspFVector4 scale;

    void draw(skeleton *skeleton, tmh *tmh, ScePspFMatrix4 *transform);
    void drawMesh(skeleton *skeleton, tmh *tmh, u8 mesh);
    void draw_alpha(tmh *tmh, ScePspFMatrix4 *transform, u32 mesh, u32 blend_mode, u8 alpha);
    void draw_rgba8888(tmh *tmh, ScePspFMatrix4 *transform, u32 mesh, u32 blend_mode, u32 color);
    int compile(void *, pmo_header *, pmo_mesh_data *);
    void set_mesh_color(u16 mesh_index, u8 r, u8 g, u8 b);
    void set_mesh_shadow_color(u16 mesh_index, u8 r, u8 g, u8 b);
    void set_mesh_alpha(u16 mesh_index, u8 a);
    void set_mesh_blend_mode(u16 mesh_index, u8 blend_mode);
    void set_mesh_lighting(u16 mesh_index, bool enable);
    void set_mesh_fog(u16 mesh_index, bool enable);
    pmo_mesh_lighting *mesh_lighting(u32 mesh_index);
};

// possible wrapper class?
void emit_world_model(ScePspFMatrix4 *transform, ScePspFVector4 *scale);

struct model : drawable {
    model();
    virtual ~model();
    virtual void draw();

    ScePspFMatrix4 transform;
    pmo model_pmo;
    tmh model_tmh;
    skeleton model_skeleton;

    int compile_pmo(void *, pmo_header *, pmo_mesh_data *);
    int compile_tmh(void *, tmh_header *);
    void reset_transform();

    static void operator delete(void *p);
};
