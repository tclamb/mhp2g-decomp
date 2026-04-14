#include "model_base.hpp"

#include "vfpu.h"
#include "draw_manager.hpp"
#include "immediate_ge.hpp"

#pragma opt_unroll_loops on
//#define BUILD_NONMATCHING

using namespace immediate_ge;

ModelBase::ModelBase() {
    memset((void *)&model_pmo, 0, sizeof(model_pmo));
    memset((void *)&hierarchy, 0, sizeof(hierarchy));
    memset((void *)&model_tmh, 0, sizeof(model_tmh));
}

ModelBase::~ModelBase() {
    // empty
}

void ModelBase::draw() {
    model_pmo.draw(&hierarchy, &model_tmh, &transform);
}

void emit_world_model(ScePspFMatrix4 *transform, ScePspFVector4 *scale);

void pmo::draw(Hierarchy *hierarchy, tmh *tmh, ScePspFMatrix4 *transform) {
    emit_world_model(transform, &scale);
    for (int i = 0; i < header->mesh_count; ++i) {
        drawMesh(hierarchy, tmh,  i);
    }
}

void pmo::drawMesh(Hierarchy *hierarchy, tmh *tmh, u8 mesh_index) {
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
                        u32 (&fragment)[8] = tmh->fragments[texture_index].commands;
                        ge::texturemapenable(true);

                        ge::texmode(GE_TEXMODE_SWIZZLE);

                        ge::impl::emit(fragment[0]);
                        ge::impl::emit(fragment[1]);
                        ge::impl::emit(fragment[2]);
                        ge::impl::emit(fragment[3]);
                        ge::impl::emit(fragment[4]);
                        ge::impl::emit(fragment[5]);
                        ge::impl::emit(fragment[6]);
                        ge::impl::emit(fragment[7]);

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

void pmo::draw_alpha(tmh *tmh, ScePspFMatrix4 *transform, u32 mesh_index, u32 blend_mode, u8 alpha) {
    u16 mesh_index_16 = mesh_index;
    set_mesh_blend_mode(mesh_index_16, blend_mode);
    ge::atest(0xFF, 0, GE_OP_GREATER_THAN);
    emit_world_model(transform, &scale);
    set_mesh_alpha(mesh_index_16, alpha);
    set_mesh_shadow_color(mesh_index_16, 0xFF, 0xFF, 0xFF);
    drawMesh(0, tmh, mesh_index);
    ge::atest(0xFF, 0x80, GE_OP_AT_LEAST);
}

void pmo::draw_rgba8888(tmh *tmh, ScePspFMatrix4 *transform, u32 mesh_index, u32 blend_mode, u32 color) {
    u16 mesh_index_16 = mesh_index;
    ScePspUnion32 rgba; rgba.ui = color;
    set_mesh_blend_mode(mesh_index_16, blend_mode);
    ge::atest(0xFF, 0, GE_OP_GREATER_THAN);
    emit_world_model(transform, &scale);
    u8 r = rgba.uc[0], b = rgba.uc[2], g = rgba.uc[1], a = rgba.uc[3];
    set_mesh_color(mesh_index_16, r, g, b);
    set_mesh_shadow_color(mesh_index_16, r, g, b);
    set_mesh_alpha(mesh_index_16, a);
    drawMesh(0, tmh, mesh_index);
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
    DrawManager::objectPtr->world_model(&o);
}

#ifdef BUILD_NONMATCHING
// 65/7700, one regswap remaining
int pmo::compile(void *buffer, pmo_header *header, pmo_mesh_data *mesh_data) {
    int i;

    this->header = header;
    this->mesh_data = mesh_data;

    sv_q(&this->scale, header->scale.x, header->scale.y, header->scale.z, 0);

    {
        pmo_material_data *out = (pmo_material_data *)buffer;
        this->material_data = out;
        pmo_material_data *data = header->material_data(0);
        for (i = 0; i < header->material_count(); ++i, ++out, ++data) {
            out->color.ui = data->color.ui;
            out->shadow_color.ui = data->shadow_color.ui;
            out->texture_index = data->texture_index;
        }
        buffer = out;
    }

    {
        pmo_mesh_lighting *out = (pmo_mesh_lighting *)buffer;
        this->mesh_lighting_data = out;
        pmo_mesh_header *mesh = header->mesh_header(0);
        for (i = 0; i < header->mesh_count; ++i, ++out, ++mesh) {
            out->flags = mesh->lighting_flags;
            out->blend_mode_cmd = mesh->blend_mode_cmd;
        }
        buffer = out;
    }

    return 1;
}
#else
INCLUDE_ASM("asm/eboot/nonmatchings/model_base", compile__3pmoFPvP10pmo_headerP13pmo_mesh_data);
#endif

int ModelBase::compile_pmo(void *buffer, pmo_header *header, pmo_mesh_data *mesh_data) {
    return model_pmo.compile(buffer, header, mesh_data);
}

int tmh::compile(void *buffer, tmh_header *header, u8 index) {
    GeTexture t;
    if (this != 0) {
        this->fragments = (tmh_fragment*)buffer;
        for (int i = 0; i < header->picture_count; ++i) {
            if (Ge::objectPtr->method_08859768(header, i, 0, 0, &t) == 0) {
                return 0;
            }

            tmh_fragment *out = &this->fragments[i] + index;

            out->commands[0] = (GE_CMD_TEXFORMAT << 24) | t.format;
            out->commands[1] = (GE_CMD_TEXADDR0 << 24) |      ((u32)t.data & 0x00FFFFFF);
            out->commands[2] = (GE_CMD_TEXBUFWIDTH0 << 24) | (((u32)t.data & 0xFF000000) >> 8) | t.width;

            u32 halign = Ge::objectPtr->method_0885973C(t.height);
            u32 texsize = (GE_CMD_TEXSIZE << 24) | halign << 8;
            u32 walign = Ge::objectPtr->method_0885973C(t.width);
            texsize |= walign;
            out->commands[3] = texsize;

            u8 index_mask = 0xFF;
            out->commands[4] = (GE_CMD_CLUTFORMAT << 24) | (index_mask << 8) | t.palette_width;
            out->commands[5] = (GE_CMD_CLUTADDR << 24) | ((u32)t.palette_data & 0x00FFFFFF);
            out->commands[6] = (GE_CMD_CLUTADDRUPPER << 24) | (((u32)t.palette_data & 0xFF000000) >> 8);
            out->commands[7] = (GE_CMD_LOADCLUT << 24) | ((t.palette_height + 7) >> 3);
        }

        this->header = header;
        this->picture_count = header->picture_count;
        return 1;
    }
    return 0;
}

int ModelBase::compile_tmh(void *buffer, tmh_header *header) {
    return model_tmh.compile(buffer, header, 0);
}

void ModelBase::operator delete(void *p) {
    // empty
}

void ModelBase::reset_transform() {
    flags = Draw::VISIBLE | Draw::ALIVE;
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

void pmo::set_mesh_color(u16 mesh_index, u8 r, u8 g, u8 b) {
    u8 *remap = header->material_remap(mesh_index, 0);
    int count = header->mesh_material_count(mesh_index);
    for (int i = 0; i < count; ++i) {
        pmo_material_data &material = material_data[remap[i]];
        material.color.uc[0] = r;
        material.color.uc[1] = g;
        material.color.uc[2] = b;
    }
}
void pmo::set_mesh_shadow_color(u16 mesh_index, u8 r, u8 g, u8 b) {
    u8 *remap = header->material_remap(mesh_index, 0);
    int count = header->mesh_material_count(mesh_index);
    for (int i = 0; i < count; ++i) {
        pmo_material_data &material = material_data[remap[i]];
        material.shadow_color.uc[0] = r;
        material.shadow_color.uc[1] = g;
        material.shadow_color.uc[2] = b;
    }
}

void pmo::set_mesh_alpha(u16 mesh_index, u8 a) {
    u8 *remap = header->material_remap((u16)mesh_index, 0);
    int count = header->mesh_material_count((u16)mesh_index);
    for (int i = 0; i < count; ++i) {
        material_data[remap[i]].color.uc[3] = a;
    }
}

void pmo::set_mesh_blend_mode(u16 mesh_index, u8 blend_mode) {
    pmo_mesh_lighting *lighting = mesh_lighting(mesh_index);
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

void pmo::set_mesh_lighting(u16 mesh_index, bool enable) {
    pmo_mesh_lighting *lighting = mesh_lighting(mesh_index);
    if (enable == true) {
        lighting->flags |= pmo_mesh_lighting::LIGHTING;
    } else {
        lighting->flags &= ~pmo_mesh_lighting::LIGHTING;
    }
}

void pmo::set_mesh_fog(u16 mesh_index, bool enable) {
    pmo_mesh_lighting *lighting = mesh_lighting(mesh_index);
    if (enable == true) {
        lighting->flags |= pmo_mesh_lighting::FOG;
    } else {
        lighting->flags &= ~pmo_mesh_lighting::FOG;
    }
}

pmo_mesh_lighting *pmo::mesh_lighting(u32 mesh_index) {
    return mesh_lighting_data + mesh_index;
}

void pmo_mesh_lighting::emit() {
    if ((flags & pmo_mesh_lighting::ENABLE) != 0) {
        if ((flags & pmo_mesh_lighting::LIGHTING) != 0) {
            ge::lightingenable(true);
        } else {
            ge::lightingenable(false);
        }

        if ((flags & pmo_mesh_lighting::FOG) != 0) {
            ge::fogenable(true);
        } else {
            ge::fogenable(false);
        }

        if ((flags & pmo_mesh_lighting::ALPHABLEND) != 0) {
            ge::alphablendenable(true);
            ge::impl::emit(blend_mode_cmd);

            const u32 fixed_blend_mode =
                ((GE_CMD_BLENDMODE << 24) |
                    (GE_BLENDMODE_MUL_AND_ADD << 8) |
                    (GE_DSTBLEND_FIXB << 4) |
                    (GE_SRCBLEND_SRCALPHA));
            if (blend_mode_cmd == fixed_blend_mode) {
                ge::colortestenable(true);
                ge::colortest(GE_OP_NOT_EQUALS);
                ge::colorref(0, 0, 0);
            }
        }
    }
}

float spline(float t, float x0, float t0, float dxdt0, float x1, float t1, float dxdt1) {
    // standard cubic spline interpolation on the interval [t0, t1]
    // found by solving f(t) = a + b*t + c*t^2 + d*t^3 for coefficients
    // such that f(t0) = x0, f(t1) = x1, f'(t0) = dxdt0, f'(t1) = dxdt1
    // for computation, rearrange that solution into the form:
    //     f(t) =   x0 g0(t*) + dxdt0 (t - t0) h0(t*)
    //            + x1 g1(t*) + dxdt1 (t - t0) h1(t*)
    //     where t* = (t - t0) / (t1 - t0)
    // this yields the following polynomials in the normalized variable:
    //     g0(t) = 1 - 3t^2 + 2t^3
    //     h0(t) = 1 - 2t + t^2
    //     g1(t) = 3t^2 - 2t^3
    //     h1(t) = -t + t^2
    float result;
#ifdef __MWERKS__
    __asm__ (
        "lv.s S100, %1"
        "lv.s S101, %2"
        "lv.s S102, %3"
        "lv.s S103, %4"
        "lv.s S110, %5"
        "lv.s S111, %6"
        "lv.s S112, %7"
        "vsub.s S200, S100, S102" // t - t0
        "vsub.s S201, S111, S102" // t1 - t0
        "vrcp.s S201, S201"       // (t1 - t0)^-1
        "vmul.s S202, S201, S201" // (t1 - t0)^-2
        "vmul.s S203, S200, S200" // (t - t0)^2
        "vmul.s S210, S203, S201" // (t - t0)^2 / (t1 - t0)
        "vmul.s S211, S203, S200" // (t - t0)^3
        "vmul.s S211, S211, S202" // (t - t0)^3 / (t1 - t0)^2
        "vfim.s S212, 2.0f"
        "vmul.s S212, S212, S211" // 2 (t - t0)^3 / (t1 - t0)^2
        "vmul.s S212, S212, S201" // 2 (t - t0)^3 / (t1 - t0)^3 = 2 t*^3
        "vfim.s S213, 3.0f"
        "vmul.s S213, S213, S203" // 3 (t - t0)^2
        "vmul.s S213, S213, S202" // 3 (t - t0)^2 / (t1 - t0)^2 = 3 t*^2
        "vfim.s S220, 1.0f"
        "vadd.s S220, S220, S212" // 1 + 2 t*^3
        "vsub.s S220, S220, S213" // 1 - 3 t*^3 + 2 t*^2 = g0(t*)
        "vmul.s S220, S101, S220" // x0 g0(t*)
        "vsub.s S221, S213, S212" // 3 t*^2 - 2 t*^3  = g1(t*)
        "vmul.s S221, S110, S221" // x1 g1(t*)
        "vsub.s S222, S211, S210" // (t - t0) (-t* + t*^2)
        "vsub.s S222, S222, S210" // (t - t0) (-2t* + t*^2)
        "vadd.s S222, S222, S200" // (t - t0) (1 - 2t* + t*^2) = (t - t0) h0(t*)
        "vmul.s S222, S103, S222" // dxdt0 (t - t0) h0(t*)
        "vsub.s S223, S211, S210" // (t - t0) (-t* + t*^2) = (t - t0) h1(t*)
        "vmul.s S223, S112, S223" // dxdt1 (t - t0) h1(t*)
        "vfad.q S000, C220"       // f(t)
        "sv.s S000, %0"
        : "=m"(result)
        : "m"(t),
          "m"(x0), "m"(t0), "m"(dxdt0),
          "m"(x1), "m"(t1), "m"(dxdt1)
    );
#else
    float tn = (t - t0) / (t1 - t0);
    float tn2 = tn*tn;
    float tn3 = tn*tn*tn;
    return x0 * (1 - 3*tn2 + 2*tn3)
        + x1 * (3*tn2 - 2*tn3)
        + dxdt0 * (t - t0) * (1 - 2*tn + tn2)
        + dxdt1 * (t - t0) * (-tn + tn2);
#endif
    return result;
}

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_088630C8);

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

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08863190);

// only a rotation & translation, no scale
extern "C"
void func_eboot_088632D0(ScePspFMatrix4 *, ScePspFMatrix4 *out, ScePspFMatrix3 *args) {
    vmidt_q(out);
    eulerRotation(out, out, args->y.x, args->y.y, args->y.z);
    out->w.x = args->z.x; out->w.y = args->z.y; out->w.z = args->z.z;
}

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_088633C4);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08863644);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08863660);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_088637BC);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08863B68);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08863D50);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08863DEC);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08863E68);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_088640F0);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_088641B8);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08864214);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08864234);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_088642F4);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08864340);

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08864400);
