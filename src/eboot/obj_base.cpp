#include "obj_base.hpp"
#include "common.h"
#include "game_sys.hpp"
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
    unknown_0x410 = 0;
    setMemFn(&ObjBase::vtable_0x10);
}

ObjBase::~ObjBase() {

}

void *ObjBase::operator new(u32, void *p) {
    return p;
}

void ObjBase::operator delete(void *) {
    // empty
}

void ObjBase::vtable_0x10() {
    reset_transform();
    memset(&unknown_0x1F0, 0, sizeof(unknown_0x1F0));
    memset(&position, 0, sizeof(position));
    memset(&unknown_0x210, 0, sizeof(unknown_0x210));
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
        if ((bool)(flags & Draw::ALIVE) != false) {
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
    func_eboot_088641B8(&hierarchy);
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

void ObjBase::method_08865A24() {
    if (unknown_0x396 != 0) {
        unknown_0x33C += unknown_0x396;
        unknown_0x396 = 0;
    } else {
        if (unknown_0x33C > 0) {
            --unknown_0x33C;
        }
    }
}

void ObjBase::method_08865A5C(u32 flag) {
    if ((flag & 0x80000000) == 0) {
        unknown_0x288 |= flag;
    } else {
        unknown_0x28C |= flag & 0x7FFFFFFF;
    }
}

void ObjBase::method_08865A94(u32 flag) {
    if ((flag & 0x80000000) == 0) {
        unknown_0x288 &= ~flag;
    } else {
        unknown_0x28C &= ~(flag & 0x7FFFFFFF);
    }
}

void ObjBase::method_08865AD4(u16 flags, int a, int b) {
    switch(flags & 0xFF) {
    case 1:
        unknown_0x280 = 1;
        break;
    case 2:
        unknown_0x280 = 2;
        break;
    default:
        unknown_0x280 = 0;
        break;
    }

    if ((flags & 0x8000) != 0) {
        method_08865A94(8);
    } else {
        method_08865A5C(8);
    }

    if (a == 0) {
        method_08865A94(1);
    } else {
        method_08865A5C(1);
    }

    if (b == 0) {
        method_08865A94(2);
    } else {
        method_08865A5C(2);
    }
}

int ObjBase::inLoadedStage() {
    return stageId == GameSys::objectPtr->stage_id;
}

extern struct StageCoordinateInfo {
    float xOffset;
    float zOffset;
    float xMin; // guess
    float zMin; // guess
    float xMax; // guess
    float zMax; // guess
    float yMax; // guess
    float yMin; // guess
} D_game_sub_09CD9EB0[267];

void ObjBase::updateStagePosition() {
    StageCoordinateInfo &info = D_game_sub_09CD9EB0[stageId];
    stagePosition.x = position.x + info.xOffset;
    stagePosition.y = position.y + (info.yMax + info.yMin) / 2;
    stagePosition.z = position.z + info.zOffset;
}

void ObjBase::toStagePosition(ScePspFVector4 *out, u16 stageId, ScePspFVector4 *in) {
    StageCoordinateInfo &info = D_game_sub_09CD9EB0[stageId];
    out->x = in->x + info.xOffset;
    out->y = in->y + (info.yMax + info.yMin) / 2;
    out->z = in->z + info.zOffset;
}

u32 ObjBase::method_08865C80(u32 flag) {
    if ((flag & 0x80000000) != 0) {
        return unknown_0x28C & (flag & 0x7FFFFFFF);
    } else {
        return unknown_0x288 & flag;
    }
}

int ObjBase::testAnimation(bool isIdle, u8 animationId) {
    if (this->isIdle == isIdle && this->animationId == animationId) {
        return true;
    }
    return false;
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865CE4);

void ObjBase::method_08865D4C() {
    unknown_0x27C = D_game_task_09BB3C20[pl_type].unknown_0x54;
}

void ObjBase::method_08865D7C() {
    flags &= ~Draw::ALIVE;
    memFn = 0;
}

u8 ObjBase::vtable_0x30() {
    return alpha;
}

void ObjBase::vtable_0x34(float *x, float *y) {
    *x = 1;
    *y = 1;
}

#if BUILD_NONMATCHING

int ObjBase::method_08865DCC(s16 x) {
    float f = -1 * unknown_0x244;
    x /= 2;
    if (x <= 1 || f < 0) {
        unknown_0x254 = f;
        return 0;
    } else {
        unknown_0x254 = f / x;
        return 1;
    }
}

#else
INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", method_08865DCC__7ObjBaseFs);
#endif

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865E38);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865E5C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865E70);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865E8C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x2C__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865F1C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865F60);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865F84);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08866048);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_0886627C);
