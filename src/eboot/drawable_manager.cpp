#include "common.h"
#include "singleton.hpp"
#include "immediate_ge.hpp"

#include "drawable.hpp"
#include "model.hpp"
#include "drawable_manager.hpp"
#include "stage_manager.hpp"
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
    vram_transfer_queued = false;
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
        ScePspFMatrix4 perspective; // unsure
        ScePspFMatrix4 world;
        ScePspFMatrix4 projection;
        u8 padding_0xC40[0xDA0 - 0xC40];
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
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);

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

extern "C" {
    extern struct global_089C7508 {
        u8 padding_0x0[0x6AF0E];
        u16 stage_id;
    } *D_eboot_089C7508;
}

void drawable_manager::draw() {
    for (int i = 0; i < render_group::GROUP_COUNT; ++i) {
        if (start_fragment(i)) {
            if (i == 1) {
                ScePspFMatrix4 m;
                vmidt_q(&m);
                ge::view(&m);
                ge::projection(&D_eboot_089C6CB4->projection);
            }

            for (int j = 0; j < z_index_bucket_length[i]; ++j) {
                drawable *object = z_index_buckets[i][j];
                while (object != NULL) {
                    object->draw();
                    object = object->next;
                }
            }

            if (i == 8 && D_eboot_089C7508->stage_id != 0) {
                base_stage *stage = stage_manager::get()->stage;
                if (stage != 0 && stage->flag_0x3D4 == true) {
                    stage->method_088CD61C();
                }
            }

            end_fragment();
        }
    }

    if (vram_transfer_queued == true) {
        vram_transfer();
        vram_transfer_queued = false;
    }
}

extern "C" {
    struct vram_transfer_request {
        u8 pad_0x0[0x8];
        void *src;
        u8 pad_0xC[0xC];
    };
    extern void *D_eboot_089C6CB0;
    bool func_eboot_08813364(void *global_089C6CB0, u8 buffer_index, vram_transfer_request *out);

    // copies a display list fragment to the write head and calls that copy at the specified index
    bool func_eboot_088593A0(ge_manager *, u32 *display_list, s32 length, s32 fragment_index);
}

bool drawable_manager::vram_transfer() {
    vram_transfer_request req;
    u32 display_list[16];

    func_eboot_08813364(D_eboot_089C6CB0, ge_manager::get()->active_buffer ^ 1, &req);

    // TODO: immediate_ge with destination parameter
    u16 transfer_height = 272;
    u16 transfer_width = 256;
    u32 *write_head = &display_list[0];
    *write_head++ = GE_CMD_TEXSYNC << 24;
    *write_head++ = (GE_CMD_TRANSFERSRC << 24) | ((u32)req.src & 0xFFFFFF);
    *write_head++ = (GE_CMD_TRANSFERSRCW << 24) | (((u32)req.src & 0xFF000000)) >> 8 | transfer_width;
    *write_head++ = (GE_CMD_TRANSFERDST << 24) | ((u32)vram_transfer_dst & 0xFFFFFF);
    *write_head++ = (GE_CMD_TRANSFERDSTW << 24) | (((u32)vram_transfer_dst & 0xFF000000)) >> 8 | transfer_width;
    *write_head++ = GE_CMD_TRANSFERSRCPOS << 24;
    *write_head++ = GE_CMD_TRANSFERDSTPOS << 24;
    *write_head++ = (GE_CMD_TRANSFERSIZE << 24) | ((transfer_height - 1) << 10) | (transfer_width - 1);
    *write_head++ = (GE_CMD_TRANSFERSTART << 24) | GE_TRANSFER_BPP_4;
    *write_head++ = GE_CMD_TEXSYNC << 24;
    *write_head++ = GE_CMD_BASE << 24;
    *write_head++ = GE_CMD_JUMP << 24;

    return func_eboot_088593A0(ge_manager::get(), display_list, 12, vram_transfer_fragment_index);
}

void drawable_manager::initialize() {
    if (start_fragment(render_group::RESET)) {
        ge::ztestenable(true);
        ge::ztest(GE_OP_AT_MOST);

        ge::alphatestenable(true);

        ge::texmapmode(GE_TEXMAP_TEXTURE_COORDS, GE_PROJMAP_POSITION);
        ge::texfilter(GE_TFILT_LINEAR_MIPMAP_LINEAR, GE_TFILT_LINEAR);
        ge::texfunc(GE_TEXFUNC_MODULATE, GE_TEXFUNC_COMPONENTS_RGBA);
        ge::texwrap();
        ge::texoffset();

        ge::depthclampenable(true);
        ge::cullfaceenable(false);
        ge::fogenable(true);

        ge::shademode(GE_SHADE_GOURAUD);

        ge::ambientcolor(0, 0, 0);
        ge::ambientalpha(0xFF);

        ge::blendfixeda(0xFF, 0xFF, 0xFF);
        ge::blendfixedb(0xFF, 0xFF, 0xFF);

        end_fragment();
    }

    if (start_fragment(render_group::GROUP_2)) {
        ge::atest(0xFF, 0x40, GE_OP_AT_LEAST);
        ge::alphablendenable(true);
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);

        ge::lightingenable(false);

        end_fragment();
    }

    if (start_fragment(render_group::GROUP_5)) {
        ge::atest(0xFF, 0xC0, GE_OP_AT_LEAST);
        ge::alphablendenable(false);
        ge::lightingenable(true);

        end_fragment();
    }

    if (start_fragment(render_group::GROUP_6)) {
        ge::atest(0xFF, 0x80, GE_OP_AT_LEAST);
        ge::alphablendenable(true);
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);

        ge::lightingenable(false);

        end_fragment();
    }

    if (start_fragment(render_group::GROUP_18)) {
        ge::ztestenable(true);
        ge::ztest(GE_OP_AT_MOST);

        ge::alphatestenable(true);

        ge::texmapmode(GE_TEXMAP_TEXTURE_COORDS, GE_PROJMAP_POSITION);
        ge::texfilter(GE_TFILT_LINEAR, GE_TFILT_LINEAR);
        ge::texfunc(GE_TEXFUNC_MODULATE, GE_TEXFUNC_COMPONENTS_RGBA);
        ge::texwrap();
        ge::texoffset();

        ge::depthclampenable(true);
        ge::cullfaceenable(false);
        ge::lightingenable(false);

        end_fragment();
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

inline float min(float x, float y) {
    return (x < y) ? x : y;
}

int drawable_manager::add(u8 group, character *character, bool no_culling) {
    int result;
    do {
        character->flags &= ~drawable::VISIBLE;
        if (no_culling == false) {
            float s = character->scale.x;
            if (s < character->scale.y) {
                s = character->scale.y;
            }
            if (s < character->scale.z) {
                s = character->scale.z;
            }
            float z = character->position.z;
            float y = character->position.y + (0.5f * (s * character->model_pmo.header->scale.y));
            float x = character->position.x;
            float w = 0;

            ScePspFVector4 position;
            sv_q(&position, x, y, z, 0);
            result = func_eboot_08816EA8(D_eboot_089C6CB4, &position, s * character->model_pmo.header->clipping_distance);
            if ((u8)result == false) {
                break;
            }
        }
        if (character->alpha != 0xFF && group == render_group::GROUP_5) {
            group = render_group::GROUP_7;
        }
        result = add(group, (drawable*)character, &character->position, no_culling);
        if ((u8)result == true) {
            character->flags |= drawable::VISIBLE;
        }
    } while(0);
    return result;
}

int drawable_manager::add(u8 group, model *model, bool no_culling) {
    int result;
    model->flags &= ~drawable::VISIBLE;
    if (no_culling != false ||
        (result = func_eboot_08816EA8(D_eboot_089C6CB4, &model->transform.w, model->model_pmo.header->clipping_distance), (u8)result != false)) {
        result = add(group, model, &model->transform.w, no_culling);
        if ((u8)result == true) {
            model->flags |= drawable::VISIBLE;
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

bool drawable_manager::queue_vram_transfer(void *dst, u8 fragment_index) {
    if (vram_transfer_queued == false) {
        vram_transfer_fragment_index = fragment_index;
        vram_transfer_dst = dst;
        vram_transfer_queued = true;
        return vram_transfer_queued;
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
