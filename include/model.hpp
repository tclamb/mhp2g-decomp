#pragma once

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
    u32 picture_count;
    u32 padding_0xC;
};

struct tmh {
    u8 placeholder[0x10];

    int compile(u32 *out, tmh_header *header, u32 index);
};

struct pmo_material_params {

};

struct pmo_mesh_data {

};

struct pmo_mesh_header {
    ScePspFVector2 uv_scale;
    u32 lighting_cmd;
    u32 blend_mode_cmd;
    u8 material_count;
    u16 cumulative_material_count;
    u16 tristrip_count;
    u16 cumulative_tristrip_count;
};

struct pmo_tristrip_header {
    u8 material_offset;
    u8 weight_count;
    u16 cumulative_weight_count;
    u32 mesh_offset;
    u32 vertex_offset;
    u32 index_offset;
};

struct pmo_mesh_lighting {
    u32 lighting_flags;
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
    u32 material_count();
    pmo_tristrip_header *tristrip_header(u32 mesh_index, u32 tristrip_index);
};

struct pmo {
    pmo_header *header;
    pmo_mesh_data *mesh_data;
    pmo_material_params *material_params;
    pmo_mesh_lighting *mesh_lighting_;
    ScePspFVector4 scale;

    void draw_skin(skeleton &skeleton, tmh &tmh, ScePspFMatrix4 &transform);
    void draw_skin_mesh(skeleton &skeleton, tmh &tmh, u32 index);
    void draw_alpha(tmh &tmh, ScePspFMatrix4 &transform, u32 mesh, u8 blend_mode, u8 alpha);
    void draw_alpha(tmh &tmh, ScePspFMatrix4 &transform, u32 mesh, u8 blend_mode, u32 color);
    int compile(pmo_material_params *, pmo_header *, pmo_mesh_data *);
    void set_mesh_color(u32 mesh_index, u8 r, u8 g, u8 b);
    void set_mesh_shadow_color(u32 mesh_index, u8 r, u8 g, u8 b);
    void set_mesh_alpha(u32 mesh_index, u8 a);
    void set_mesh_blend_mode(u32 mesh_index, u8 blend_mode);
    void set_mesh_lighting(u32 mesh_index, u8 lighting_params, bool enable);
    void set_mesh_fog(u32 mesh_index, u8 fog_params, bool enable);
    pmo_mesh_lighting *mesh_lighting(u32 mesh_index);
};

struct model : drawable {
    model();
    virtual ~model();
    virtual void draw();


    u32 flags_0x4;
    u32 unknown_0x8;
    float unknown_0xC;
    ScePspFMatrix4 transform;
    pmo model_pmo;
    tmh model_tmh;
    skeleton model_skeleton;

    int compile_pmo(pmo_material_params*, pmo_header*, pmo_mesh_data*);
    int compile_tmh(u32*, tmh_header*);
    void reset_transform();

    static void operator delete(void *p);
};
