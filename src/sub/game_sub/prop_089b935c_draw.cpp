#include "draw_manager.hpp"
#include "camera.hpp"
#include "immediate_ge.hpp"
using namespace immediate_ge;

struct Prop935Draw {
    u8 padding[0x2C];
    pmo *model;
    tmh *texture;
    u16 *params;
    u16 tick;
    u8 pad3a[6];
    ScePspFVector4 position;
    float alpha;
};

static inline void copy_matrix(ScePspFMatrix4 *out, ScePspFMatrix4 *in) {
#ifdef __MWERKS__
    __asm__(
        "lv.q C000, 0x0(%1)"
        "lv.q C010, 0x10(%1)"
        "lv.q C020, 0x20(%1)"
        "lv.q C030, 0x30(%1)"
        "sv.q C000, 0x0(%0)"
        "sv.q C010, 0x10(%0)"
        "sv.q C020, 0x20(%0)"
        "sv.q C030, 0x30(%0)"
        : "=m"(*out) : "m"(*in)
    );
#else
    *out = *in;
#endif
}
extern "C" void func_eboot_08815274(Camera *, ScePspFMatrix4 *, float, float, float, float);

extern "C" void func_game_sub_09CB81A0(Prop935Draw *self) {
    ScePspUnion32 offset;
    ScePspFMatrix4 matrix;
    ScePspFMatrix4 projection;
    ge::atest(255, 0, GE_OP_GREATER_THAN);
    ge::zwritedisable(true);
    ge::ztest(GE_OP_ALWAYS);
    DrawManager::objectPtr->dither_matrix(1);
    copy_matrix(&matrix, (ScePspFMatrix4 *)((u8 *)Camera::objectPtr + 0xC80));
    float z = self->position.z;
    float y = self->position.y;
    float x = self->position.x;
    matrix.w.x = x;
    matrix.w.y = y;
    matrix.w.z = z;
    emit_world_model(&matrix, &self->model->scale);
    offset.f = (self->tick & 0x3FF) * 0.0009765625f;
    ge::texoffsetu(offset);
    self->model->set_mesh_shadow_color(self->params[0], 255, 255, 255);
    self->model->set_mesh_alpha(self->params[0], (u8)(255.0f * self->alpha));
    Camera *camera = Camera::objectPtr;
    func_eboot_08815274(camera, &projection, 0.8726646900177002f, camera->unknown_0x8, camera->near_z, camera->far_z);
    ge::projection(&projection);
    self->model->drawMesh(0, self->texture, self->params[0]);
    ge::projection(&Camera::objectPtr->projection);
    ge::texoffsetu();
    ge::zwritedisable(false);
    ge::ztest(GE_OP_AT_MOST);
    ge::atest(255, 128, GE_OP_AT_LEAST);
    ge::ditherenable(false);
}
