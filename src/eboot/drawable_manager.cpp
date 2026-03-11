#include "common.h"
#include "singleton.hpp"
#include "immediate_ge.hpp"

#include "drawable.hpp"
#include "model.hpp"
#include "drawable_manager.hpp"
#include "vfpu.h"

#pragma opt_unroll_loops on

using namespace immediate_ge;

drawable_manager *singleton<drawable_manager>::INSTANCE;
u32 *DRAWABLE_WRITE_HEAD;
u16 D_eboot_089C70D4[2];
ScePspFVector3 D_eboot_089C70D8;
float D_eboot_089C70E4;
float D_eboot_089C70E8;

drawable_manager::drawable_manager() {
    z_index_buckets[render_group::GROUP_0] = NULL;
    z_index_buckets[render_group::RESET] = NULL;
    z_index_buckets[render_group::GROUP_2] = &z_index[0];
    z_index_buckets[render_group::STAGE] = &z_index[1];
    z_index_buckets[render_group::GROUP_4] = &z_index[2];
    z_index_buckets[render_group::GROUP_5] = &z_index[3];
    z_index_buckets[render_group::GROUP_6] = &z_index[35];
    z_index_buckets[render_group::GROUP_7] = &z_index[43];
    z_index_buckets[render_group::GROUP_8] = &z_index[107];
    z_index_buckets[render_group::GROUP_9] = &z_index[108];
    z_index_buckets[render_group::GROUP_10] = NULL;
    z_index_buckets[render_group::GROUP_11] = NULL;
    z_index_buckets[render_group::GROUP_12] = NULL;
    z_index_buckets[render_group::GROUP_13] = NULL;
    z_index_buckets[render_group::GROUP_14] = NULL;
    z_index_buckets[render_group::GROUP_15] = NULL;
    z_index_buckets[render_group::GROUP_16] = &z_index[109];
    z_index_buckets[render_group::GROUP_17] = NULL;
    z_index_buckets[render_group::GROUP_18] = &z_index[110];
    z_index_buckets[render_group::GROUP_19] = NULL;
    writing = 0;
    DRAWABLE_WRITE_HEAD = NULL;
    fragment_start = NULL;
    flag_0x214 = false;
}

drawable_manager::~drawable_manager() {
    // empty
}

extern "C" {
    // camera?
    extern struct global_089C6CB4 {
        float near_z;
        float far_z;
        u8 padding_0x8[0xB80 - 0x8];
        ScePspFMatrix4 perspective;
        ScePspFMatrix4 world;
        u8 padding_0xBC0[0xDA0 - 0xC00];
        ScePspVector3 viewport_scale;
        ScePspVector3 viewport_center;
    } *D_eboot_089C6CB4;

    // clipping test; objects are clipped when false
    int func_eboot_08816EA8(global_089C6CB4 *, ScePspFVector4 *position, float clipping_distance);
}

void drawable_manager::reset() {
    clear();
    if (start_fragment(render_group::RESET)) {
        global_089C6CB4 *global = D_eboot_089C6CB4;
        ge::viewportscale(global->viewport_scale);
        ge::viewportcenter(global->viewport_center);

        ge::minz(0x0000);
        ge::maxz(0xFFFF);

        ge::scissor(0, 0, 480, 272);

        ge::offset(1808 << 4, 1912 << 4);

        ge::alphatestenable(true);
        ge::atest(0xFF, 0xC0, GE_OP_AT_LEAST);

        ge::alphablendenable(true);
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_SRCBLEND_INVSRCALPHA);

        ge::colortestenable(false);
        ge::colortest(GE_OP_NEVER);
        ge::colortestmask(0xFF, 0xFF, 0xFF);

        ge::stenciltestenable(false);

        ge::ztestenable(true);
        ge::ztest(GE_OP_AT_MOST);

        ge::ditherenable(false);
        dither_matrix(0);

        ge::antialiasenable(false);
        ge::logicopenable(false);
        ge::cullfaceenable(false);
        ge::depthclampenable(true);

        ge::shademode(GE_SHADE_GOURAUD);

        ge::ambientcolor(0, 0, 0);
        ge::ambientalpha(0xFF);

        ge::materialemissive(0, 0, 0);
        ge::materialspecular(0, 0, 0);
        ge::materialspecularcoef();

        ge::texmapmode(GE_TEXMAP_TEXTURE_COORDS, GE_PROJMAP_POSITION);
        ge::texfilter(GE_TFILT_LINEAR, GE_TFILT_LINEAR);
        ge::texfunc(GE_TEXFUNC_MODULATE, GE_TEXFUNC_COMPONENTS_RGBA);
        ge::texwrap();
        ge::texoffset();

        ScePspUnion32 scale; scale.f = 1.0f;
        ge::texscale(scale);

        ge::lightmode(GE_LIGHTMODE_SINGLECOLOR);
        ge::lightingenable(false);
        for (int i = 0; i < 4; ++i) {
            ge::lightenable(i, i < 3);
            ge::lightdiffusecolor(i, 0, 0, 0);
            ge::lightambientcolor(i, 0, 0, 0);
            ge::lightspecularcolor(i, 0, 0, 0);
        }

        ge::fogenable(false);

        ScePspUnion32 incolor; incolor.ui = 0;
        float distance = 65000.0f * ge_manager::get()->norm;
        ge::fog(incolor.f, distance, distance);

        end_fragment();
    }
}

s8 z_index_bucket_length[render_group::GROUP_COUNT] = {
    [render_group::GROUP_2] = 1,
    [render_group::STAGE] = 1,
    [render_group::GROUP_4] = 1,
    [render_group::GROUP_5] = 32,
    [render_group::GROUP_6] = 8,
    [render_group::GROUP_7] = 64,
    [render_group::GROUP_8] = 1,
    [render_group::GROUP_9] = 1,
    [render_group::GROUP_16] = 1,
    [render_group::GROUP_18] = 1,
};

void drawable_manager::clear() {
    for (int i = 0; i < render_group::GROUP_COUNT; ++i) {
        for (int j = 0; j < z_index_bucket_length[i]; ++j) {
            z_index_buckets[i][j] = 0;
        }
    }
}

#define DITHER_SCALAR(a) ((u8)a & 0xF)
#define DITHER_ROW(a, b, c, d) ((DITHER_SCALAR(a) << 12) | (DITHER_SCALAR(b) << 8) | (DITHER_SCALAR(c) << 4) | DITHER_SCALAR(d))
#define DITHER_MATRIX(xx, xy, xz, xw, \
                      yx, yy, yz, yw, \
                      zx, zy, zz, zw, \
                      wx, wy, wz, ww) \
      ((GE_CMD_DITH0 << 24) | DITHER_ROW(xx, xy, xz, xw)), \
      ((GE_CMD_DITH1 << 24) | DITHER_ROW(yx, yy, yz, yw)), \
      ((GE_CMD_DITH2 << 24) | DITHER_ROW(zx, zy, zz, zw)), \
      ((GE_CMD_DITH3 << 24) | DITHER_ROW(wx, wy, wz, ww))


extern "C" {
// main loop; draws the 3d scene; iterates by group, calls #draw() of z-index bucketed drawables
INCLUDE_ASM("asm/eboot/nonmatchings/drawable_manager", func_eboot_0884BA5C);

// texture block transfer display list; refers to the unknown fields, possibly ui textures?
INCLUDE_ASM("asm/eboot/nonmatchings/drawable_manager", func_eboot_0884C02C);

// like reset, display lists for render groups 1, 2, 5, 6, 18
// called during startup and from edit_task
INCLUDE_ASM("asm/eboot/nonmatchings/drawable_manager", func_eboot_0884C130);
}

u32 dither_matrices[3*4] = {
    DITHER_MATRIX(  0, -1,  0, -1,
                   -1,  0, -1,  1,
                    0, -1,  0, -1,
                    0,  0, -1,  1  ),
    DITHER_MATRIX( -2,  0, -2,  0,
                   -1,  1, -1,  1,
                   -2,  0, -2,  0,
                   -1,  1, -1,  1  ),
    DITHER_MATRIX(  1, -3,  0, -4,
                   -1,  3, -2,  2,
                    0, -4,  1, -3,
                   -2,  2, -1,  3  ) };

void drawable_manager::dither_matrix(u8 i) {
    ge::ditherenable(true);
    i *= 4;
    ge::impl::emit(dither_matrices[i]);
    ge::impl::emit(dither_matrices[i + 1]);
    ge::impl::emit(dither_matrices[i + 2]);
    ge::impl::emit(dither_matrices[i + 3]);
}

bool drawable_manager::start_fragment(u8 group) {
    if ((bool)writing != false) {
        end_fragment();
    }
    DRAWABLE_WRITE_HEAD = ge_manager::get()->write_head();
    if (DRAWABLE_WRITE_HEAD != NULL) {
        writing = true;
        fragment_start = DRAWABLE_WRITE_HEAD;
        fragment_group = group;
    }
    return writing != 0;
}

extern "C"
void func_eboot_088595E8(ge_manager *, ge_command *, int, u32);

void drawable_manager::end_fragment() {
    if ((bool)writing != 0) {
        int length = DRAWABLE_WRITE_HEAD - fragment_start;
        if (length != 0) {
            DRAWABLE_WRITE_HEAD += 2;
            func_eboot_088595E8(ge_manager::get(), fragment_start, length + 2, fragment_group);
            ge_manager::get()->set_write_head(DRAWABLE_WRITE_HEAD);
        }
        DRAWABLE_WRITE_HEAD = NULL;
        fragment_start = NULL;
        writing = false;
    }
}

extern "C" {
// add for player/em objects
INCLUDE_ASM("asm/eboot/nonmatchings/drawable_manager", func_eboot_0884C8BC);
}

int drawable_manager::add(u8 group, model *model, bool no_culling) {
    int result;
    model->flags &= ~0x2;
    if (no_culling != false ||
        (result = func_eboot_08816EA8(D_eboot_089C6CB4, &model->transform.w, model->model_pmo.header->clipping_distance), (u8)result != false)) {
        result = add(group, model, &model->transform.w, no_culling);
        if ((u8)result == true) {
            model->flags |= 2;
        }
    }
    return result;
}

inline drawable **drawable_manager::head(u8 group, int index) {
    return &z_index_buckets[group][(z_index_bucket_length[group] - 1) - index];
}

int drawable_manager::add(u8 group, drawable *object, ScePspFVector4 *position, bool no_culling) {
    if (0 > group || group >= render_group::GROUP_COUNT) {
        return false;
    }
    object->next = NULL;

    float zindex =
        position->x * D_eboot_089C6CB4->perspective.x.z +
        position->y * D_eboot_089C6CB4->perspective.y.z +
        position->z * D_eboot_089C6CB4->perspective.z.z +
                      D_eboot_089C6CB4->perspective.w.z ;
    zindex *= -1;
    if (zindex < 0.0f) {
        if (no_culling == true) {
            zindex = 0.0f;
        } else {
            if (zindex < -1600.0f) {
                return false;
            } else {
                zindex = 0.0f;
            }
        }
    }
    object->zindex = zindex;

    int index = z_index_bucket_length[group] * ((zindex - D_eboot_089C6CB4->near_z) / (D_eboot_089C6CB4->far_z - D_eboot_089C6CB4->near_z));
    if (index >= z_index_bucket_length[group]) {
        return false;
    }

    drawable *next = *head(group, index);
    if (group == render_group::GROUP_7) {
        drawable *cur = next;
        drawable *prev = NULL;
        if (cur != NULL) {
            while (true) {
                if (zindex >= cur->zindex) {
                    object->next = cur;
                    if (prev != NULL) {
                        prev->next = object;
                    } else {
                        *head(group, index) = object;
                    }
                    break;
                }
                if (cur->next == NULL) {
                    cur->next = object;
                    break;
                }
                prev = cur;
                cur = cur->next;
            }
        } else {
            *head(group, index) = object;
        }
    } else {
        object->next = next;
        *head(group, index) = object;
    }

    return true;
}

bool drawable_manager::method_0884CCC0(void *unknown_data, u8 unknown_index) {
    if (flag_0x214 == false) {
        unknown_0x215 = unknown_index;
        unknown_0x218 = unknown_data;
        flag_0x214 = true;
        return flag_0x214;
    }
    return false;
}

void drawable_manager::world_model(ScePspFMatrix4 *transform) {
    ScePspFMatrix4 m;
    float norm = ge_manager::get()->norm;
    scaleMatrix(&m, norm, norm, norm);
    vmmul_q(&m, transform, &m);
    vmmul_q(&m, &m, &D_eboot_089C6CB4->world);

    ge::world(&m);

}
