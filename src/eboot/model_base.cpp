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
            u32 texsize = (GE_CMD_TEXSIZE0 << 24) | halign << 8;
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

extern "C" s16 func_eboot_08863660(void *, motion_track *, motion_key **k0, motion_key **k1, s16 cursor, float t);
// The call passes the joint pointer through a0 as well, ahead of the floats.
extern "C" float spline__Ffffffff(void *, float t, float x0, float t0, float dxdt0, float x1, float t1, float dxdt1);

// Sample one track at frame t: find the key pair around t (cursor caches the last
// key index), then Hermite-interpolate between them.
extern "C" float func_eboot_088630C8(void *joint, float t, motion_track *track, s16 *cursor) {
    motion_key *k0 = 0;
    motion_key *k1 = 0;
    *cursor = func_eboot_08863660(joint, track, &k0, &k1, *cursor, t);
    if (k1 == 0) {
        return k0->value;
    }
    return spline__Ffffffff(joint, t, k0->value, k0->frame, k0->tangent_out, k1->value, k1->frame, k1->tangent_in);
}

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

// Sample every track of a joint's motion group at `frame` into out[9]
// (scale xyz, rotation xyz, position xyz). Rotations are s16 with 16384 = 2 pi,
// scale and position are fixed point with 16 = 1.0.
extern "C" int func_eboot_08863190(u8 *joint, float *out, u32 frame, s16 *cursors) {
    motion_group *g = *(motion_group **)(joint + 0x64);
    if (g != 0 && (g->flags & 0x1FF) != 0) {
        motion_track *t = g->tracks;
        for (u32 i = 0; i < (*(motion_group **)(joint + 0x64))->track_count; ++i) {
            int idx = log2table[t->type];
            float *o = &out[idx];
            *o = func_eboot_088630C8(joint, frame, t, &cursors[idx]);
            if (t->type & 0x38) {
                *o *= 3.14159265f / 8192.0f;
            } else {
                *o *= 0.0625f;
            }
            t = (motion_track *)((u8 *)t + t->size);
        }
    }
    return 1;
}

// only a rotation & translation, no scale
extern "C"
void func_eboot_088632D0(ScePspFMatrix4 *, ScePspFMatrix4 *out, ScePspFMatrix3 *args) {
    vmidt_q(out);
    eulerRotation(out, out, args->y.x, args->y.y, args->y.z);
    out->w.x = args->z.x; out->w.y = args->z.y; out->w.z = args->z.z;
}

// m = scale * rotX * rotY * rotZ with the rotation matrix built on the left (vmmul.q, full 4x4).
#define ROT4(NAME, R0, R1, R2, R3) \
inline void NAME(ScePspFMatrix4 *m, float angle) { \
    __asm__ ( \
        "lv.s S100, 0x0(%1)" \
        "vcst.s S101, VFPU_2_PI" \
        "vmul.s S100, S100, S101" \
        R0 R1 R2 R3 \
        "lv.q C100, 0x0(%0)" \
        "lv.q C110, 0x10(%0)" \
        "lv.q C120, 0x20(%0)" \
        "lv.q C130, 0x30(%0)" \
        "vmmul.q M200, M000, M100" \
        "sv.q C200, 0x0(%0)" \
        "sv.q C210, 0x10(%0)" \
        "sv.q C220, 0x20(%0)" \
        "sv.q C230, 0x30(%0)" \
        : "=m" (*m) \
        : "m" (angle) \
    ); \
}
ROT4(rotX4, "vidt.q C000", "vrot.q C010, S100, [0, C, S, 0]", "vrot.q C020, S100, [0, -S, C, 0]", "vidt.q C030")
ROT4(rotY4, "vrot.q C000, S100, [C, 0, -S, 0]", "vidt.q C010", "vrot.q C020, S100, [S, 0, C, 0]", "vidt.q C030")
ROT4(rotZ4, "vrot.q C000, S100, [C, S, 0, 0]", "vrot.q C010, S100, [-S, C, 0, 0]", "vidt.q C020", "vidt.q C030")
#undef ROT4

// Joint local matrix at `frame`: start from the rest pose (9 floats: scale xyz, rotation xyz,
// position xyz), overwrite the animated components from the joint's motion group (as in
// func_eboot_08863190), then m = scale * rotX * rotY * rotZ with the position in m->w.
// The float goes second in the declaration (EABI passes it in f12 either way); that order is what
// makes the call arguments evaluate like the original.
extern "C" void func_eboot_088633C4(u8 *joint, float frame, float *rest, float *out, ScePspFMatrix4 *m, s16 *cursors) {
    for (int i = 0; i < 9; ++i) {
        out[i] = rest[i];
    }
    motion_group *g = *(motion_group **)(joint + 0x64);
    if (g != 0) {
        u32 i;
        motion_track *t = g->tracks;
        if ((g->flags & 0x1FF) != 0) {
        for (i = 0; i < (*(motion_group **)(joint + 0x64))->track_count; ++i) {
            int idx = log2table[t->type];
            float *o = &out[idx];
            *o = func_eboot_088630C8(joint, frame, t, &cursors[idx]);
            if (t->type & 0x38) {
                *o *= 3.14159265f / 8192.0f;
            } else {
                *o *= 0.0625f;
            }
            t = (motion_track *)((u8 *)t + t->size);
        }
    }
    }
    scaleMatrix(m, out[0], out[1], out[2]);
    rotX4(m, out[3]);
    rotY4(m, out[4]);
    rotZ4(m, out[5]);
    m->w.x = out[6];
    m->w.y = out[7];
    m->w.z = out[8];
}

// Frame number of the last key of a motion track.
extern "C" float func_eboot_08863644(void *joint_0x150, motion_track *track) {
    return track->keys[track->key_count - 1].frame;
}

// Find the keys around frame t in a track, starting from the cached key index.
// Returns the new cursor; *k1 == 0 means "hold *k0" (before the first key, after the
// last one, or exactly on a key).
extern "C" s16 func_eboot_08863660(void *joint, motion_track *track, motion_key **k0, motion_key **k1, s16 cursor, float t) {
    *k0 = 0;
    *k1 = 0;
    motion_key *keys = track->keys;
    if (t <= keys[0].frame || track->key_count == 1) {
        *k0 = keys;
        *k1 = 0;
        return 0;
    }
    if (t >= keys[track->key_count - 1].frame) {
        *k0 = &keys[track->key_count - 1];
        *k1 = 0;
        return track->key_count - 1;
    }
    if (cursor < 0) {
        cursor = 0;
    } else if (cursor >= track->key_count) {
        cursor = track->key_count - 1;
    }
    motion_key *k = &keys[cursor];
    while (1) {
        if (k->frame == t) {
            *k0 = k;
            *k1 = 0;
            return cursor;
        }
        if (k->frame < t && t < k[1].frame) {
            *k0 = k;
            *k1 = k + 1;
            return cursor;
        }
        if (t < k->frame) {
            --k;
            --cursor;
        } else {
            ++k;
            ++cursor;
        }
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_088637BC);

inline void vzero_q(ScePspFVector4 *v) {
#if defined(__MWERKS__)
    __asm__ (
        "vzero.q C000"
        "sv.q C000, 0x0(%0)"
        : "=m" (*v)
    );
#else
    v->x = 0; v->y = 0; v->z = 0; v->w = 0;
#endif
}

// Root displacement of a joint between two frames of a channel's motion (joint.cpp).
extern "C" void func_eboot_0885F8E0(void *joint, ScePspFVector4 *out, int channel, float from, float to);

inline void copy_m(ScePspFMatrix4 *d, ScePspFMatrix4 *s) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C000, 0x0(%1)"
        "lv.q C010, 0x10(%1)"
        "lv.q C020, 0x20(%1)"
        "lv.q C030, 0x30(%1)"
        "sv.q C000, 0x0(%0)"
        "sv.q C010, 0x10(%0)"
        "sv.q C020, 0x20(%0)"
        "sv.q C030, 0x30(%0)"
        : "=m" (*d)
        : "m" (*s)
    );
#else
    *d = *s;
#endif
}

// v = a * wa + b * wb
inline void vblend_q(ScePspFVector4 *v, ScePspFVector4 *a, ScePspFVector4 *b, float wa, float wb) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C000, %1"
        "lv.q C010, %2"
        "lv.s S020, %3"
        "lv.s S021, %4"
        "vscl.q C100, C000, S020"
        "vscl.q C110, C010, S021"
        "vadd.q C000, C100, C110"
        "sv.q C000, %0"
        : "=m" (*v)
        : "m" (*a), "m" (*b), "m" (wa), "m" (wb)
    );
#else
    v->x = a->x * wa + b->x * wb;
    v->y = a->y * wa + b->y * wb;
    v->z = a->z * wa + b->z * wb;
    v->w = a->w * wa + b->w * wb;
#endif
}

// v = p * m (row vector times matrix, 4x4)
inline void vtfm4_q(ScePspFVector4 *v, ScePspFMatrix4 *m, ScePspFVector4 *p) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C100, %2"
        "lv.q C200, 0x0(%1)"
        "lv.q C210, 0x10(%1)"
        "lv.q C220, 0x20(%1)"
        "lv.q C230, 0x30(%1)"
        "vtfm4.q C000, E200, C100"
        "sv.q C000, %0"
        : "=m" (*v)
        : "m" (*m), "m" (*p)
    );
#else
    v->x = p->x * m->x.x + p->y * m->y.x + p->z * m->z.x + p->w * m->w.x;
    v->y = p->x * m->x.y + p->y * m->y.y + p->z * m->z.y + p->w * m->w.y;
    v->z = p->x * m->x.z + p->y * m->y.z + p->z * m->z.z + p->w * m->w.z;
    v->w = p->x * m->x.w + p->y * m->y.w + p->z * m->z.w + p->w * m->w.w;
#endif
}

// Root-motion step of channel 0: the root joint's displacement over one motion step
// (func_eboot_0885F8E0 between two frames), rotated by m with its translation row cleared.
// While blending (direction != 0) the new motion's next step and the old motion's next step are
// cross-faded with weight spline(blend_step, 0 -> 1); direction < 0 gives (0, 0, 0, 1).
extern "C" void func_eboot_08863B68(Hierarchy *h, ScePspFVector4 *out, ScePspFMatrix4 *m) {
    ScePspFVector4 c;
    ScePspFVector4 a;
    ScePspFVector4 b;
    ScePspFMatrix4 mat;
    vzero_q(out);
    copy_m(&mat, m);
    vzero_q(&mat.w);
    u8 *j = *(u8 **)((u8 *)h->roots[0] + 0x14C);
    u8 *p = j + 0x150;
    if (h->motion[0].direction != 0) {
        func_eboot_0885F8E0(j, &a, 0, h->motion[0].frame, h->motion[0].frame + h->motion[0].speed);
        float w = spline__Ffffffff(p, h->motion[0].blend_step, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
        float iw = 1.0f - w;
        func_eboot_0885F8E0(*(u8 **)((u8 *)h->roots[0] + 0x14C), &b, 1, h->motion[0].blend_frame, h->motion[0].blend_frame + h->motion[0].speed);
        vblend_q(&c, &a, &b, w, iw);
        if (h->motion[0].direction > 0) {
            vtfm4_q(out, &mat, &c);
        } else {
            sv_q(out, 0.0f, 0.0f, 0.0f, 1.0f);
        }
    } else {
        func_eboot_0885F8E0(j, &c, 0, h->motion[0].frame - h->motion[0].speed, h->motion[0].frame);
        vtfm4_q(out, &mat, &c);
    }
}

extern "C" void func_eboot_0885F998(Joint *, void *motion, int channel, int);
extern "C" float func_eboot_08860254(Joint *, void *motion);
extern "C" void func_eboot_08864234(Hierarchy *, void *motion, int channel, float frame);
extern "C" void func_eboot_08863E68(Hierarchy *, void *motion, int frame, int blend, int channel);


// Set the motion of one channel without blending.
extern "C" void func_eboot_08863D50(Hierarchy *h, void *motion, int channel, float frame) {
    if (motion != 0) {
        h->unknown_0x128[channel] = 0;
        h->unknown_0x130[channel] = 0;
        func_eboot_08864234(h, motion, channel, frame);
        for (int i = 0; i < h->root_count; ++i) {
            func_eboot_0885F998(h->roots[i], motion, channel, 0);
        }
    }
}

// Set the motion of one channel, blending from the current one over `blend` frames.
extern "C" void func_eboot_08863DEC(Hierarchy *h, void *motion, int blend, int channel, float frame) {
    if (blend != 0 && h->motion[channel].motion == 0) {
        blend = 0;
    }
    if (blend == 0) {
        func_eboot_08863D50(h, motion, channel, frame);
    } else if (motion != 0) {
        func_eboot_08863E68(h, motion, (int)frame, blend, channel);
    } else {
        h->unknown_0x128[channel] = 0;
        h->unknown_0x130[channel] = 0;
    }
}

#ifdef BUILD_NONMATCHING
// 97.78%: the original keeps &motion[channel] in v0 for the second group of stores and computes
// &blend_frame (for the 0885FB4C argument) before the direction branch; ours keeps it in a0 and
// computes it in the branch delay slot, so the likely-branch preload of chain_count is lost.
extern "C" void func_eboot_0885FB4C(Joint *, int channel, float frame);
extern "C" void func_eboot_0885FBAC(Joint *, int chain);
#define M h->motion[channel]
#define MSTART(m) (*(float *)((u8 *)(m) + 0x10))
#define MLOOP(m) (*(u32 *)((u8 *)(m) + 0xC))
extern "C" void func_eboot_08863E68(Hierarchy *h, void *motion, int frame, int blend, int channel) {
    if (blend != 0) {
        if (M.blending == 0) {
            func_eboot_0885F998(h->roots[0], M.motion, channel, 1);
            M.blend_end = M.end;
            if (M.flags & Hierarchy::Motion::LOOP) {
                M.flags |= Hierarchy::Motion::BLEND_LOOP;
            } else {
                M.flags &= ~Hierarchy::Motion::BLEND_LOOP;
            }
            M.blend_start = M.start;
            float *bf = &M.blend_frame;
            *bf = M.frame;
            M.blending = 1;
            if (M.direction == 0) {
                func_eboot_0885FB4C(h->roots[0], channel, *bf);
            } else {
                for (int i = 0; i < h->chain_count; ++i) {
                    func_eboot_0885FBAC(h->roots[0], i);
                }
            }
        }
        if (blend < 0) {
            blend = -blend;
            M.direction = -1;
        } else {
            M.direction = 1;
        }
        int n = blend + 1;
        func_eboot_0885F998(h->roots[0], motion, channel, 0);
        M.end = func_eboot_08860254(h->roots[channel], motion);
        if (MLOOP(motion) != 0) {
            M.flags |= Hierarchy::Motion::LOOP;
        } else {
            M.flags &= ~Hierarchy::Motion::LOOP;
        }
        M.start = MSTART(motion);
        M.frame = frame;
        M.motion = motion;
        M.blend_frames = n;
        M.blend_step = M.blend_speed = 1.0f / n;
    } else {
        func_eboot_0885F998(h->roots[0], motion, channel, 0);
        M.end = func_eboot_08860254(h->roots[channel], motion);
        if (MLOOP(motion) != 0) {
            M.flags |= Hierarchy::Motion::LOOP;
        } else {
            M.flags &= ~Hierarchy::Motion::LOOP;
        }
        M.start = MSTART(motion);
        M.frame = frame;
        M.motion = motion;
        M.blend_frames = 0;
        M.direction = 0;
    }
    M.flags |= Hierarchy::Motion::ACTIVE;
    if (channel == 0) {
        vzero_q(&h->motion[0].root_delta);
    }
}
#undef M
#undef MSTART
#undef MLOOP
#else
INCLUDE_ASM("asm/eboot/nonmatchings/model_base", func_eboot_08863E68);
#endif

// Update every joint chain: root->update(transform or identity, identity, x, y, z).
extern "C" void func_eboot_088640F0(Hierarchy *h, ScePspFMatrix4 *transform, float x, float y, float z) {
    ScePspFMatrix4 identity;
    vmidt_q(&identity);
    if (transform == 0) {
        transform = &identity;
    }
    for (int i = 0; i < h->root_count; ++i) {
        h->roots[i]->update(transform, &identity, x, y, z);
    }
}

extern "C" void func_eboot_08860640(Joint *);

extern "C" void func_eboot_088641B8(Hierarchy *h) {
    for (int i = 0; i < h->root_count; ++i) {
        func_eboot_08860640(h->roots[i]);
    }
}

// Joint `index` of the joint array (all joints are allocated contiguously after roots[0]).
extern "C" Joint *func_eboot_08864214(Hierarchy *h, int index) {
    return &h->roots[0][index];
}

// Start `motion` on `channel` at `frame` (no blend): speed 2, start/end from the data.
extern "C" void func_eboot_08864234(Hierarchy *h, void *motion, int channel, float frame) {
    h->motion[channel].flags |= Hierarchy::Motion::ACTIVE;
    h->motion[channel].frame = frame;
    h->motion[channel].speed = 2.0f;
    h->motion[channel].start = *(float *)((u8 *)motion + 0x10);
    h->motion[channel].end = func_eboot_08860254(h->roots[0], motion);
    if (*(u32 *)((u8 *)motion + 0xC) != 0) {
        h->motion[channel].flags |= Hierarchy::Motion::LOOP;
    } else {
        h->motion[channel].flags &= ~Hierarchy::Motion::LOOP;
    }
    h->motion[channel].motion = motion;
    if (channel == 0) {
        vzero_q(&h->motion[channel].root_delta);
    }
}

extern "C" void func_eboot_088642F4(Hierarchy *h) {
    for (int i = 0; i < 4; ++i) {
        h->roots[i] = 0;
    }
    for (int i = 0; i < 4; ++i) {
        h->unknown_0x128[i] = 0;
        h->unknown_0x130[i] = 0;
        h->unknown_0x138[i] = 0;
    }
    h->root_count = 0;
    h->chain_count = 0;
}

// True when frame t is crossed by the next update of `channel` (event trigger test).
extern "C" int func_eboot_08864340(Hierarchy *h, int channel, float t) {
    int wrapped = 0;
    if (h->motion[0].direction != 0) {
        return 0;
    }
    float frame = h->motion[channel].frame;
    float next = frame + h->motion[channel].speed;
    float end = h->motion[channel].end;
    if (next > end) {
        if (h->motion[channel].flags & Hierarchy::Motion::LOOP) {
            next = h->motion[channel].start + (next - end);
            wrapped = 1;
        } else {
            next = end;
        }
    }
    if (!wrapped) {
        if (t >= frame && t < next) {
            return 1;
        }
    } else {
        if (t >= frame && next < end) {
            return 1;
        }
    }
    return 0;
}

// True once `channel` has reached frame t.
extern "C" bool func_eboot_08864400(Hierarchy *h, int channel, float t) {
    if (h->motion[0].direction != 0) {
        return false;
    }
    if (t <= h->motion[channel].frame) {
        return true;
    }
    return false;
}
