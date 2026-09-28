#include "obj_base.hpp"
#include "common.h"
#include "game_sys.hpp"
#include "model_base.hpp"
#include "draw_manager.hpp"
#include "vfpu.h"
#include "sound.hpp"

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
    memset(&last_position, 0, sizeof(last_position));
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

extern "C" void *vtable_0x20__7ObjBaseFv(ObjBase *obj, u32 stamp) {
    u8 *base = *(u8 **)((u8 *)obj + 0x1AC);
    if (base == NULL) {
        return NULL;
    }
    u32 time = stamp % 1000;
    u32 phase = time % 100;
    u32 interval = time / 100;
    u32 *entry = (u32 *)(base + interval * 8);
    if (phase >= entry[0]) {
        return NULL;
    }
    u32 offset = entry[1];
    s32 value = *(s32 *)(base + offset + phase * 4);
    if (value == -1) {
        return NULL;
    }
    return base + value;
}

extern "C" void func_eboot_088655F0(ScePspFVector4 *, u16 *);
extern "C" void func_eboot_088655D8(ScePspFMatrix4 *);
extern "C" void func_eboot_088653C8(ScePspFMatrix4 *, ScePspFMatrix4 *, float);
extern "C" void func_eboot_08865478(ScePspFMatrix4 *, ScePspFMatrix4 *, float);
extern "C" void func_eboot_08865528(ScePspFMatrix4 *, ScePspFMatrix4 *, float);

extern "C" void func_eboot_088652D4(ObjBase *obj) {
    ScePspFMatrix4 scaleMatrix;
    ScePspFVector4 angles;
    float sz = obj->scale.z;
    float sy = obj->scale.y;
    float sx = obj->scale.x;
    vmidt_q(&scaleMatrix);
    scaleMatrix.x.x = sx;
    scaleMatrix.y.y = sy;
    scaleMatrix.z.z = sz;
    func_eboot_088655F0(&angles, (u16 *)&obj->unknown_0x1F0);
    func_eboot_088655D8(&obj->transform);
    func_eboot_08865528(&obj->transform, &obj->transform, angles.y);
    func_eboot_08865478(&obj->transform, &obj->transform, angles.x);
    func_eboot_088653C8(&obj->transform, &obj->transform, angles.z);
    float pz = obj->position.z;
    float py = obj->position.y;
    float px = obj->position.x;
    obj->transform.w.x = px;
    obj->transform.w.y = py;
    obj->transform.w.z = pz;
    vmmul_t(&obj->transform, &scaleMatrix, &obj->transform);
}

extern "C" void func_eboot_088653C8(ScePspFMatrix4 *out, ScePspFMatrix4 *in, float angle) {
    ScePspFMatrix4 matrix;
    vmidt_q(&matrix);
    float rotation = angle;
#ifdef __MWERKS__
    __asm__ (
        "lv.s S100, %3"
        "vcst.s S101, VFPU_2_PI"
        "vmul.s S100, S100, S101"
        "vrot.q C000, S100, [C, S, 0, 0]"
        "vrot.q C010, S100, [-S, C, 0, 0]"
        "vidt.q C020"
        "vidt.q C030"
        "vmmov.q E200, E000"
        "lv.q C100, 0x0(%2)"
        "lv.q C110, 0x10(%2)"
        "lv.q C120, 0x20(%2)"
        "lv.q C130, 0x30(%2)"
        "vmmul.q E200, E100, E000"
        "sv.q C200, 0x0(%2)"
        "sv.q C210, 0x10(%2)"
        "sv.q C220, 0x20(%2)"
        "sv.q C230, 0x30(%2)"
        "lv.q C100, 0x0(%2)"
        "lv.q C110, 0x10(%2)"
        "lv.q C120, 0x20(%2)"
        "lv.q C200, 0x0(%1)"
        "lv.q C210, 0x10(%1)"
        "lv.q C220, 0x20(%1)"
        "vmmul.t E000, E100, E200"
        "sv.s S000, 0x0(%0)"
        "sv.s S001, 0x4(%0)"
        "sv.s S002, 0x8(%0)"
        "sv.s S010, 0x10(%0)"
        "sv.s S011, 0x14(%0)"
        "sv.s S012, 0x18(%0)"
        "sv.s S020, 0x20(%0)"
        "sv.s S021, 0x24(%0)"
        "sv.s S022, 0x28(%0)"
        : "=m"(*out)
        : "m"(*in), "m"(matrix), "m"(rotation)
    );
#endif
}

#define DEFINE_OBJ_ROTATE(NAME, COLUMNS) \
extern "C" void NAME(ScePspFMatrix4 *out, ScePspFMatrix4 *in, float angle) { \
    ScePspFMatrix4 matrix; \
    vmidt_q(&matrix); \
    float rotation = angle; \
    __asm__ ( \
        "lv.s S100, %3" \
        "vcst.s S101, VFPU_2_PI" \
        "vmul.s S100, S100, S101" \
        COLUMNS \
        "vmmov.q E200, E000" \
        "lv.q C100, 0x0(%2)" \
        "lv.q C110, 0x10(%2)" \
        "lv.q C120, 0x20(%2)" \
        "lv.q C130, 0x30(%2)" \
        "vmmul.q E200, E100, E000" \
        "sv.q C200, 0x0(%2)" \
        "sv.q C210, 0x10(%2)" \
        "sv.q C220, 0x20(%2)" \
        "sv.q C230, 0x30(%2)" \
        "lv.q C100, 0x0(%2)" \
        "lv.q C110, 0x10(%2)" \
        "lv.q C120, 0x20(%2)" \
        "lv.q C200, 0x0(%1)" \
        "lv.q C210, 0x10(%1)" \
        "lv.q C220, 0x20(%1)" \
        "vmmul.t E000, E100, E200" \
        "sv.s S000, 0x0(%0)" \
        "sv.s S001, 0x4(%0)" \
        "sv.s S002, 0x8(%0)" \
        "sv.s S010, 0x10(%0)" \
        "sv.s S011, 0x14(%0)" \
        "sv.s S012, 0x18(%0)" \
        "sv.s S020, 0x20(%0)" \
        "sv.s S021, 0x24(%0)" \
        "sv.s S022, 0x28(%0)" \
        : "=m"(*out) \
        : "m"(*in), "m"(matrix), "m"(rotation) \
    ); \
}

DEFINE_OBJ_ROTATE(func_eboot_08865478,
    "vidt.q C000"
    "vrot.q C010, S100, [0, C, S, 0]"
    "vrot.q C020, S100, [0, -S, C, 0]"
    "vidt.q C030")

DEFINE_OBJ_ROTATE(func_eboot_08865528,
    "vrot.q C000, S100, [C, 0, -S, 0]"
    "vidt.q C010"
    "vrot.q C020, S100, [S, 0, C, 0]"
    "vidt.q C030")

#undef DEFINE_OBJ_ROTATE

extern "C" void func_eboot_088655D8(ScePspFMatrix4 *matrix) {
    vmidt_q(matrix);
}

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

extern "C" void func_eboot_0886592C(ObjBase *obj) {
    u8 *p = (u8 *)obj;
    if (obj->unknown_0x33C == 0) {
        if (obj->method_08865C80(0x400001) != 0) {
            *(u16 *)(p + 0x45C) += 2;
        } else {
            *(u16 *)(p + 0x45C) = 0;
        }
        *(u32 *)(p + 0x3F4) += 2;
        if (*(s32 *)(p + 0x3F4) >= 0x186A0) {
            *(u32 *)(p + 0x3F4) = 0x2710;
        }
        if (p[0x276] != 0) {
            --p[0x276];
        }
        obj->vtable_0x3C();
    }
    int active = (*(u32 *)((u8 *)GameSys::objectPtr + 0x6AF14) & 1) != 0;
    if (active == 0) {
        obj->method_08865A94(0x800);
        if (obj->method_08865C80(1) != 0) {
            obj->method_08865A5C(0x2000);
        } else {
            obj->method_08865A94(0x2000);
        }
    }
}

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

bool ObjBase::pl_action_ck(u8 type, u8 id) {
    if (action_type == type && action_id == id) {
        return true;
    }
    return false;
}

extern "C" void func_eboot_08865CE4(ObjBase *obj, int amount) {
    u8 *p = (u8 *)obj;
    s16 left = *(s16 *)(p + 0x334);
    s16 right = *(s16 *)(p + 0x32C);
    *(u32 *)(p + 0x3F4) = 0;
    if (left < 0) {
        left = -left;
    }
    *(u32 *)(p + 0x3F4) += (s16)right - (s16)left;
    if ((s16)amount != 0) {
        if (left != 0) {
            --left;
        }
        p[0x426] += (u8)((s16)left - (s16)right);
    }
}

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

int ObjBase::method_08865DCC(s16 x) {
    float f = -1 * unknown_0x244;
    x /= 2;
    if (x <= 1 || unknown_0x244 < 0) {
        unknown_0x254 = f;
        return 0;
    } else {
        unknown_0x254 = f / x;
        return 1;
    }
}

extern "C" void func_eboot_08865E38(ObjBase *obj) {
    u32 *p = (u32 *)((u8 *)obj + 0x240);
    p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0;
    p[4] = 0; p[5] = 0; p[6] = 0; p[7] = 0;
}

extern "C" void func_eboot_08865E5C(ObjBase *obj) {
    u32 *p = (u32 *)((u8 *)obj + 0x250);
    p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0;
}

extern "C" void func_eboot_08865E70(ObjBase *obj) {
    vadd_t(&obj->position, &obj->position, (ScePspFVector4 *)((u8 *)obj + 0x240));
}

extern "C" void func_eboot_08865E8C(ObjBase *obj) {
    ScePspFVector4 *position = &obj->position;
    ScePspFVector4 *velocity = (ScePspFVector4 *)((u8 *)obj + 0x240);
    ScePspFVector4 *acceleration = (ScePspFVector4 *)((u8 *)obj + 0x250);
#ifdef __MWERKS__
    __asm__ (
        "lv.q C000, %1"
        "lv.q C010, %2"
        "lv.q C020, %3"
        "vadd.t C010, C010, C020"
        "vadd.t C000, C000, C010"
        "sv.q C010, %2"
        "sv.q C000, %0"
        : "=m"(*position)
        : "m"(*position), "m"(*velocity), "m"(*acceleration)
    );
#else
    velocity->x += acceleration->x;
    velocity->y += acceleration->y;
    velocity->z += acceleration->z;
    position->x += velocity->x;
    position->y += velocity->y;
    position->z += velocity->z;
#endif
}

void ObjBase::vtable_0x2C() {
    u8 *p = (u8 *)this;
    p[0x285] = 0;
    *(u16 *)(p + 0x394) = 0;
    *(u16 *)(p + 0x3B8) = 0;
    p[0x3B0] = 0;
    *(u16 *)(p + 0x3BA) = 0;
    p[0x3B1] = 0;
    *(u16 *)(p + 0x3BC) = 0;
    p[0x3B2] = 0;
    *(u16 *)(p + 0x3BE) = 0;
    p[0x3B3] = 0;
    *(u16 *)(p + 0x3C0) = 0;
    p[0x3B4] = 0;
    *(u16 *)(p + 0x3C2) = 0;
    p[0x3B5] = 0;
    *(u16 *)(p + 0x3C4) = 0;
    p[0x3B6] = 0;
    *(u16 *)(p + 0x3C6) = 0;
    p[0x3B7] = 0;
    *(u16 *)(p + 0x38C) = 0;
    *(u16 *)(p + 0x38E) = 0;
    *(u16 *)(p + 0x392) = 0;
    *(u16 *)(p + 0x390) = 0;
    p[0x3CE] = 0;
    *(u32 *)(p + 0x2F0) = 0;
}

extern "C" u8 *func_eboot_08864214(void *, s16);

extern "C" void func_eboot_08865F1C(ObjBase *obj, float *out, int index) {
    u8 *joint = func_eboot_08864214((u8 *)obj + 0x80, (s16)index);
    out[0] = *(float *)(joint + 0x100);
    out[1] = *(float *)(joint + 0x104);
    out[2] = *(float *)(joint + 0x108);
}

extern "C" void *func_eboot_08865F60(ObjBase *obj, int index) {
    return func_eboot_08864214((u8 *)obj + 0x80, (s16)index) + 0xD0;
}

extern "C" int func_eboot_08865F84(ObjBase *obj, u32 a, u32 b, u32 c, u32 d,
                                     ScePspFVector4 *position, u32 g, u32 h, u32 i) {
    int result = 0;
    if ((u8)obj->inLoadedStage() == true) {
        result = func_eboot_08883858(Sound::objectPtr, 1, a, b, c, d,
                                     position, g, h, i, false);
    }
    return result;
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08866048);

extern "C" u32 func_eboot_0886627C(ObjBase *obj) {
    u32 flag;
    switch (obj->kind) {
    case 0:
        flag = 0x20;
        break;
    case 1:
        flag = 0x40;
        break;
    case 2:
        flag = 0x40;
        break;
    default:
        flag = 0xE0;
        break;
    }
    return flag | obj->unknown_0x1E4;
}
