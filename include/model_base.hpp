#pragma once

#include "common.h"

#include "joint.hpp"
#include "draw.hpp"

// Motion (animation) data, as read by the Hierarchy/Joint code.
// A key is a Hermite spline point: value, frame, tangents (see spline()).
struct motion_key {
    s16 value;
    s16 frame;
    s16 tangent_in;   // slope used when this key ends a segment
    s16 tangent_out;  // slope used when this key starts a segment
};

// One animated component of one joint.
struct motion_track {
    u16 type;         // one bit: 0x001-0x004 scale xyz, 0x008-0x020 rotation xyz, 0x040-0x100 position xyz
    u16 unknown_0x2;
    u32 key_count;
    u32 size;         // bytes to the next track
    motion_key keys[1];
};

// The tracks of one joint (motion + 0x14 onwards, walked by size).
struct motion_group {
    u32 flags;        // & 0x1FF: which components are animated
    u32 track_count;
    u32 size;         // bytes to the next group
    motion_track tracks[1];
};

struct Hierarchy {
    inline Hierarchy() {
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
    virtual ~Hierarchy() {}

    // One animation channel (4 of them, 0x40 bytes each, at Hierarchy+0x10).
    struct Motion {
        enum {
            ACTIVE = 1 << 0,  // a motion is set
            LOOP   = 1 << 1,  // motion header +0xC != 0: wrap to start at end
            BLEND_LOOP = 1 << 2, // copy of LOOP for the blended-out motion
        };
        float frame;          // 0x00 current frame
        float speed;          // 0x04 frames per update (set to 2.0 by func_eboot_08864234)
        float start;          // 0x08 first frame (motion header +0x10)
        float end;            // 0x0C last key frame over all tracks (func_eboot_08860254)
        float blend_frame;    // 0x10 blended-out motion: its frame when the blend started
        float blend_speed;    // 0x14 1 / (blend frames + 1)
        float blend_start;    // 0x18 blended-out motion: start
        float blend_end;      // 0x1C blended-out motion: end
        s32 blend_frames;     // 0x20 blend frames + 1
        float blend_step;     // 0x24 1 / (blend frames + 1)
        void *motion;         // 0x28 motion data (header: +0x4 group count, +0xC loop, +0x10 start frame)
        u16 flags;            // 0x2C
        s8 direction;         // 0x2E 0 = plain set, 1 / -1 = blend (sign of the blend count)
        u8 blending;          // 0x2F old motion captured for blending
        ScePspFVector4 root_delta; // 0x30 (cleared for channel 0 only)
    };

    u8 unknown_0x4[0xC];
    Motion motion[4];
    Joint *roots[4];
    u16 root_count;
    u16 chain_count; // ??
    u32 joint_count;
    u8 unknown_0x128[4];
    u32 *motion_table;
    u16 unknown_0x130[4];
    u16 unknown_0x138[4];
};

extern "C" {
    void func_eboot_088641B8(Hierarchy *);
}

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

    void draw(Hierarchy *skeleton, tmh *tmh, ScePspFMatrix4 *transform);
    void drawMesh(Hierarchy *skeleton, tmh *tmh, u8 mesh);
    void drawWeight(Hierarchy *skeleton, tmh *tmh, ScePspFMatrix4 *transform);
    void drawWeightMesh(Hierarchy *skeleton, tmh *tmh, int mesh);
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

struct ModelBase : Draw {
    ModelBase();
    virtual ~ModelBase();
    virtual void draw();

    ScePspFMatrix4 transform;
    pmo model_pmo;
    tmh model_tmh;
    Hierarchy hierarchy;

    int compile_pmo(void *, pmo_header *, pmo_mesh_data *);
    int compile_tmh(void *, tmh_header *);
    void reset_transform();

    static void operator delete(void *p);
};
