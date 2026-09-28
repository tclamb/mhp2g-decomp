
#include "draw_manager.hpp"
#include "camera.hpp"
#include "immediate_ge.hpp"
using namespace immediate_ge;

struct Prop947Draw {
    u8 padding[0x2C];
    pmo *model;
    tmh *texture;
    u16 *params;
    u8 active;
    u8 pad39;
    u16 tick;
    u32 pad3c;
    float alpha;
    u8 pad44[12];
    ScePspFVector4 position;
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
extern "C" void func_game_sub_09CBD9E0(Prop947Draw *self) {
    volatile ScePspUnion32 offset;
    ScePspFMatrix4 matrix;
    ScePspFMatrix4 projection;
    ge::atest(255, 0, GE_OP_GREATER_THAN);
    ge::zwritedisable(true);
    ge::ztest(GE_OP_ALWAYS);
    copy_matrix(&matrix, (ScePspFMatrix4 *)((u8 *)Camera::objectPtr + 0xC80));
    float z = self->position.z;
    float y = self->position.y;
    float x = self->position.x;
    matrix.w.x = x;
    matrix.w.y = y;
    matrix.w.z = z;
    emit_world_model(&matrix, &self->model->scale);
    Camera *camera = Camera::objectPtr;
    func_eboot_08815274(camera, &projection, 0.8726646900177002f, camera->unknown_0x8, camera->near_z, camera->far_z);
    ge::projection(&projection);
    offset.f = 1.0f - (float)self->tick / 256.0f;
    ge::texoffsetv(const_cast<ScePspUnion32 &>(offset));
    self->model->set_mesh_alpha(self->params[0], (u8)(255.0f * self->alpha));
    self->model->set_mesh_color(self->params[0], 255, 255, 255);
    self->model->set_mesh_shadow_color(self->params[0], 255, 255, 255);
    self->model->drawMesh(0, self->texture, self->params[0]);
    if (self->tick <= 128) {
        offset.f = 1.0f - (float)self->tick / 128.0f;
        ge::texoffsetv(const_cast<ScePspUnion32 &>(offset));
        self->model->drawMesh(0, self->texture, self->params[0]);
    }
    ge::texoffsetv();
    ge::zwritedisable(false);
    ge::ztest(GE_OP_AT_MOST);
    ge::atest(255, 128, GE_OP_AT_LEAST);
    ge::projection(&Camera::objectPtr->projection);
}
