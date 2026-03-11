#include "common.h"

#include "drawable.hpp"
#include "bone.hpp"

#include "vfpu.h"

#pragma opt_unroll_loops on
//#define BUILD_NONMATCHING

#include "model.hpp"
#include "drawable_manager.hpp"
#include "immediate_ge.hpp"

using namespace immediate_ge;

model::model() {
    memset(&model_pmo, 0, sizeof(model_pmo));
    memset(&model_skeleton, 0, sizeof(model_skeleton));
    memset(&model_tmh, 0, sizeof(model_tmh));
}

model::~model() {
    // empty
}

void model::draw() {
    model_pmo.draw(&model_skeleton, &model_tmh, &transform);
}

void emit_world_model(ScePspFMatrix4 *transform, ScePspFVector4 *scale);

void pmo::draw(skeleton *skeleton, tmh *tmh, ScePspFMatrix4 *transform) {
    emit_world_model(transform, &scale);
    for (int i = 0; i < header->mesh_count; ++i) {
        draw_mesh(skeleton, tmh,  i);
    }
}

void pmo::draw_mesh(skeleton *skeleton, tmh *tmh, u8 mesh_index) {
    pmo_header *header = this->header;
    pmo_mesh_header *mesh = header->mesh_header(mesh_index);
    pmo_mesh_lighting *lighting = mesh_lighting(mesh_index);
    lighting->emit();

    ge::texscale(mesh->uv_scale);

    header->mesh_material_count(mesh_index); // oops

    u8 *remap = header->material_remap(mesh_index, 0);
    u32 last_texture = -1;
    pmo_tristrip_header *tristrip = header->tristrip_header(mesh_index, 0);
    for (int i = 0; i < mesh->tristrip_count; ++i, ++tristrip) {
        if (remap[tristrip->material_offset] != -1) {
            pmo_material_data *material = &material_data[remap[tristrip->material_offset]];
            if (material->color.uc[3] != 0) {
                ge::materialupdate(GE_MATERIALCOLOR_AMBIENT | GE_MATERIALCOLOR_DIFFUSE);
                ge::materialdiffuse(material->color);
                ge::materialalpha(material->color.uc[3]);
                ge::materialambient(material->shadow_color);

                u32 texture_index = material->texture_index;
                if (material->texture_index != 0xFF) {
                    if (texture_index != last_texture) {
                        tmh_display_list_fragment *texture_fragment = tmh->display_list_fragments + texture_index;
                        ge::texturemapenable(true);

                        ge::texmode(GE_TEXMODE_SWIZZLE);

                        ge::impl::emit(texture_fragment->texformat);
                        ge::impl::emit(texture_fragment->texture_address_low);
                        ge::impl::emit(texture_fragment->texture_stride_address_high);
                        ge::impl::emit(texture_fragment->texture_size);
                        ge::impl::emit(texture_fragment->clut_format);
                        ge::impl::emit(texture_fragment->clut_address_low);
                        ge::impl::emit(texture_fragment->clut_address_high);
                        ge::impl::emit(texture_fragment->clut_load);

                        ge::texflush();
                        last_texture = material->texture_index;
                    }
                } else {
                    ge::texturemapenable(false);
                    last_texture = material->texture_index;
                }
                goto draw;
            }
        } else {
            draw:
            pmo_mesh_data *data = mesh_data + tristrip->mesh_offset;
            ge::call(data, 0);
        }
    }
    ge::colortestenable(false);
}

void pmo::draw_alpha(tmh *tmh, ScePspFMatrix4 *transform, u16 mesh_index, u32 blend_mode, u8 alpha) {
    set_mesh_blend_mode(mesh_index, blend_mode);
    ge::atest(0xFF, 0, GE_OP_GREATER_THAN);
    emit_world_model(transform, &scale);
    set_mesh_alpha(mesh_index, alpha);
    set_mesh_shadow_color(mesh_index, 0xFF, 0xFF, 0xFF);
    draw_mesh(0, tmh, mesh_index);
    ge::atest(0xFF, 0x80, GE_OP_AT_LEAST);
}

void pmo::draw_rgba8888(tmh *tmh, ScePspFMatrix4 *transform, u16 mesh_index, u32 blend_mode, u32 color) {
    ScePspUnion32 rgba; rgba.ui = color;
    set_mesh_blend_mode(mesh_index, blend_mode);
    ge::atest(0xFF, 0, GE_OP_GREATER_THAN);
    emit_world_model(transform, &scale);
    u8 r = rgba.uc[0], b = rgba.uc[2], g = rgba.uc[1], a = rgba.uc[3];
    set_mesh_color(mesh_index, r, g, b);
    set_mesh_shadow_color(mesh_index, r, g, b);
    set_mesh_alpha(mesh_index, a);
    draw_mesh(0, tmh, mesh_index);
    ge::atest(0xFF, 0x80, GE_OP_AT_LEAST);
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

void emit_world_model(ScePspFMatrix4 *transform, ScePspFVector4 *scale) {
    ScePspFMatrix4 n;
    ScePspFMatrix4 o;
    scaleMatrix(&n, scale->x, scale->y, scale->z);
    vmmulr_q(&o, transform, &n);
    drawable_manager::get()->world_model(&o);
}

#ifdef BUILD_NONMATCHING
// 65/7700, one regswap remaining
#pragma nofpregforblkmv on
int pmo::compile(void *buffer, pmo_header *header, pmo_mesh_data *mesh_data) {
    this->header = header;
    this->mesh_data = mesh_data;

    sv_q(&this->scale, header->scale.x, header->scale.y, header->scale.z, 0);

    {
        int i = 0;
        pmo_material_data *out = (pmo_material_data *)buffer;
        this->material_data = out;
        pmo_material_data *data = header->material_data(0);
        for (; i < header->material_count(); ++i, ++out, ++data) {
            out->color = data->color;
            out->shadow_color = data->shadow_color;
            out->texture_index = data->texture_index;
        }
        buffer = out;
    }

    {
        pmo_mesh_lighting *out = (pmo_mesh_lighting *)buffer;
        this->mesh_lighting_ = out;
        pmo_mesh_header *mesh = header->mesh_header(0);
        for (int i = 0; i < header->mesh_count; ++i, ++out, ++mesh) {
            out->lighting_cmd = mesh->lighting_cmd;
            out->blend_mode_cmd = mesh->blend_mode_cmd;
        }
        buffer = out;
    }

    return 1;
}
#pragma nofpregforblkmv reset
#else
extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/model", compile__3pmoFPvP10pmo_headerP13pmo_mesh_data);
}
#endif

int model::compile_pmo(void *buffer, pmo_header *header, pmo_mesh_data *mesh_data) {
    return model_pmo.compile(buffer, header, mesh_data);
}

extern "C"
INCLUDE_ASM("asm/eboot/nonmatchings/model", compile__3tmhFPvP10tmh_headerUi);


int model::compile_tmh(void *buffer, tmh_header *header) {
    return model_tmh.compile(buffer, header, 0);
}

void model::operator delete(void *p) {
    // empty
}

void model::reset_transform() {
    flags = drawable::VISIBLE | drawable::DISPOSE;
    next = 0;
    zindex = 0.0f;
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

s32 pmo_header::material_count() {
    return material_count_;
}

pmo_tristrip_header *pmo_header::tristrip_header(u32 mesh_index, u32 tristrip_index) {
    pmo_mesh_header *mesh = mesh_header(mesh_index);
    return (pmo_tristrip_header*)(magic + tristrip_header_offset + mesh->cumulative_tristrip_count * sizeof(ScePspFVector4) + tristrip_index * sizeof(ScePspFVector4));
}

// scheduling & regswaps on all three
#ifdef BUILD_NONMATCHING
void pmo::set_mesh_color(u32 mesh_index, u8 r, u8 g, u8 b) {
    u8 *remap = header->material_remap(mesh_index, 0);
    int count = header->mesh_material_count(mesh_index);
    for (int i = 0; i < count; ++i, ++remap) {
        pmo_material_data &material = material_data[*remap];
        material.color.uc[0] = r;
        material.color.uc[1] = g;
        material.color.uc[2] = b;
    }
}
void pmo::set_mesh_shadow_color(u32 mesh_index, u8 r, u8 g, u8 b) {
    u8 *remap = header->material_remap(mesh_index, 0);
    int count = header->mesh_material_count(mesh_index);
    for (int i = 0; i < count; ++i, ++remap) {
        pmo_material_data &material = material_data[*remap];
        material.shadow_color.uc[0] = r;
        material.shadow_color.uc[1] = g;
        material.shadow_color.uc[2] = b;
    }
}

void pmo::set_mesh_alpha(u32 mesh_index, u8 a) {
    u8 *remap = header->material_remap((u16)mesh_index, 0);
    int count = header->mesh_material_count((u16)mesh_index);
    for (int i = 0; i < count; ++remap, ++i) {
        material_data[*remap].color.uc[3] = a;
    }
}
#else
extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/model", set_mesh_color__3pmoFUiUcUcUc);

INCLUDE_ASM("asm/eboot/nonmatchings/model", set_mesh_shadow_color__3pmoFUiUcUcUc);

INCLUDE_ASM("asm/eboot/nonmatchings/model", set_mesh_alpha__3pmoFUiUc);
}
#endif

void pmo::set_mesh_blend_mode(u32 mesh_index, u8 blend_mode) {
    pmo_mesh_lighting *lighting = mesh_lighting((u16)mesh_index);
    switch (blend_mode) {
    case 1:
        lighting->blend_mode_cmd = (GE_CMD_BLENDMODE << 24) | 0x32;
        break;
    case 2:
        lighting->blend_mode_cmd = (GE_CMD_BLENDMODE << 24) | 0xA2;
        break;
    case 3:
        lighting->blend_mode_cmd = (GE_CMD_BLENDMODE << 24) | (0x2 << 8) | 0xA2;
        break;
    default:
        break;
    }
}

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862DA4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862DF8);
}

pmo_mesh_lighting *pmo::mesh_lighting(u32 mesh_index) {
    return mesh_lighting_ + mesh_index;
}

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/model", emit__17pmo_mesh_lightingFv);

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
