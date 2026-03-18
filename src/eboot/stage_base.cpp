#include "common.h"

#include "stage_base.hpp"
#include "vfpu.h"
#include "draw_manager.hpp"
#include "stage_manager.hpp"
#include "immediate_ge.hpp"
#include "light_manager.hpp"
#include "player.hpp"
#include "system.hpp"

using namespace immediate_ge;

#pragma opt_unroll_loops on

StageBase::StageBase() {

}

StageBase::~StageBase() {
    // empty
}

extern struct global_089C7508 {
    u8 padding_0x0[0x1C];
    u16 short_0x1C;
    u8 padding_0x1E[0x28 - 0x1E];
    u8 player_id;
    u8 padding_0x29[0x2E - 0x29];
    u8 byte_0x2E;
    u8 padding_0x2F[0x422-0x2F];
    bool allow_hidden_flag;
    u8 padding_0x423[0x6AF0E - 0x423];
    u16 stage_id;
    u8 padding_0x6AF10[0x6AF14 - 0x6AF10];
    u32 flags_0x6AF14;
} *D_eboot_089C7508;

void StageBase::drawStg() {
    pmo *pmo = &model_pmo;
    stage_draw_command *command = vtable_0x48()->model_commands;
    emit_world_model(&transform, &pmo->scale);
    for (int i = 0; i < vtable_0x48()->model_commands_length; ++i, ++command) {
        ge::atest(0xFF, command->alpha_threshold, GE_OP_AT_LEAST);
        if ((command->flags & 1) == 0 || !D_eboot_089C7508->allow_hidden_flag) {
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_ALWAYS);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(true);
            }
            switch (command->opcode) {
            case 0:
                break;
            case 1:
                pmo->drawMesh(0, &model_tmh, command->mesh_index);
                break;
            case 2:
                vtable_0x54(pmo, command->data, command->mesh_index);
                break;
            case 3:
                vtable_0x58(pmo, command->data, command->mesh_index);
                break;
            case 4:
                vtable_0x5C(pmo, command->data, command->mesh_index);
                break;
            case 6:
                vtable_0x64(pmo, command->data, command->mesh_index);
                break;
            case 7:
                vtable_0x68(pmo, command->data, command->mesh_index);
                break;
            case 8:
                vtable_0x6C(pmo, command->data, command->mesh_index);
                break;
            case 9:
                vtable_0x70(pmo, command->data, command->mesh_index);
                break;
            case 11:
                vtable_0x78(pmo, command->data, command->mesh_index);
                break;
            case 15:
                vtable_0x88(pmo, command->data);
                break;
            case 18:
                vtable_0x94(pmo, command->data, command->mesh_index);
                // fallthrough
            case 19:
                // stage-defined opcode (see stages 13, 17, 39, 46, 66, 80, 99, 124, 192)
                vtable_0x98(pmo, command->data);
            default:
                break;
            }
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_AT_MOST);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(false);
            }
        }
    }
}

void StageBase::drawSet() {
    pmo *pmo = &prop_pmo;
    stage_draw_command *command = vtable_0x48()->prop_commands;
    emit_world_model(&transform, &pmo->scale);
    for (int i = 0; i < vtable_0x48()->prop_commands_length; ++i, ++command) {
        ge::atest(0xFF, command->alpha_threshold, GE_OP_AT_LEAST);
        if ((command->flags & 1) == 0 || !D_eboot_089C7508->allow_hidden_flag) {
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_ALWAYS);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(true);
            }
            switch (command->opcode) {
            case 1:
                pmo->drawMesh(0, &model_tmh, command->mesh_index);
                break;
            case 2:
                vtable_0x54(pmo, command->data, command->mesh_index);
                break;
            case 3:
                vtable_0x58(pmo, command->data, command->mesh_index);
                break;
            case 4:
                vtable_0x5C(pmo, command->data, command->mesh_index);
                break;
            case 5:
                vtable_0x60(pmo, command->data, command->mesh_index);
                break;
            case 6:
                vtable_0x64(pmo, command->data, command->mesh_index);
                break;
            case 7:
                vtable_0x68(pmo, command->data, command->mesh_index);
                break;
            case 8:
                vtable_0x6C(pmo, command->data, command->mesh_index);
                break;
            case 9:
                vtable_0x70(pmo, command->data, command->mesh_index);
                break;
            case 10:
                vtable_0x74(pmo, command->data, command->mesh_index);
                break;
            case 12:
                vtable_0x7C(pmo, command->data, command->mesh_index);
                break;
            case 13:
                vtable_0x80(pmo, command->data, command->mesh_index);
                break;
            case 14:
                vtable_0x84(pmo, command->data, command->mesh_index);
                break;
            case 15:
                vtable_0x88(pmo, command->data);
                break;
            case 16:
                vtable_0x8C(pmo, command->data, command->mesh_index);
                break;
            case 17:
                vtable_0x90(pmo, command->data, command->mesh_index);
                break;
            case 19:
                vtable_0x98(pmo, command->data);
                break;
            default:
                break;
            }
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_AT_MOST);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(false);
            }
        }
    }
}

void StageBase::vtable_0x54(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x54_params *args = (vtable_0x54_params *)data;

    u32 t;
    if ((args->flags & 8) != 0) {
        t = ~unknown_0x1C0 & 0xFFFF;
    } else {
        t = unknown_0x1C0;
    }
    float angle = ((((float)((u16)t % args->period) * 360.0f) / args->period) / 360.0f) * 6.2831855f;

    ScePspFMatrix4 local_transform;
    ScePspFVector4 *pos = &args->position;
    positionMatrix(&local_transform, pos->x, pos->y, pos->z);

    if ((args->flags & 1) != 0) {
        rotateX(&local_transform, angle);
    } else if ((args->flags & 2) != 0) {
        rotateY(&local_transform, angle);
    } else if ((args->flags & 4) != 0) {
        rotateZ(&local_transform, angle);
    }

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
}

void StageBase::vtable_0x58(pmo *pmo, void *data, u8 mesh_index) {
    ScePspFMatrix4 local_transform;
    ScePspFVector4 *pos = (ScePspFVector4 *)data;
    positionMatrix(&local_transform, pos->x, pos->y, pos->z);

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
}

void StageBase::vtable_0x5C(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x5C_params *args = (vtable_0x5C_params *)data;
    ScePspFMatrix4 local_transform;

    ScePspFVector4 *pos = &args->position;
    positionMatrix(&local_transform, pos->x, pos->y, pos->z);

    if ((args->flags & 1) != 0) {
        rotateX(&local_transform, args->angle);
    } else if ((args->flags & 2) != 0) {
        rotateY(&local_transform, args->angle);
    } else if ((args->flags & 4) != 0) {
        rotateZ(&local_transform, args->angle);
    }

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
}

void StageBase::vtable_0x60(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x60_params *args = (vtable_0x60_params *)data;
    ScePspFMatrix4 local_transform;

    ScePspFVector3 *sca = &args->scale;
    scaleMatrix(&local_transform, sca->x, sca->y, sca->z);
    ScePspFVector4 *pos = &args->position;
    setPosition(&local_transform, pos->x, pos->y, pos->z);

    if ((args->flags & 1) != 0) {
        rotateX(&local_transform, args->angle);
    } else if ((args->flags & 2) != 0) {
        rotateY(&local_transform, args->angle);
    } else if ((args->flags & 4) != 0) {
        rotateZ(&local_transform, args->angle);
    }

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
}

void StageBase::vtable_0x64(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x64_params *args = (vtable_0x64_params *)data;
    u16 u_period = args->u_period;
    if (u_period != 0) {
        ScePspUnion32 u_offset;
        u_offset.f = (unknown_0x1C0 % u_period) / (float)u_period;
        if ((args->flags & 8) != 0) {
            u_offset.f = 1.0f - u_offset.f;
        }
        ge::texoffsetu(u_offset);
    }
    u16 v_period = args->v_period;
    if (v_period != 0) {
        ScePspUnion32 v_offset;
        v_offset.f = (unknown_0x1C0 % v_period) / (float)v_period;
        if ((args->flags & 8) != 0) {
            v_offset.f = 1.0f - v_offset.f;
        }
        ge::texoffsetv(v_offset);
    }
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    ge::texoffset();
}

void StageBase::vtable_0x68(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x68_params *args = (vtable_0x68_params *)data;
    u16 u_period = args->u_period;
    if (u_period != 0) {
        ScePspUnion32 u_offset;
        float t = vsin_s(6.2831855f * (((360.0f * (args->u_frequency * (unknown_0x1C0 % u_period))) / 65536.0f) / 360.0f));
        u_offset.f = args->u_phase + args->u_amplitude * t;
        ge::texoffsetu(u_offset);
    }
    u16 v_period = args->v_period;
    if (v_period != 0) {
        ScePspUnion32 v_offset;
        float t = vsin_s(6.2831855f * (((360.0f * (args->v_frequency * (unknown_0x1C0 % v_period))) / 65536.0f) / 360.0f));
        v_offset.f = args->v_phase + args->v_amplitude * t;
        ge::texoffsetv(v_offset);
    }
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    ge::texoffset();
}

void StageBase::vtable_0x6C(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x6C_params *args = (vtable_0x6C_params *)data;
    ScePspFMatrix4 local_transform;

    u16 u_period = args->u_period;
    if (u_period != 0) {
        ScePspUnion32 u_offset;
        u_offset.f = (unknown_0x1C0 % u_period) / (float)u_period;
        if ((args->flags & 8) != 0) {
            u_offset.f = 1.0f - u_offset.f;
        }
        ge::texoffsetu(u_offset);
    }
    u16 v_period = args->v_period;
    if (v_period != 0) {
        ScePspUnion32 v_offset;
        v_offset.f = (unknown_0x1C0 % v_period) / (float)v_period;
        if ((args->flags & 8) != 0) {
            v_offset.f = 1.0f - v_offset.f;
        }
        ge::texoffsetv(v_offset);
    }

    u16 t;
    if ((args->flags & 8) != 0) {
        t = ~unknown_0x1C0 & 0xFFFF;
    } else {
        t = unknown_0x1C0;
    }
    s16 shift = args->coarseness;
    float angle = (((((t & (0xFFFF >> shift)) << shift) * 360.0f) / 65536.0f) / 360.0f) * 6.2831855f;

    ScePspFVector4 *pos = &args->position;
    positionMatrix(&local_transform, pos->x, pos->y, pos->z);

    if ((args->flags & 1) != 0) {
        rotateX(&local_transform, angle);
    } else if ((args->flags & 2) != 0) {
        rotateY(&local_transform, angle);
    } else if ((args->flags & 4) != 0) {
        rotateZ(&local_transform, angle);
    }

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    ge::texoffset();
    emit_world_model(&this->transform, &pmo->scale);
}

void StageBase::vtable_0x70(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x70_params *args = (vtable_0x70_params *)data;
    ScePspFMatrix4 local_transform;

    u16 u_period = args->u_period;
    if (u_period != 0) {
        ScePspUnion32 u_offset;
        u_offset.f = (unknown_0x1C0 % u_period) / (float)u_period;
        if ((args->flags & 8) != 0) {
            u_offset.f = 1.0f - u_offset.f;
        }
        ge::texoffsetu(u_offset);
    }
    u16 v_period = args->v_period;
    if (v_period != 0) {
        ScePspUnion32 v_offset;
        v_offset.f = (unknown_0x1C0 % v_period) / (float)v_period;
        if ((args->flags & 8) != 0) {
            v_offset.f = 1.0f - v_offset.f;
        }
        ge::texoffsetv(v_offset);
    }

    ScePspFVector4 *pos = &args->position;
    positionMatrix(&local_transform, pos->x, pos->y, pos->z);

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
    ge::texoffset();
}

void StageBase::vtable_0x74(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x74_params *args = (vtable_0x74_params *)data;
    ScePspFMatrix4 local_transform;

    u16 u_period = args->u_period;
    if (u_period != 0) {
        ScePspUnion32 u_offset;
        u_offset.f = (unknown_0x1C0 % u_period) / (float)u_period;
        if ((args->flags & 8) != 0) {
            u_offset.f = 1.0f - u_offset.f;
        }
        ge::texoffsetu(u_offset);
    }
    u16 v_period = args->v_period;
    if (v_period != 0) {
        ScePspUnion32 v_offset;
        v_offset.f = (unknown_0x1C0 % v_period) / (float)v_period;
        if ((args->flags & 8) != 0) {
            v_offset.f = 1.0f - v_offset.f;
        }
        ge::texoffsetv(v_offset);
    }

    ScePspFVector4 *pos = &args->position;
    positionMatrix(&local_transform, pos->x, pos->y, pos->z);

    if ((args->flags & 1) != 0) {
        rotateX(&local_transform, args->angle);
    } else if ((args->flags & 2) != 0) {
        rotateY(&local_transform, args->angle);
    } else if ((args->flags & 4) != 0) {
        rotateZ(&local_transform, args->angle);
    }

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
    ge::texoffset();
}

void StageBase::vtable_0x78(pmo *pmo, void *data, u8 mesh_index) {
    u16 flags = *(u16 *)data;
    if ((flags & 0x100) != 0) {
        ScePspUnion32 u_offset; u_offset.f = ext(unknown_0x1C0, 0, 2) * 0.25f;
        ScePspUnion32 v_offset; v_offset.f = ext(unknown_0x1C0, 2, 2) * 0.25f;
        ge::texoffset(u_offset, v_offset);
    } else if ((flags & 0x200) != 0) {
        ScePspUnion32 u_offset; u_offset.f = ext(unknown_0x1C0, 0, 3) * 0.125f;
        ScePspUnion32 v_offset; v_offset.f = ext(unknown_0x1C0, 3, 2) * 0.125f;
        ge::texoffset(u_offset, v_offset);
    } else if ((flags & 0x400) != 0) {
        ScePspUnion32 u_offset; u_offset.f = ext(unknown_0x1C0, 0, 3) * 0.125f;
        ScePspUnion32 v_offset; v_offset.f = ext(unknown_0x1C0, 3, 3) * 0.125f;
        ge::texoffset(u_offset, v_offset);
    }

    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    ge::texoffset();
}

extern "C" {
    extern void *D_eboot_09A4AE04;
    player *func_eboot_088DF804(void *, int);
}

void StageBase::vtable_0x7C(pmo *pmo, void *data, u8 mesh_index) {
    u8 player_id = D_eboot_089C7508->player_id;
    player *p = func_eboot_088DF804(D_eboot_09A4AE04, player_id);
    vtable_0x7C_params *args = (vtable_0x7C_params *)data;
    if (p != 0) {
        if (p->position.x < args->bbox_min.x ||
            args->bbox_max.x < p->position.x ||
            p->position.y < args->bbox_min.y ||
            args->bbox_max.y < p->position.y ||
            p->position.z < args->bbox_min.z ||
            args->bbox_max.z < p->position.z) {
                return;
        }
    }

    ScePspFMatrix4 local_transform;
    ScePspFVector4 *pos = &args->position;
    positionMatrix(&local_transform, pos->x, pos->y, pos->z);

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
}

void StageBase::vtable_0x80(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x80_params *args = (vtable_0x80_params *)data;

    float t = 6.2830896f * ((unknown_0x1C0 % args->period) / (float)args->period);
    float f = vsin_s(t);
    ScePspFVector4 position;
    vscl_t(&position, &args->amplitude, f);
    vadd_t(&position, &position, &args->phase);

    ScePspFMatrix4 local_transform;
    positionMatrix(&local_transform, position.x, position.y, position.z);

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
}

void StageBase::vtable_0x84(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x84_params *args = (vtable_0x84_params *)data;

    float t = 6.2830896f * ((unknown_0x1C0 % args->period) / (float)args->period);
    float f = vsin_s(t);
    ScePspFVector4 scale;
    vscl_t(&scale, &args->amplitude, f);
    vadd_t(&scale, &scale, &args->phase);

    ScePspFMatrix4 local_transform;
    ScePspFVector4 *pos = &args->position;
    scaleMatrix(&local_transform, scale.x, scale.y, scale.z);
    setPosition(&local_transform, pos->x, pos->y, pos->z);

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
}

extern "C" {
    int func_eboot_08816E20(global_089C6CB4 *, ScePspFVector4 *, float);
    void func_eboot_08814E84(global_089C6CB4 *, ScePspFVector4 *);
}

void StageBase::vtable_0x88(pmo *pmo, void *data) {
    vtable_0x88_params *args = (vtable_0x88_params *)data;
    ScePspFVector4 *position = &args->positions[0];

    ScePspFMatrix4 local_transform;
    float one = 1.0f;
    scaleMatrix(&local_transform, one, one, one);

    for (int i = 0; i < args->count; ++i, ++position) {
        if ((u8)func_eboot_08816E20(D_eboot_089C6CB4, position, args->cutoff) == true) {
            setPosition(&local_transform, position->x, position->y, position->z);
            emit_world_model(&local_transform, &pmo->scale);

            ScePspFVector4 camera_position;
            func_eboot_08814E84(D_eboot_089C6CB4, &camera_position);

            float d2 = distanceSquared(position, &camera_position);
            if (d2 <= 2250000.0f + args->cutoff * args->cutoff) {
                pmo->drawMesh(NULL, &model_tmh, args->near_mesh_index);
            } else {
                pmo->drawMesh(NULL, &model_tmh, args->far_mesh_index);
            }
        }
    }
    emit_world_model(&this->transform, &pmo->scale);
}

extern "C" {
    // vfpu trunc?? effectively: return (float)(u32)x;
    float func_eboot_08899DF8(float x);
}

void StageBase::vtable_0x8C(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x8C_params *args = (vtable_0x8C_params *)data;

    ScePspUnion32 u_offset;
    u_offset.f = 0.033333335f * ((360.0f * unknown_0x1C2) / 65536.0f);
    u_offset.f -= func_eboot_08899DF8(u_offset.f);
    ge::texoffsetu(u_offset);

    u16 v_period = args->v_period;
    if (v_period != 0) {
        ScePspUnion32 v_offset;
        v_offset.f = (unknown_0x1C0 % v_period) / (float)v_period;
        ge::texoffsetv(v_offset);
    }

    ScePspFMatrix4 local_transform;
    positionMatrix(&local_transform, args->position.x, args->position.y, args->position.z);
    rotateY(&local_transform, 6.2831855f * (((360.0f * unknown_0x1C2) / 65536.0f) / 360.0f));

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    ge::texoffset();
    emit_world_model(&this->transform, &pmo->scale);
}

inline float sinu16(int x) {
    return vsin_s(6.2831855f * (((360.0f * x) / 65536.0f) / 360.0f));
}

inline void handle_rotate(ScePspFMatrix4 *transform, u16 flags, float angle) {
    if ((flags & 1) != 0) {
        rotateX(transform, angle);
    } else if ((flags & 2) != 0) {
        rotateY(transform, angle);
    } else if ((flags & 4) != 0) {
        rotateZ(transform, angle);
    }
}

inline void rotate_y(ScePspFMatrix4 *transform, float angle) {
    rotateY(transform, angle);
}

void StageBase::vtable_0x90(pmo *pmo, void *data, u8 mesh_index) {
    ScePspFMatrix4 local_transform;
    vtable_0x90_params *args = (vtable_0x90_params *)data;

    u16 t = unknown_0x1C0 + args->phase;
    u16 a = t & 0xFFF;
    float c = sinu16((u16)(a * sinu16(a << 2)) << 10);
    float e = 0.5f * args->amplitude;
    float f = e * c;
    float g = e * sinu16((u16)a << 10);
    float angle = f + g;

    ScePspFVector4 *pos = &args->position;
    positionMatrix(&local_transform, pos->x, pos->y, pos->z);

    handle_rotate(&local_transform, args->flags, angle);

    rotate_y(&local_transform, args->angle);

    emit_world_model(&local_transform, &pmo->scale);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
    emit_world_model(&this->transform, &pmo->scale);
}

extern "C" {
    int func_eboot_088566DC(global_089C7508 *);
}

void StageBase::vtable_0x94(pmo *pmo, void *data, u8 mesh_index) {
    vtable_0x94_params *state = (vtable_0x94_params *)data;

    if ((u8)func_eboot_088566DC(D_eboot_089C7508) == 0) {
        switch (state->c) {
        case 0:
            if (--state->e < 0) {
                state->e = (System::get()->next_index(1) % 60) + 30;
                ++state->c;
            }
            break;
        case 1:
            state->d = state->a + (System::get()->next_index(1) % (state->b - state->a));
            if (--state->e < 0) {
                ++state->c;
            }
            break;
        case 2:
            if (state->d > 0xFF - 4) {
                state->e = (System::get()->next_index(1) % 180) + 180;
                state->d = 0xFF;
                state->c = 0;
            } else {
                state->d += 4;
            }
            break;
        }
    }

    pmo->set_mesh_blend_mode(mesh_index, GE_BLENDMODE_MUL_AND_SUBTRACT_REVERSE);
    pmo->set_mesh_color(mesh_index, 0xFF, 0xFF, 0xFF);
    pmo->set_mesh_shadow_color(mesh_index, 0xFF, 0xFF, 0xFF);
    pmo->set_mesh_alpha(mesh_index, state->d);
    pmo->drawMesh(NULL, &model_tmh, mesh_index);
}

void StageBase::vtable_0x98(pmo *, void *) {
    // empty
}

void StageBase::clear() {
    memset(&prop_pmo, 0, sizeof(prop_pmo));
    memset(&prop_skeleton, 0, sizeof(prop_skeleton));
    reset_transform();
    unknown_0x330 = 0; // environment_params->type
    flag_0x3D4 = false;
    flags &= ~drawable::VISIBLE;
    unknown_0x1C0 = 0; // animation timer?
    unknown_0x1C2 = 0;
    method_088CDCAC();
    set_ptmf_0x3D8(&StageBase::vtable_0x4C);
}

extern "C" {
    extern void *D_eboot_08A5DE5C;
    int func_eboot_08885198(void *, u32, u8, u32, u8);
    void func_eboot_08883858(void *, u32, u32, u32, u32, u32, ScePspFVector4*, u32, u32, u32, bool);
    int func_eboot_088852C8(void *, u32, u32, u32);
    void func_eboot_0888444C(void *, u32, u32, u32, u32, ScePspFVector4 *, u32, u32, u32);

    bool func_eboot_0884F9A0(void *, int);

    extern void *D_eboot_09A4ADAC;
    bool func_eboot_088D0824(void *, u32, u8);
}

void StageBase::vtable_0x1C() {
    int i;
    stage_sound *sound = vtable_0xB0();
    for (i = 0; i < vtable_0xAC(); ++i, ++sound) {
        if ((bool)(D_eboot_089C7508->flags_0x6AF14 & 1) == false && func_eboot_0884F9A0(D_eboot_089C7508, 0) == true) {
            func_eboot_08885198(D_eboot_08A5DE5C, sound->unknown_0x4, 0xC0, i + 1, 0);
        } else {
            if (func_eboot_088D0824(D_eboot_09A4ADAC, sound->unknown_0x8, 0) != false) {
                continue;
            }
            if (sound->unknown_0x0 == 0) {
                StageManager::get()->register_sound(sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC);
            } else {
                func_eboot_08883858(D_eboot_08A5DE5C, sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC, 0, 0, false);
            }
        }
    }
}

int StageBase::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *StageBase::vtable_0xB0() {
    return definitions()->sounds;
}

void StageBase::vtable_0x20() {
    if ((bool)(D_eboot_089C7508->flags_0x6AF14 & 1) != true) {
        int i;
        u16 *shorts = StageManager::get()->stage->definitions()->unknown_0x10;
        for (i = 0; i < (int)StageManager::get()->stage->definitions()->unknown_0x3E; ++i, shorts += 12) {
            int global_sound = 0;
            if (shorts[1] == 0x17 && shorts[11] == 0) {
                ScePspFVector4 *position = vtable_0x44();
                if (position != NULL) {
                    if ((0 < (D_eboot_089C7508->flags_0x6AF14 & 1)) == 0) {
                        u8 byte_0x2E = D_eboot_089C7508->byte_0x2E;
                        switch (byte_0x2E) {
                        default:
                            break;
                        case 6:
                        case 5:
                        case 7:
                        case 8:
                            if (func_eboot_088852C8(D_eboot_08A5DE5C, 6, 0xC0, 0) != false) {
                                func_eboot_08885198(D_eboot_08A5DE5C, 6, 0xC0, 0, 0);
                            }
                            global_sound = 1;
                            break;
                        }
                    }
                    if (global_sound == 0) {
                        StageManager::get()->register_sound(6, 0x11, 0, 0xC0, 0, position, 9);
                    }
                }
                break;
            }
        }
    }
    stage_sound *sound = vtable_0xB0();
    for (int i = 0; i < vtable_0xAC(); ++i, ++sound) {
        if ((0 < (D_eboot_089C7508->flags_0x6AF14 & 1)) == 0 &&
            func_eboot_0884F9A0(D_eboot_089C7508, 0) == 1) {
            if (func_eboot_088852C8(D_eboot_08A5DE5C, sound->unknown_0x4, 0xC0, i + 1) != 0) {
                func_eboot_08885198(D_eboot_08A5DE5C, sound->unknown_0x4, 0xC0, i + 1, 0);
            }
        } else if (func_eboot_088D0824(D_eboot_09A4ADAC, sound->unknown_0x8, 1) == 0) {
            if (sound->unknown_0x0 == 0) {
                StageManager::get()->register_sound(sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC);
            } else {
                if (D_eboot_089C7508->short_0x1C % sound->unknown_0x0 == 0) {
                    func_eboot_08883858(D_eboot_08A5DE5C, sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC, 0, 0, false);
                }
            }
            if (sound->unknown_0x0 != 0 && sound->unknown_0x8 != 0x50) {
                func_eboot_0888444C(D_eboot_08A5DE5C, sound->unknown_0x4, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC, 0, 0);
            }
        }
    }
}

void StageBase::call_ptmf_0x3D8() {
    if (ptmf_0x3D8 != 0) {
        (this->*ptmf_0x3D8)();
    }
}

void StageBase::destroy() {
    func_eboot_08885198(D_eboot_08A5DE5C, 6, 0xC0, 0, 0);
    int i;
    stage_sound *sound = vtable_0xB0();
    for (i = 0; i < vtable_0xAC(); ++i, ++sound) {
        func_eboot_08885198(D_eboot_08A5DE5C, sound->unknown_0x4, 0xC0, i + 1, 0);
    }
}

void StageBase::draw() {
    if (D_eboot_089C7508->stage_id != 0) {
        if (vtable_0x48() != 0) {
            vmidt_q(&transform);
            if (vtable_0x48()->model_commands != 0) {
                drawStg();
            }
            if (vtable_0x48()->prop_commands != 0) {
                drawSet();
            }
        }

        ge::texoffsetu();
        ge::texoffsetv();
        ge::atest(0xFF, 0, GE_OP_GREATER_THAN);
        LightManager::get()->method_08860C4C();
        LightManager::get()->method_0886117C(1);
    }
}

stage_definitions *StageBase::definitions() {
    return 0;
}

stage_exit *StageBase::exits(u32 ignored_map_id) {
    stage_definitions *d = definitions();
    if (d != 0) {
        return d->exits;
    }
    return 0;
}

s8 StageBase::exit_count(u32 ignored_map_id) {
    stage_definitions *d = definitions();
    u8 count;
    if (d != 0) {
        count = d->exit_count;
    } else {
        count = 0;
    }
    return count;
}

#ifdef BUILD_NONMATCHING
// score 45: 9 regswaps on the args passed to ge::fog; when is ft0 used??
void StageBase::emit_fog() {
    stage_fog *fog = &this->fog;
    if (DrawManager::get()->start_fragment(render_group::RESET)) {
        float begin = fog->begin;
        float end = fog->end;
        float norm = Ge::get()->norm;
        begin *= norm;
        end *= norm;
        ge::fog(fog->color, begin, end);
        DrawManager::get()->end_fragment();
    }
}
#else
INCLUDE_ASM("asm/eboot/nonmatchings/stage_base", emit_fog__9StageBaseFv);
#endif

void StageBase::vtable_0x24() {
    method_088CEA2C();
}

void StageBase::method_088CD61C() {
    if (flag_0x3D4 != 0) {
        ge::ditherenable(true);
        DrawManager::get()->dither_matrix(0);

        ge::alphatestenable(false);
        ge::lightingenable(false);

        ge::alphablendenable(true);
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);

        ge::ztestenable(false);
        ge::ztest(GE_OP_NEVER);

        ge::texturemapenable(false);

        ge::shademode(GE_SHADE_GOURAUD);

        draw_sky_gradient();
        draw_flash();

        ge::ztest(GE_OP_AT_MOST);
        ge::ztestenable(true);

        ge::alphatestenable(true);

        ge::ditherenable(false);

        flag_0x3D4 = false;
    }
}

void StageBase::compile_environment_params(stage_environment_params *environment) {
    unknown_0x330 = environment->type;

    float *in;
    switch (unknown_0x330) {
    case 1:
        in = (float *)environment->data;
        in = compile_fog_params(in);
        compile_lights(in);
        break;
    case 2:
        in = (float *)environment->data;
        in = compile_fog_params(in);
        in = compile_lights(in);
        compile_sky_gradient(in);
        break;
    default:
        unknown_0x330 = 0;
        break;
    }
}

void StageBase::operator delete(void *) {
    // empty
}

void StageBase::vtable_0x4C() {
    sky_gradient_vdata[2].x = 0;
    sky_gradient_vdata[0].x = 0;
    sky_gradient_vdata[3].x = 480;
    sky_gradient_vdata[1].x = 480;
    sky_gradient_vdata[3].z = 0;
    sky_gradient_vdata[1].z = 0;
    sky_gradient_vdata[2].z = 0;
    sky_gradient_vdata[0].z = 0;
    flags |= drawable::VISIBLE;
    vtable_0x1C();
    method_088CDCAC();
    set_ptmf_0x3D8(&StageBase::vtable_0x50);
}

extern "C" {
    struct resource_type {
        enum {
            PICKAXE = 3,
            BUG_NET = 4,
        };
    private: resource_type();
    };

    struct resource_node {
        ScePspFVector3 position;
        float unknown_0xC; // interaction distance?
        u16 unknown_0x10; // loot table?
        u16 remaining;
        u16 type;
        u16 unknown_0x16; // high tier rolls?
    };

    extern void *D_eboot_08A5DD4C;
    resource_node *func_eboot_08869568(void *, int);

    struct bug_flag {
        enum {
            SCALE_WITH_ALPHA = 1,
            PERSISTENT = 2,
            RANDOM_SPAWN_POSITION = 4,
        };
    private: bug_flag();
    };
}


void StageBase::vtable_0x50() {
    if (flash_state != 0) {
        if (flash_state == 1) {
            if (--flash_frames <= 0) {
                flash_state = 2;
                flash_frames = 2;
            }
        } else if (flash_state == 2) {
            if (--flash_frames <= 0) {
                flash_state = 3;
                flash_frames = 0x5A;
            }
        } else {
            if (--flash_frames <= 0) {
                flash_state = 0;
                flash_frames = 0;
            }
        }
    }
    resource_node *node = NULL;
    if ((bool)(D_eboot_089C7508->flags_0x6AF14 & 1) != true) {
        node = func_eboot_08869568(D_eboot_08A5DD4C, D_eboot_089C7508->stage_id);
    }
    if (node) {
        while (node->position.x != -1.0f) {
            if (node->remaining > 0 && node->type == resource_type::BUG_NET) {
                stage_definitions *assets = StageManager::get()->stage->definitions();
                if (assets->bug_mesh_index != -1 && (unknown_0x1C0 & 0x7F) == 0) {
                    ScePspFVector4 spawn_center, spawn_box;
                    sv_q(&spawn_center, node->position.x, node->position.y + 65.0f, node->position.z, 0.0f);
                    sv_q(&spawn_box, 64.0f, 64.0f, 64.0f, 0.0f);
                    switch (D_eboot_089C7508->stage_id) {
                        case stages::TOWER_3:
                        case stages::GREAT_FOREST_N_2: {
                            stage_definitions *assets = StageManager::get()->stage->definitions();
                            u32 flags = bug_flag::RANDOM_SPAWN_POSITION;
                            StageManager::get()->push_prop_089B969C(flags, assets->bug_mesh_index, &spawn_center, &spawn_box);
                            break;
                        }
                        default: {
                            stage_definitions *assets = StageManager::get()->stage->definitions();
                            u32 flags = bug_flag::RANDOM_SPAWN_POSITION | bug_flag::SCALE_WITH_ALPHA;
                            StageManager::get()->push_prop_089B969C(flags, assets->bug_mesh_index, &spawn_center, &spawn_box);
                            break;
                        }
                    }
                }
            }
            ++node;
        }
    }
    method_088CDCAC();
    vtable_0x20();
    ++unknown_0x1C0;
    unknown_0x1C2 += 0x2E;
}

extern "C" {
    extern void *D_game_task_09C0D430;
    u8 func_game_task_09AAEE58(void *, u8, bool, bool);
}


bool StageBase::method_088CDC74() {
    return func_game_task_09AAEE58(D_game_task_09C0D430, 54, false, false) == true;
}

extern "C" {
    bool func_eboot_0886A304(void *, bool);
    int func_eboot_0886A354(void *, bool);
}

void StageBase::method_088CDCAC() {
    if (vtable_0xA4() == false) {
        unknown_0x444 = 0;
        return;
    }

    if (func_eboot_0886A304(D_eboot_08A5DD4C, true)) {
        return;
    }

    if (func_game_task_09AAEE58(D_game_task_09C0D430, 54, true, true) == true) {
        unknown_0x444 = 2;
        return;
    }

    if (func_eboot_0886A354(D_eboot_08A5DD4C, true) - func_eboot_0886A354(D_eboot_08A5DD4C, false) < 900) {
        unknown_0x444 = 0;
        return;
    }

    if ((func_eboot_0886A354(D_eboot_08A5DD4C, false) / 7200 & 1) != 0) {
        unknown_0x444 = 0;
        return;
    }

    unknown_0x444 = 1;
}

bool StageBase::vtable_0xA4() {
    return false;
}

inline u16 atan2s16(float y, float x) {
    return (int)((65536.0f * atan2f_s(y, x)) / 6.2831855f + 0.5f);
}

void StageBase::draw_sky_gradient() {
    u16 camera_angle = atan2s16(-D_eboot_089C6CB4->position.x, -D_eboot_089C6CB4->position.z);
    u16 sun_angle = atan2s16(sky_gradient_origin.x, sky_gradient_origin.z);
    u16 dtheta = sun_angle - camera_angle;
    u16 top = sky_gradient_top;
    sky_gradient_vdata[1].y = top;
    sky_gradient_vdata[0].y = top;
    float height = sky_gradient_height / 448.0f;
    u16 bottom = height * 272.0f;
    sky_gradient_vdata[3].y = bottom;
    sky_gradient_vdata[2].y = bottom;
    if (dtheta < 16384) {
        float t = dtheta / 16384.0f;
        sky_gradient_vdata[0].color.ui = lerp_bgra8888(sky_gradient_colors[0].uc, sky_gradient_colors[2].uc, t).ui;
        sky_gradient_vdata[1].color.ui = lerp_bgra8888(sky_gradient_colors[0].uc, sky_gradient_colors[3].uc, t).ui;
        sky_gradient_vdata[2].color.ui = lerp_bgra8888(sky_gradient_colors[4].uc, sky_gradient_colors[6].uc, t).ui;
        sky_gradient_vdata[3].color.ui = lerp_bgra8888(sky_gradient_colors[4].uc, sky_gradient_colors[7].uc, t).ui;
    } else if (dtheta < 2 * 16384) {
        float t = (dtheta - 0x4000) / 16384.0f;
        sky_gradient_vdata[0].color.ui = lerp_bgra8888(sky_gradient_colors[2].uc, sky_gradient_colors[1].uc, t).ui;
        sky_gradient_vdata[1].color.ui = lerp_bgra8888(sky_gradient_colors[3].uc, sky_gradient_colors[1].uc, t).ui;
        sky_gradient_vdata[2].color.ui = lerp_bgra8888(sky_gradient_colors[6].uc, sky_gradient_colors[5].uc, t).ui;
        sky_gradient_vdata[3].color.ui = lerp_bgra8888(sky_gradient_colors[7].uc, sky_gradient_colors[5].uc, t).ui;
    } else if (dtheta < 3 * 16384) {
        float t = (dtheta - 0x8000) / 16384.0f;
        sky_gradient_vdata[0].color.ui = lerp_bgra8888(sky_gradient_colors[1].uc, sky_gradient_colors[3].uc, t).ui;
        sky_gradient_vdata[1].color.ui = lerp_bgra8888(sky_gradient_colors[1].uc, sky_gradient_colors[2].uc, t).ui;
        sky_gradient_vdata[2].color.ui = lerp_bgra8888(sky_gradient_colors[5].uc, sky_gradient_colors[7].uc, t).ui;
        sky_gradient_vdata[3].color.ui = lerp_bgra8888(sky_gradient_colors[5].uc, sky_gradient_colors[6].uc, t).ui;
    } else {
        float t = (dtheta - 0xC000) / 16384.0f;
        sky_gradient_vdata[0].color.ui = lerp_bgra8888(sky_gradient_colors[3].uc, sky_gradient_colors[0].uc, t).ui;
        sky_gradient_vdata[1].color.ui = lerp_bgra8888(sky_gradient_colors[2].uc, sky_gradient_colors[0].uc, t).ui;
        sky_gradient_vdata[2].color.ui = lerp_bgra8888(sky_gradient_colors[7].uc, sky_gradient_colors[4].uc, t).ui;
        sky_gradient_vdata[3].color.ui = lerp_bgra8888(sky_gradient_colors[6].uc, sky_gradient_colors[4].uc, t).ui;
    }
    ge::vertextype(
        GE_VTYPE_TC_NONE,
        GE_VTYPE_COL_8888,
        GE_VTYPE_NRM_NONE,
        GE_VTYPE_POS_S16,
        GE_VTYPE_WEIGHT_NONE,
        GE_VTYPE_IDX_NONE,
        0,
        0,
        true);
    ge::vaddr(sky_gradient_vdata);
    ge::prim(GE_PRIM_TRIANGLE_STRIP, 4);
}

template<typename T, typename U>
inline T lerp(T a, T b, U t) {
    return a + (T) (t * (b - a));
}

ScePspUnion32 StageBase::lerp_bgra8888(u8 *a, u8 *b, float t) {
    ScePspUnion32 result;
    result.uc[0] = lerp(a[2], b[2], t);
    result.uc[1] = lerp(a[1], b[1], t);
    result.uc[2] = lerp(a[0], b[0], t);
    result.uc[3] = lerp(a[3], b[3], t);
    return result;
}

void StageBase::draw_flash() {
    if (flash_state != 0) {
        ge::vertextype(
            GE_VTYPE_TC_NONE,
            GE_VTYPE_COL_8888,
            GE_VTYPE_NRM_NONE,
            GE_VTYPE_POS_S16,
            GE_VTYPE_WEIGHT_NONE,
            GE_VTYPE_IDX_NONE,
            0,
            0,
            true);
        u8 blend_mask;
        if (flash_state == 1) {
            blend_mask = 0xFF;
            ge::blendfixeda(0xFF, 0xFF, 0xFF);
            ge::blendfixedb(0xFF, 0xFF, 0xFF);
            ge::blendmode(GE_BLENDMODE_MUL_AND_SUBTRACT, GE_SRCBLEND_FIXA, GE_DSTBLEND_FIXB);
        } else if (flash_state == 2) {
            blend_mask = 0xFF - (u8)(flash_frames * 127.5f);
            flash_blank_vdata[1].color.rgba8888 = (blend_mask << 24) | 0x000000;
            flash_blank_vdata[0].color.rgba8888 = (blend_mask << 24) | 0x000000;
            ge::vaddr(flash_blank_vdata);
            ge::prim(GE_PRIM_RECTANGLES, 2);
            ge::blendfixeda(0xFF, 0xFF, 0xFF);
            ge::blendfixedb(0xFF, 0xFF, 0xFF);
            ge::blendmode(GE_BLENDMODE_MUL_AND_SUBTRACT, GE_SRCBLEND_FIXA, GE_DSTBLEND_FIXB);
        } else {
            float x = vsin_s(6.2831855f * (((360.0f * (float) ((flash_frames * 0xB6) + 0x7FFF + 0x4001)) / 65536.0f) / 360.0f));
            blend_mask = (x + 1.0f) * 255.0f;
            ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);
        }

        if (flash_state - 1U < 2) {
            flash_blend_vdata[1].color.rgba8888 = 0xFFFFFFFF;
            flash_blend_vdata[0].color.rgba8888 = 0xFFFFFFFF;
        } else {
            u32 color = (blend_mask << 24) | 0xFFFFFF;
            flash_blend_vdata[1].color.rgba8888 = color;
            flash_blend_vdata[0].color.rgba8888 = color;
        }
        ge::vaddr(flash_blend_vdata);
        ge::prim(GE_PRIM_RECTANGLES, 2);
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);
    }
}

void StageBase::method_088CEA2C() {
    flash_state = 0;
    flash_frames = 0;

    flash_blend_vdata[0].z = 0;
    flash_blend_vdata[0].y = 0;
    flash_blend_vdata[0].x = 0;
    flash_blend_vdata[1].x = 480;
    flash_blend_vdata[1].y = 272;
    flash_blend_vdata[1].z = 0;

    // oops! fortunately this object is allocated in the bss & the missed values are always 0
    flash_blend_vdata[0].z = 0;
    flash_blend_vdata[0].y = 0;
    flash_blank_vdata[0].x = 0;
    flash_blank_vdata[1].x = 480;
    flash_blank_vdata[1].y = 272;
    flash_blank_vdata[1].z = 0;
}

extern "C" {
    void *memcpy(void *, void *, int);
}


float *StageBase::compile_fog_params(float *in) {
    memcpy(&fog.color, in++, sizeof(fog.color));
    memcpy(&fog.begin, in++, sizeof(fog.begin));
    memcpy(&fog.end, in++, sizeof(fog.end));
    return in;
}

float *StageBase::compile_lights(float *in) {
    u32 size = sizeof(lights[0].position);
    u32 stride = size / sizeof(*in);
    memcpy(&lights[0].position, in + 0 * stride, size);
    memcpy(&lights[1].position, in + 1 * stride, size);
    memcpy(&lights[2].position, in + 2 * stride, size);
    memcpy(&lights[0].diffuse_color, in + 3 * stride, size);
    memcpy(&lights[1].diffuse_color, in + 4 * stride, size);
    memcpy(&lights[2].diffuse_color, in + 5 * stride, size);
    memcpy(&lights[0].ambient_color, in + 6 * stride, size);
    memcpy(&lights[1].ambient_color, in + 7 * stride, size);
    memcpy(&lights[2].ambient_color, in + 8 * stride, size);
    return in + 9 * stride;
}

inline u32 from_bgra8888(ScePspUnion32 &x) {
    return (x.uc[3] << 24) | (x.uc[0] << 16) |  (x.uc[1] << 8) | x.uc[2];
}

float *StageBase::compile_sky_gradient(float *in) {
    memcpy(&sky_gradient_origin, in, 0xC);
    memcpy(&sky_gradient_top, in + 3, 4);
    ScePspUnion32 x;
    memcpy(&x, in +  4, 4); sky_gradient_colors[0].ui = from_bgra8888(x);
    memcpy(&x, in +  5, 4); sky_gradient_colors[1].ui = from_bgra8888(x);
    memcpy(&x, in +  6, 4); sky_gradient_colors[2].ui = from_bgra8888(x);
    memcpy(&x, in +  7, 4); sky_gradient_colors[3].ui = from_bgra8888(x);
    memcpy(&x, in +  8, 4); sky_gradient_colors[4].ui = from_bgra8888(x);
    memcpy(&x, in +  9, 4); sky_gradient_colors[5].ui = from_bgra8888(x);
    memcpy(&x, in + 10, 4); sky_gradient_colors[6].ui = from_bgra8888(x);
    memcpy(&x, in + 11, 4); sky_gradient_colors[7].ui = from_bgra8888(x);
    return in + 12;
}

stage_definitions_0x28_t *StageBase::method_088CEDC0() {
    stage_definitions *d = definitions();
    stage_definitions_0x28_t *result;
    if (d != 0) {
        result = d->unknown_0x28;
    } else {
        result = NULL;
    }
    return result;
}

stage_definitions_0x2C_t *StageBase::method_088CEDF0() {
    stage_definitions *d = definitions();
    stage_definitions_0x2C_t *result;
    if (d != 0) {
        result = d->unknown_0x2C;
    } else {
        result = NULL;
    }
    return result;
}

stage_definitions_0x28_t *StageBase::vtable_0x34() {
    return NULL;
}

void *StageBase::vtable_0x38() {
    return NULL;
}

stage_draw_commands *StageBase::vtable_0x48() {
    return NULL;
}

void *StageBase::vtable_0x3C() {
    return NULL;
}

void *StageBase::vtable_0x40() {
    return NULL;
}

ScePspFVector4 *StageBase::vtable_0x44() {
    return NULL;
}

u8 StageBase::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *StageBase::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
