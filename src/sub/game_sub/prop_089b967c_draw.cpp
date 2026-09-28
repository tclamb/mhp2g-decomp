#include "draw_manager.hpp"
#include "camera.hpp"
#include "immediate_ge.hpp"
#include "vfpu.h"
using namespace immediate_ge;
struct Prop967Draw {
    u8 padding[0x2C];
    pmo *model;
    tmh *texture;
    u16 *params;
    u16 tick;
    u16 pad3a;
    float alpha;
    u8 pad40[16];
    ScePspFVector4 position;
};
static inline void copy_three(ScePspFVector4 *out, ScePspFVector4 *in) {
#ifdef __MWERKS__
    __asm__(
        "lv.s S000, 0x0(%1)" "lv.s S001, 0x4(%1)" "lv.s S002, 0x8(%1)"
        "sv.s S000, 0x0(%0)" "sv.s S001, 0x4(%0)" "sv.s S002, 0x8(%0)"
        : "=m"(*out) : "m"(*in)
    );
#else
    out->x = in->x; out->y = in->y; out->z = in->z;
#endif
}
extern "C" void func_eboot_08815274(Camera *, ScePspFMatrix4 *, float, float, float, float);
extern "C" void func_game_sub_09CC5DA0(Prop967Draw *self) {
    ScePspUnion32 offset;
    ScePspFMatrix4 matrix;
    ScePspFMatrix4 projection;
    ge::atest(255, 0, GE_OP_GREATER_THAN);
    DrawManager::objectPtr->dither_matrix(1);
    float z = self->position.z;
    float y = self->position.y;
    float x = self->position.x;
    vmidt_q(&matrix);
    matrix.w.x = x; matrix.w.y = y; matrix.w.z = z;
    ScePspFMatrix4 *camera_basis = (ScePspFMatrix4 *)((u8 *)Camera::objectPtr + 0xC80);
    copy_three(&matrix.x, &camera_basis->x);
    copy_three(&matrix.y, &camera_basis->y);
    copy_three(&matrix.z, &camera_basis->z);
    emit_world_model(&matrix, &self->model->scale);
    Camera *camera = Camera::objectPtr;
    func_eboot_08815274(camera, &projection, 0.8726646900177002f, camera->unknown_0x8, camera->near_z, camera->far_z);
    ge::projection(&projection);
    float period = (float)self->params[1];
    offset.f = (float)self->tick / period;
    ge::texoffsetu(offset);
    self->model->set_mesh_color(self->params[0], 255, 255, 255);
    self->model->set_mesh_shadow_color(self->params[0], 255, 255, 255);
    self->model->set_mesh_alpha(self->params[0], (u8)(255.0f * self->alpha));
    self->model->drawMesh(0, self->texture, self->params[0]);
    ge::projection(&Camera::objectPtr->projection);
    ge::texoffsetu();
    ge::atest(255, 128, GE_OP_AT_LEAST);
    ge::ditherenable(false);
}
