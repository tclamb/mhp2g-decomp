#include "obj_base.hpp"
#include "common.h"
#include "game_sys.hpp"
#include "gamestage.hpp"
#include "model_base.hpp"
#include "draw_manager.hpp"

ObjBase::ObjBase() {
    nextObj = NULL;
    prevObj = NULL;
    unknown_0x1D7 = 0;
    unknown_0x1D6 = 0;
    unknown_0x1D5 = 0;
    state_0x1D4 = 0;
    unknown_0x414 = 0;
    kind = 3;
    objFlags = 0;
    setMemFn(&ObjBase::vtable_0x10);
}

ObjBase::~ObjBase() {
    // empty
}

void *ObjBase::operator new(u32, void *p) {
    return p;
}

void ObjBase::operator delete(void *) {
    // empty
}

void ObjBase::vtable_0x10() {
    reset_transform();
    memset(&rotation, 0, sizeof(rotation));
    memset(&position, 0, sizeof(position));
    memset(&next_position, 0, sizeof(next_position));
    // inline method?
    scale.z = 1; scale.y = 1; scale.x = 1;
    // inline method?
    unknown_0x324.w = 0; unknown_0x324.z = 0; unknown_0x324.y = 0; unknown_0x324.x = 0;
    diffuse_light_colors[0].ui = 0xFFFFFFFF;
    diffuse_light_colors[1].ui = 0xFFFFFFFF;
    diffuse_light_colors[2].ui = 0xFFFFFFFF;
    unknown_0x2BA = 0;
    unknown_0x2BC = 0;
    alpha = 0xFF;
    setMemFn(&ObjBase::vtable_0x38);
}

void ObjBase::vtable_0x14() {
    if (memFn != 0) {
        (this->*memFn)();
        if ((bool)(drawFlags & DrawFlag::ACTIVE) != false) {
            vtable_0x18();
            vtable_0x1C();
        }
    }
}

void ObjBase::vtable_0x18() {
    // empty
}

void ObjBase::vtable_0x1C() {
    // empty
}

void ObjBase::draw() {
    hierarchy.method_088641B8();
    model_pmo.drawWeight(&hierarchy, &model_tmh, &transform);
}

void pmo::drawWeight(Hierarchy *hierarchy, tmh *textures, ScePspFMatrix4 *transform) {
    DrawManager::objectPtr->world_model(transform);
    for (int i = 0; i < header->mesh_count; ++i) {
        drawWeightMesh(hierarchy, textures, i);
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", drawWeightMesh__3pmoFP9HierarchyP3tmhi);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_0886503C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x20__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_088652D4);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_088653C8);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865478);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865528);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_088655D8);

extern "C"
void func_eboot_088655F0(ScePspFVector4 *out, u16 *in) {
    float step = PI / 32768;
    out->x = step * (s32)in[0];
    out->y = step * (s32)in[2];
    out->z = step * (s32)in[4];
    out->w = 0;
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865640);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x24__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x28__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_0886592C);

void ObjBase::vtable_0x3C() {
    // empty
}

void ObjBase::hit_stop_calc() {
    if (unknown_0x396 != 0) {
        unknown_0x33C += unknown_0x396;
        unknown_0x396 = 0;
    } else {
        if (unknown_0x33C > 0) {
            --unknown_0x33C;
        }
    }
}

void ObjBase::pl_flag_set(u32 flag) {
    if ((flag & 0x80000000) == 0) {
        unknown_0x288 |= flag;
    } else {
        unknown_0x28C |= flag & 0x7FFFFFFF;
    }
}

void ObjBase::pl_flag_clr(u32 flag) {
    if ((flag & 0x80000000) == 0) {
        unknown_0x288 &= ~flag;
    } else {
        unknown_0x28C &= ~(flag & 0x7FFFFFFF);
    }
}

void ObjBase::method_08865AD4(u16 flags, int a, int b) {
    switch(flags & 0xFF) {
    case 1:
        posture = 1;
        break;
    case 2:
        posture = 2;
        break;
    default:
        posture = 0;
        break;
    }

    if ((flags & 0x8000) != 0) {
        pl_flag_clr(8);
    } else {
        pl_flag_set(8);
    }

    if (a == 0) {
        pl_flag_clr(1);
    } else {
        pl_flag_set(1);
    }

    if (b == 0) {
        pl_flag_clr(2);
    } else {
        pl_flag_set(2);
    }
}

bool ObjBase::Pl_stg_ck() {
    return stageId == GameSys::objectPtr->stage_id;
}

void ObjBase::World_calc() {
    StageWorldCoordinate &info = Gamestage::stage_world_tbl[stageId];
    world_position.x = position.x + info.x_offset;
    world_position.y = position.y + (info.y_max + info.y_min) / 2;
    world_position.z = position.z + info.z_offset;
}

void ObjBase::World_calc2(ScePspFVector4 *out, u16 stageId, ScePspFVector4 *in) {
    StageWorldCoordinate &info = Gamestage::stage_world_tbl[stageId];
    out->x = in->x + info.x_offset;
    out->y = in->y + (info.y_max + info.y_min) / 2;
    out->z = in->z + info.z_offset;
}

u32 ObjBase::pl_flag_ck(u32 flag) {
    if ((flag & 0x80000000) != 0) {
        return unknown_0x28C & (flag & 0x7FFFFFFF);
    } else {
        return unknown_0x288 & flag;
    }
}

bool ObjBase::act_ck(u8 type, u8 id) {
    if (action_type == type && action_id == id) {
        return true;
    }
    return false;
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", action_timer_calc__7ObjBaseFs);

void ObjBase::em_wall_bit_set() {
    wall_bit = D_game_task_09BB3C20[pl_type].wall_bit;
}

void ObjBase::method_08865D7C() {
    drawFlags &= ~DrawFlag::ACTIVE;
    memFn = 0;
}

u8 ObjBase::vtable_0x30() {
    return alpha;
}

void ObjBase::vtable_0x34(float *x, float *y) {
    *x = 1;
    *y = 1;
}

int ObjBase::rate_g_calc(s16 x) {
    float ay, vy = velocity.y;
    ay = vy * -1.0f;
    x /= 2;
    if (x <= 1 || vy < 0) {
        acceleration.y = ay;
        return 0;
    } else {
        acceleration.y = ay / x;
        return 1;
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865E38);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865E5C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865E70);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865E8C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x2C__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", get_joint_pos__9ModelBaseFP14ScePspFVector4s);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", get_joint_wmat__9ModelBaseFs);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865F84);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08866048);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_0886627C);
