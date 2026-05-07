#include "draw_manager.hpp"

#include "camera.hpp"
#include "singleton.hpp"
#include "immediate_ge.hpp"
#include "draw.hpp"
#include "model_base.hpp"
#include "stage_manager.hpp"
#include "vram_manager.hpp"
#include "vfpu.h"
#include "game_sys.hpp"

#pragma opt_unroll_loops on

using namespace immediate_ge;

template<> DrawManager *Singleton<DrawManager>::objectPtr;
u32 *DRAWABLE_WRITE_HEAD;
u16 D_eboot_089C70D4[2];
ScePspFVector3 D_eboot_089C70D8;
float D_eboot_089C70E4;
float D_eboot_089C70E8;

DrawManager::DrawManager() {
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

DrawManager::~DrawManager() {
    // empty
}

void DrawManager::reset() {
    clear();
    if (start_fragment(render_group::RESET)) {
        Camera *camera = Camera::objectPtr;
        ge::viewportscale(camera->viewport_scale);
        ge::viewportcenter(camera->viewport_center);

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
        ge::texwrap(GE_TWRAP_WRAP, GE_TWRAP_WRAP);
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
        ge::fog(incolor.f, 65000.0f, 65000.0f);

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

void DrawManager::clear() {
    for (int i = 0; i < render_group::GROUP_COUNT; ++i) {
        for (int j = 0; j < z_index_bucket_length[i]; ++j) {
            z_index_buckets[i][j] = 0;
        }
    }
}

void DrawManager::draw() {
    for (int i = 0; i < render_group::GROUP_COUNT; ++i) {
        if (start_fragment(i)) {
            if (i == 1) {
                ScePspFMatrix4 m;
                vmidt_q(&m);
                ge::view(&m);
                ge::projection(&Camera::objectPtr->projection);
            }

            for (int j = 0; j < z_index_bucket_length[i]; ++j) {
                Draw *object = z_index_buckets[i][j];
                while (object != NULL) {
                    object->draw();
                    object = object->next;
                }
            }

            if (i == 8 && GameSys::objectPtr->stage_id != 0) {
                StageBase *stage = StageManager::objectPtr->stage;
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

bool DrawManager::vram_transfer() {
    VramAllocation allocation;
    u32 display_list[16];

    VramManager::objectPtr->method_08813364(Ge::objectPtr->active_buffer ^ 1, &allocation);

    // TODO: immediate_ge with destination parameter
    u16 transfer_height = 272;
    u16 transfer_width = 256;
    u32 *write_head = &display_list[0];
    *write_head++ = GE_CMD_TEXSYNC << 24;
    *write_head++ = (GE_CMD_TRANSFERSRC << 24) | ((u32)allocation.texture.vramAddress & 0xFFFFFF);
    *write_head++ = (GE_CMD_TRANSFERSRCW << 24) | (((u32)allocation.texture.vramAddress & 0xFF000000)) >> 8 | transfer_width;
    *write_head++ = (GE_CMD_TRANSFERDST << 24) | ((u32)vram_transfer_dst & 0xFFFFFF);
    *write_head++ = (GE_CMD_TRANSFERDSTW << 24) | (((u32)vram_transfer_dst & 0xFF000000)) >> 8 | transfer_width;
    *write_head++ = GE_CMD_TRANSFERSRCPOS << 24;
    *write_head++ = GE_CMD_TRANSFERDSTPOS << 24;
    *write_head++ = (GE_CMD_TRANSFERSIZE << 24) | ((transfer_height - 1) << 10) | (transfer_width - 1);
    *write_head++ = (GE_CMD_TRANSFERSTART << 24) | GE_TRANSFER_BPP_4;
    *write_head++ = GE_CMD_TEXSYNC << 24;
    *write_head++ = GE_CMD_BASE << 24;
    *write_head++ = GE_CMD_JUMP << 24;

    return Ge::objectPtr->method_088593A0(display_list, 12, vram_transfer_fragment_index);
}

void DrawManager::initialize() {
    if (start_fragment(render_group::RESET)) {
        ge::ztestenable(true);
        ge::ztest(GE_OP_AT_MOST);

        ge::alphatestenable(true);

        ge::texmapmode(GE_TEXMAP_TEXTURE_COORDS, GE_PROJMAP_POSITION);
        ge::texfilter(GE_TFILT_LINEAR_MIPMAP_LINEAR, GE_TFILT_LINEAR);
        ge::texfunc(GE_TEXFUNC_MODULATE, GE_TEXFUNC_COMPONENTS_RGBA);
        ge::texwrap(GE_TWRAP_WRAP, GE_TWRAP_WRAP);
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
        ge::texwrap(GE_TWRAP_WRAP, GE_TWRAP_WRAP);
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

void DrawManager::dither_matrix(u8 i) {
    ge::ditherenable(true);
    i *= 4;
    ge::impl::emit(dither_matrices[i]);
    ge::impl::emit(dither_matrices[i + 1]);
    ge::impl::emit(dither_matrices[i + 2]);
    ge::impl::emit(dither_matrices[i + 3]);
}

bool DrawManager::start_fragment(u8 group) {
    if ((bool)writing != false) {
        end_fragment();
    }
    DRAWABLE_WRITE_HEAD = Ge::objectPtr->write_head();
    if (DRAWABLE_WRITE_HEAD != NULL) {
        writing = true;
        fragment_start = DRAWABLE_WRITE_HEAD;
        fragment_group = group;
    }
    return writing != 0;
}

void DrawManager::end_fragment() {
    if ((bool)writing != 0) {
        int length = DRAWABLE_WRITE_HEAD - fragment_start;
        if (length != 0) {
            DRAWABLE_WRITE_HEAD += 2;
            Ge::objectPtr->method_088595E8(fragment_start, length + 2, fragment_group);
            Ge::objectPtr->set_write_head(DRAWABLE_WRITE_HEAD);
        }
        DRAWABLE_WRITE_HEAD = NULL;
        fragment_start = NULL;
        writing = false;
    }
}

inline float max(float x, float y) {
    float result = x;
    if (result < y) {
        result = y;
    }
    return result;
}

int DrawManager::addObj(u8 group, ObjBase *character, bool no_culling) {
    int result;
    do {
        character->flags &= ~Draw::VISIBLE;
        if (no_culling == false) {
            float s = character->scale.x;
            s = max(s, character->scale.y);
            s = max(s, character->scale.z);

            float z = character->position.z;
            float y = character->position.y + (0.5f * (s * character->model_pmo.header->scale.y));
            float x = character->position.x;
            float w = 0;

            ScePspFVector4 position;
            sv_q(&position, x, y, z, 0);

            result = func_eboot_08816EA8(Camera::objectPtr, &position, s * character->model_pmo.header->clipping_distance);
            if ((u8)result == false) {
                break;
            }
        }
        if (character->alpha != 0xFF && group == render_group::GROUP_5) {
            group = render_group::GROUP_7;
        }
        result = add(group, (Draw*)character, &character->position, no_culling);
        if ((u8)result == true) {
            character->flags |= Draw::VISIBLE;
        }
    } while(0);
    return result;
}

int DrawManager::add(u8 group, ModelBase *model, bool no_culling) {
    int result;
    model->flags &= ~Draw::VISIBLE;
    if (no_culling != false ||
        (result = func_eboot_08816EA8(Camera::objectPtr, &model->transform.w, model->model_pmo.header->clipping_distance), (u8)result != false)) {
        result = add(group, model, &model->transform.w, no_culling);
        if ((u8)result == true) {
            model->flags |= Draw::VISIBLE;
        }
    }
    return result;
}

inline Draw **DrawManager::head(u8 group, int index) {
    return &z_index_buckets[group][(z_index_bucket_length[group] - 1) - index];
}

int DrawManager::add(u8 group, Draw *object, ScePspFVector4 *position, bool no_culling) {
    if (0 > group || group >= render_group::GROUP_COUNT) {
        return false;
    }
    object->next = NULL;

    float zindex =
        position->x * Camera::objectPtr->perspective.x.z +
        position->y * Camera::objectPtr->perspective.y.z +
        position->z * Camera::objectPtr->perspective.z.z +
                      Camera::objectPtr->perspective.w.z ;
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

    int index = z_index_bucket_length[group] * ((zindex - Camera::objectPtr->near_z) / (Camera::objectPtr->far_z - Camera::objectPtr->near_z));
    if (index >= z_index_bucket_length[group]) {
        return false;
    }

    Draw *next = *head(group, index);
    if (group == render_group::GROUP_7) {
        Draw *cur = next;
        Draw *prev = NULL;
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

bool DrawManager::queue_vram_transfer(void *dst, u8 fragment_index) {
    if (vram_transfer_queued == false) {
        vram_transfer_fragment_index = fragment_index;
        vram_transfer_dst = dst;
        vram_transfer_queued = true;
        return vram_transfer_queued;
    }
    return false;
}

void DrawManager::world_model(ScePspFMatrix4 *transform) {
    ScePspFMatrix4 m;
    float norm = Ge::objectPtr->norm;
    scaleMatrix(&m, norm, norm, norm);
    vmmul_q(&m, transform, &m);
    vmmul_q(&m, &m, &Camera::objectPtr->world);

    ge::world(&m);

}
