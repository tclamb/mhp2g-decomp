#include "draw_manager.hpp"
#include "camera.hpp"
#include "immediate_ge.hpp"
#include "vfpu.h"
using namespace immediate_ge;

struct Prop943Params {
    s16 first_mesh;
    u16 mode;
};
struct Prop943Layers {
    s16 count;
    u16 padding;
    u32 flags[3];
};
struct Prop943Draw {
    u8 padding[0x2C];
    pmo *model;
    tmh *texture;
    Prop943Params *params;
    u8 pad38[8];
    ScePspFVector4 position;
    u8 pad50[20];
    float alpha[6];
    ScePspUnion32 offset_u[3];
    ScePspUnion32 offset_v[3];
    Prop943Layers *layers;
};

static inline void copy_matrix(ScePspFMatrix4 *out, ScePspFMatrix4 *in) {
#ifdef __MWERKS__
    __asm__(
        "lv.q C000, 0x0(%1)" "lv.q C010, 0x10(%1)"
        "lv.q C020, 0x20(%1)" "lv.q C030, 0x30(%1)"
        "sv.q C000, 0x0(%0)" "sv.q C010, 0x10(%0)"
        "sv.q C020, 0x20(%0)" "sv.q C030, 0x30(%0)"
        : "=m"(*out) : "m"(*in)
    );
#else
    *out = *in;
#endif
}
extern "C" void func_eboot_08815274(Camera *, ScePspFMatrix4 *, float, float, float, float);

extern "C" void func_game_sub_09CBBBD8(Prop943Draw *self) {
    ScePspFMatrix4 matrix;
    ScePspFMatrix4 projection;
    ScePspFMatrix4 scale;
    ge::atest(255, 0, GE_OP_GREATER_THAN);
    ge::zwritedisable(true);
    ge::ztest(GE_OP_ALWAYS);
    for (int i = 0; i < self->layers->count; i++) {
        if (self->params->first_mesh + i >= self->model->header->mesh_count)
            break;
        u32 flags = self->layers->flags[i];
        if (flags & 1) {
            vmidt_q(&matrix);
        } else if (flags & 2) {
            Camera *camera = Camera::objectPtr;
            func_eboot_08815274(camera, &projection, 0.8726646900177002f,
                               camera->unknown_0x8, camera->near_z, camera->far_z);
            ge::projection(&projection);
            copy_matrix(&matrix, (ScePspFMatrix4 *)((u8 *)Camera::objectPtr + 0xC80));
            float z = self->position.z;
            float y = self->position.y;
            float x = self->position.x;
            matrix.w.x = x; matrix.w.y = y; matrix.w.z = z;
        } else if (flags & 4) {
            Camera *camera = Camera::objectPtr;
            func_eboot_08815274(camera, &projection, 0.8726646900177002f,
                               camera->unknown_0x8, camera->near_z, camera->far_z);
            ge::projection(&projection);
            float z = self->position.z;
            float y = self->position.y;
            float x = self->position.x;
            vmidt_q(&matrix);
            matrix.w.x = x; matrix.w.y = y; matrix.w.z = z;
        }
        if (self->params->mode == 9) {
            scaleMatrix(&scale, 1.0f, 1.5f, 1.0f);
            vmmulr_q(&matrix, &matrix, &scale);
        }
        emit_world_model(&matrix, &self->model->scale);
        self->model->set_mesh_color(self->params->first_mesh + i, 255, 255, 255);
        self->model->set_mesh_shadow_color(self->params->first_mesh + i, 255, 255, 255);
        self->model->set_mesh_alpha(self->params->first_mesh + i, (u8)(255.0f * self->alpha[i]));
        ge::texoffsetu(self->offset_u[i]);
        ge::texoffsetv(self->offset_v[i]);
        self->model->drawMesh(0, self->texture, self->params->first_mesh + i);
    }
    ge::projection(&Camera::objectPtr->projection);
    ge::texoffsetu();
    ge::texoffsetv();
    ge::zwritedisable(false);
    ge::ztest(GE_OP_AT_MOST);
    ge::atest(255, 128, GE_OP_AT_LEAST);
}
