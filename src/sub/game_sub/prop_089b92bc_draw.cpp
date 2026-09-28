

// copy 1 zero 0
#include "stage_manager.hpp"
#include "draw_manager.hpp"
#include "camera.hpp"
#include "immediate_ge.hpp"
#include "vfpu.h"

using namespace immediate_ge;

struct Prop92Params {
    u8 padding[0x14];
    u16 mesh;
    u16 period;
};
struct Prop92Draw {
    u8 padding[0x28];
    u32 mode;
    pmo *model;
    tmh *texture;
    Prop92Params *params;
    u16 tick;
    u16 pad3a;
    float alpha;
    ScePspFVector4 position;
};

extern "C" void func_game_sub_09CB5CC0(Prop92Draw *self) {
    ScePspUnion32 offset;
    ScePspFMatrix4 matrix;
    ge::zwritedisable(true);
    ge::atest(255, 0, GE_OP_GREATER_THAN);
    ge::ztest(GE_OP_ALWAYS);
    if ((self->mode & 2) == 0)
        DrawManager::objectPtr->dither_matrix(1);
    vmidt_q(&matrix);
    matrix.x.x = 1.25f;
    matrix.y.y = 1.25f;
    matrix.z.z = 1.25f;
    vmmul_t(&matrix, (ScePspFMatrix4 *)((u8 *)Camera::objectPtr + 0xC80), &matrix);
    float z = self->position.z;
    float y = self->position.y;
    float x = self->position.x;
    matrix.w.x = x;
    matrix.w.y = y;
    matrix.w.z = z;
    emit_world_model(&matrix, &self->model->scale);
    self->model->set_mesh_color(self->params->mesh, 255, 255, 255);
    self->model->set_mesh_shadow_color(self->params->mesh, 255, 255, 255);
    u8 alpha = (u8)(255.0f * self->alpha);
    if (self->mode & 1) {
        ge::blendfixeda(alpha, alpha, alpha);
        self->model->set_mesh_shadow_color(self->params->mesh, alpha, alpha, alpha);
    } else {
        self->model->set_mesh_alpha(self->params->mesh, alpha);
    }
    *(volatile u32 *)&offset = 0;
    if (self->params->period) {
        offset.f = (float)self->tick / (float)self->params->period;
        ge::texoffsetu(offset);
    }
    self->model->drawMesh(0, self->texture, self->params->mesh);
    ge::texoffsetu();
    ge::zwritedisable(false);
    ge::atest(255, 128, GE_OP_AT_LEAST);
    ge::ztest(GE_OP_AT_MOST);
    ge::ditherenable(false);
    ge::blendfixedb(255, 255, 255);
}
