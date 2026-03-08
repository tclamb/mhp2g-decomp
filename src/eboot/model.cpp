#include "common.h"

#include "drawable.hpp"
#include "bone.hpp"

#include "vfpu.h"

#pragma opt_unroll_loops on
//#define BUILD_NONMATCHING

#include "model.hpp"

model::model() {
    memset(&model_pmo, 0, sizeof(model_pmo));
    memset(&model_skeleton, 0, sizeof(model_skeleton));
    memset(&model_tmh, 0, sizeof(model_tmh));
}

model::~model() {
    // empty
}

void model::draw() {
    model_pmo.draw_skin(model_skeleton, model_tmh, transform);
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
    return model_pmo.compile(material_params, header, mesh_data);
}

extern "C"
INCLUDE_ASM("asm/eboot/nonmatchings/model", compile__3tmhFPUiP10tmh_headerUi);


int model::compile_tmh(u32 *displaylist_out, tmh_header *header) {
    return model_tmh.compile(displaylist_out, header, 0);
}

void model::operator delete(void *p) {
    // empty
}

void model::reset_transform() {
    flags_0x4 = 3;
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
