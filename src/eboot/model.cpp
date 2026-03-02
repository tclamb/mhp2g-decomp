#include "common.h"

#include "drawable.hpp"
#include "bone.hpp"

#include "vfpu.h"

#pragma opt_unroll_loops on
//#define BUILD_NONMATCHING

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

struct tmh_header {

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


    u32 unknown_0x4;
    u32 unknown_0x8;
    float unknown_0xC;
    ScePspFMatrix4 transform;
    pmo pmo;
    tmh tmh;
    skeleton skeleton;

    int compile_pmo(pmo_material_params*, pmo_header*, pmo_mesh_data*);
    int compile_tmh(u32*, tmh_header*);
    void reset_transform();

    static void operator delete(void *p);
};

model::model() {
    memset(&pmo, 0, sizeof(pmo));
    memset(&skeleton, 0, sizeof(skeleton));
    memset(&tmh, 0, sizeof(tmh));
}

model::~model() {
    // empty
}

void model::draw() {
    pmo.draw_skin(skeleton, tmh, transform);
}

#ifdef BUILD_NONMATCHING
void pmo::draw_skin(skeleton &skeleton, tmh &tmh, ScePspFMatrix4 &transform) {

}
#else
extern "C"
INCLUDE_ASM("asm/eboot/nonmatchings/model", draw_skin__3pmoFR8skeletonR3tmhR14ScePspFMatrix4);
#endif

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08861714);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08861C74);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08861D74);
}

void world_matrix (
    ScePspFVector3 *scale,
    ScePspFVector3 *angle,
    ScePspFVector3 *position,
    int mode,
    ScePspFMatrix4 *out)

{
    ScePspFVector4 pos;
    pos.x = position->x;
    pos.y = position->y;
    pos.z = position->z;

    scaleMatrix(out, scale->x, scale->y, scale->z);

    float x, y, z, w;
    switch (mode & 0xffff) {
    case 6:
        rotateXYZ(out, angle);
        break;
    case 7:
        rotateZXY(out, angle);
        break;
    case 4:
        rotateX(out, angle->x);
        rotateY(out, angle->y);
        break;
    case 5:
        rotateZ(out, angle->z);
        rotateY(out, angle->y);
        break;
    case 1:
        rotateX(out, angle->x);
        break;
    case 2:
        rotateY(out, angle->y);
        break;
    case 3:
        rotateZ(out, angle->z);
        break;
    default:
        break;
    }
    out->w.x = pos.x;
    out->w.y = pos.y;
    out->w.z = pos.z;
}

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_0886234C);

INCLUDE_ASM("asm/eboot/nonmatchings/model", compile__3pmoFP19pmo_material_paramsP10pmo_headerP13pmo_mesh_data);
}

int model::compile_pmo(pmo_material_params *material_params, pmo_header *header, pmo_mesh_data *mesh_data) {
    return pmo.compile(material_params, header, mesh_data);
}

extern "C"
INCLUDE_ASM("asm/eboot/nonmatchings/model", compile__3tmhFPUiP10tmh_headerUi);


int model::compile_tmh(u32 *displaylist_out, tmh_header *header) {
    return tmh.compile(displaylist_out, header, 0);
}

void model::operator delete(void *p) {
    // empty
}

void model::reset_transform() {
    unknown_0x4 = 3;
    unknown_0x8 = 0;
    unknown_0xC = 0.0f;
    vmidt_q(&transform);
}

pmo_mesh_data *pmo_header::mesh_data() {
    return (pmo_mesh_data*)(magic + mesh_data_offset);
}

u32 pmo_header::mesh_data_size() {
    return size - mesh_data_offset;
}

pmo_mesh_header *pmo_header::mesh_header(u32 mesh_index) {
    return (pmo_mesh_header*)(magic + mesh_header_offset + mesh_index * sizeof(pmo_mesh_header));
}

u8 pmo_header::mesh_material_count(u32 mesh_index) {
    return mesh_header(mesh_index)->material_count;
}

u8 *pmo_header::material_remap(u32 mesh_index, u32 material) {
    pmo_mesh_header *mesh = mesh_header(mesh_index);
    return magic + material_remaps_offset + mesh->cumulative_material_count + material;
}

u32 pmo_header::material_count() {
    return material_count_;
}

pmo_tristrip_header *pmo_header::tristrip_header(u32 mesh_index, u32 tristrip_index) {
    pmo_mesh_header *mesh = mesh_header(mesh_index);
    return (pmo_tristrip_header*)(magic + tristrip_header_offset + mesh->cumulative_tristrip_count * sizeof(ScePspFVector4) + tristrip_index * sizeof(ScePspFVector4));
}

extern "C" {
// void pmo::set_mesh_color(u32 mesh_index, u8 r, u8 g, u8 b);
INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088627F8);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088629D4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862BB0);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862D2C);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862DA4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862DF8);
}

pmo_mesh_lighting *pmo::mesh_lighting(u32 mesh_index) {
    return mesh_lighting_ + mesh_index;
}

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862E5C);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862FD8);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088630C8);

static int log2table[257] = {
    [0 ... 256] =  0xff,
    [1] = 0,
    [2] = 1,
    [4] = 2,
    [8] = 3,
    [16] = 4,
    [32] = 5,
    [64] = 6,
    [128] = 7,
    [256] = 8,
};

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863190);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088632D0);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088633C4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863644);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863660);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088637BC);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863B68);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863D50);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863DEC);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863E68);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088640F0);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088641B8);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08864214);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08864234);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088642F4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08864340);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08864400);
}
