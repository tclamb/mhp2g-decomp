#include "bone.hpp"

#include "vfpu.h"

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885F840);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885F8E0);

bone *bone::func_eboot_0885F920(bone *arg1, u32 arg2) {
    bone *temp = arg1;
    while (true) {
        if (arg1->key == arg2 || arg2 == -1) {
            return arg1;
        } else if (arg1->child != 0) {
            arg1 = arg1->child;
        } else if (arg1->sibling != 0) {
            arg1 = arg1->sibling;
        } else if (arg1 != temp) {
            while (true) {
                if (arg1->sibling != 0) {
                    arg1 = arg1->sibling;
                    break;
                } else {
                    arg1 = arg1->parent;
                }
                if (arg1 == temp) {
                    return 0;
                }
            }
        } else {
            return 0;
        }
    }
    return arg1;
}

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885F998);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885FA04);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885FB4C);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885FBAC);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885FBEC);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885FC94);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885FE3C);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_0885FF54);

extern "C"
void func_eboot_088633C4(ScePspFMatrix4*, ScePspFMatrix3*, ScePspFMatrix3*, ScePspFMatrix4*, u16*, float);

void bone::func_eboot_0885FFB4(ScePspFVector4 *arg1, s32 arg2, float arg3, float arg4) {
    struct {
        ScePspFMatrix4 b;
        ScePspFMatrix4 a;
        void *pad[2];
        ScePspFMatrix3 c;
    } s;
    ScePspFMatrix3 *new_var3;
    ScePspFVector4 *new_var, *new_var2;
    bind_pose *pafVar2;

    if (arg2 == 0) {
        pafVar2 = &bind;
    } else {
        pafVar2 = &alt_bind;
    }
    new_var3 = &f0x120;
    func_eboot_088633C4(&pafVar2->transform, new_var3, &s.c, &s.a, &pafVar2->f0x64[4], arg3);
    new_var3 = &f0x120;
    func_eboot_088633C4(&pafVar2->transform, new_var3, &s.c, &s.b, &pafVar2->f0x64[4], arg4);

    new_var2 = &s.b.w;
    new_var = &s.a.w;
    vsub_q(arg1, new_var2, new_var);

    arg1->w = 0.0f;
}

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_08860054);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_08860254);

void bone::update(ScePspFMatrix4 *parentGlobalPose, ScePspFMatrix4 *parentLocalPose, float x, float y, float z) {
    float sx, sy, sz;
    ScePspFMatrix4 invScale;
    sx = bind.scale.x * x;
    sy = bind.scale.y * y;
    sz = bind.scale.z * z;
    scaleMatrix(&localPose, sx, sy, sz);
    vmidt_q(&invScale);
    invScale.x.x = 1.0f / x;
    invScale.y.y = 1.0f / y;
    invScale.z.z = 1.0f / z;
    invScale.w.x = bind.transform.w.x;
    invScale.w.y = bind.transform.w.y;
    invScale.w.z = bind.transform.w.z;
    vmmul_t(&localPose, &bind.transform, &localPose);
    vmmul_q(&localPose, &localPose, &invScale);
    vmmul_q(&globalPose, &localPose, parentGlobalPose);
    vmmul_q(&localPose, &localPose, parentLocalPose);

    if (child) {
        child->update(&globalPose, &localPose, sx, sy, sz);
    }
    if (sibling) {
        sibling->update(parentGlobalPose, parentLocalPose, x, y, z);
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_08860534);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_08860640);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_088606C8);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_088606D8);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", func_eboot_08860700);

bone::~bone() {}

void *bone::operator new(u32 size, void *p) {
    return p;
}

void bone::operator delete(void *p) {
    return;
}
