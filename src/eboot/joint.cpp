#include "joint.hpp"

#include "vfpu.h"

INCLUDE_ASM("asm/eboot/nonmatchings/joint", func_eboot_0885F840);

extern "C" void func_eboot_0885F8E0(Joint *joint, ScePspFVector4 *out, s32 mode, float start, float end) {
    if (start < 0.0f) {
        joint->func_eboot_0885FFB4(out, mode, 0.0f, end - start);
    } else {
        joint->func_eboot_0885FFB4(out, mode, start, end);
    }
}

Joint *Joint::func_eboot_0885F920(Joint *arg1, u32 arg2) {
    Joint *temp = arg1;
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

extern "C" void func_eboot_0885F840(Joint *, Joint *, void *, u32, int);
extern "C" void func_eboot_0885F998(Joint *root, void *data, u32 key, int useAlt) {
    Joint *joint = root->func_eboot_0885F920(root, key);
    if (joint) {
        func_eboot_0885F840(root, joint, data, key, useAlt);
    }
}

extern "C" void func_eboot_08863190(void *, void *, u32, void *);
extern "C" void func_eboot_088632D0(void *, void *, void *);
extern "C" void func_eboot_0885FA04(Joint *joint, float frame, u32 key, int alt) {
    u8 useAlt = alt;
    u8 *pose;
    if (useAlt == 0) pose = (u8 *)&joint->bind;
    else pose = (u8 *)&joint->alt_bind;
    if (joint->key == key || key == (u32)-1) {
        int i = 0;
        u8 *walk = (u8 *)joint;
        u8 *out = pose;
        do {
            *(float *)(out + 0x40) = *(float *)(walk + 0x120);
            *(float *)(out + 0x44) = *(float *)(walk + 0x124);
            *(float *)(out + 0x48) = *(float *)(walk + 0x128);
            walk += 0xC;
            out += 0xC;
            i += 3;
        } while (i < 9);
        if (*(void **)(pose + 0x64)) {
            func_eboot_08863190(pose, pose + 0x40, (u32)frame, pose + 0x6C);
        }
        func_eboot_088632D0(pose, pose, pose + 0x40);
        if (joint->child) {
            func_eboot_0885FA04(joint->child, frame, key, alt);
        }
    }
    if (joint->sibling) {
        func_eboot_0885FA04(joint->sibling, frame, key, alt);
    }
}

extern "C" void func_eboot_0885FA04(Joint *, float, u32, int);
extern "C" void func_eboot_0885FB4C(Joint *root, u32 key, int useAlt, float frame) {
    Joint *joint = root->func_eboot_0885F920(root, key);
    if (joint) {
        func_eboot_0885FA04(joint, frame, key, useAlt);
    }
}

extern "C" void func_eboot_0885FBEC(Joint *, u32);
extern "C" void func_eboot_0885FBAC(Joint *root, u32 key) {
    Joint *joint = root->func_eboot_0885F920(root, key);
    if (joint) {
        func_eboot_0885FBEC(joint, key);
    }
}

extern "C" void func_eboot_0885FBEC(Joint *joint, u32 key) {
    if (joint->key == key || key == (u32)-1) {
#if defined(__MWERKS__)
        __asm__ (
            "lv.q C000, 0x0(%0)"
            "lv.q C010, 0x10(%0)"
            "lv.q C020, 0x20(%0)"
            "lv.q C030, 0x30(%0)"
            "sv.q C000, 0x0(%1)"
            "sv.q C010, 0x10(%1)"
            "sv.q C020, 0x20(%1)"
            "sv.q C030, 0x30(%1)"
            : "+m" (joint->bind.transform), "+m" (joint->alt_bind.transform)
        );
#else
        joint->alt_bind.transform = joint->bind.transform;
#endif
        joint->alt_bind.scale.x = joint->bind.scale.x;
        joint->alt_bind.scale.y = joint->bind.scale.y;
        joint->alt_bind.scale.z = joint->bind.scale.z;
        if (joint->child) {
            func_eboot_0885FBEC(joint->child, key);
        }
    }
    if (joint->sibling) {
        func_eboot_0885FBEC(joint->sibling, key);
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/joint", func_eboot_0885FC94);

inline void normalize_t(ScePspFVector4 *out, ScePspFVector4 *v) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C000, %1"
        "vdot.t S010, C000, C000"
        "vzero.s S011"
        "vcmp.s EZ, S010, S010"
        "nop"
        "vrsq.s S010, S010"
        "vcmovt.s S010, S011, 0"
        "vpfxd 0xFF"
        "vscl.t C000, C000, S010"
        "sv.q C000, %0"
        : "=m"(*out)
        : "m"(*v)
    );
#else
    float f = v->x * v->x + v->y * v->y + v->z * v->z;
    if (f == 0.0f) {
        out->x = out->y = out->z = 0.0f;
    }
    f = 1.0f / sqrtf(f);
    out->x = v->x * f;
    out->y = v->y * f;
    out->z = v->z * f;
#endif
}

extern "C"
void func_eboot_0885FE3C(ScePspFMatrix4 *out, ScePspFVector4 *dir, ScePspFVector4 *up) {
    ScePspFVector4 nA, U, R, nB;
    normalize_t(&nA, dir);
    normalize_t(&nB, up);
    flvecOuterProduct(&R, &nA, &nB);
    normalize_t(&R, &R);
    flvecOuterProduct(&U, &R, &nA);

    out->x.w = 0;
    out->y.w = 0;
    out->z.w = 0;
    out->w.w = 1.0f;
    out->x.x = nA.x; out->x.y = nA.y; out->x.z = nA.z;
    out->y.x = U.x;  out->y.y = U.y;  out->y.z = U.z;
    out->z.x = R.x;  out->z.y = R.y;  out->z.z = R.z;
}

extern "C" void func_eboot_0885FC94(Joint *, float, float, u32);
extern "C" void func_eboot_0885FF54(Joint *root, u32 key, float a, float b) {
    Joint *joint = root->func_eboot_0885F920(root, key);
    if (joint) {
        func_eboot_0885FC94(joint, a, b, key);
    }
}

extern "C"
void func_eboot_088633C4(ScePspFMatrix4*, ScePspFMatrix3*, ScePspFMatrix3*, ScePspFMatrix4*, u16*, float);

void Joint::func_eboot_0885FFB4(ScePspFVector4 *arg1, s32 arg2, float arg3, float arg4) {
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

inline void jointRotateX(ScePspFMatrix4 *m, float angle) {
#ifdef __MWERKS__
    __asm__ (
        "lv.s S100, %1"
        "vcst.s S101, VFPU_2_PI"
        "vmul.s S100, S100, S101"
        "vidt.q C000"
        "vrot.q C010, S100, [0,C,S,0]"
        "vrot.q C020, S100, [0,-S,C,0]"
        "vidt.q C030"
        "lv.q C100, 0x0(%0)"
        "lv.q C110, 0x10(%0)"
        "lv.q C120, 0x20(%0)"
        "lv.q C130, 0x30(%0)"
        "vmmul.q M200, M000, M100"
        "sv.q C200, 0x0(%0)"
        "sv.q C210, 0x10(%0)"
        "sv.q C220, 0x20(%0)"
        "sv.q C230, 0x30(%0)"
        : "+m"(*m)
        : "m"(angle)
    );
#endif
}
inline void jointRotateY(ScePspFMatrix4 *m, float angle) {
#ifdef __MWERKS__
    __asm__ (
        "lv.s S100, %1"
        "vcst.s S101, VFPU_2_PI"
        "vmul.s S100, S100, S101"
        "vrot.q C000, S100, [C,0,-S,0]"
        "vidt.q C010"
        "vrot.q C020, S100, [S,0,C,0]"
        "vidt.q C030"
        "lv.q C100, 0x0(%0)"
        "lv.q C110, 0x10(%0)"
        "lv.q C120, 0x20(%0)"
        "lv.q C130, 0x30(%0)"
        "vmmul.q M200, M000, M100"
        "sv.q C200, 0x0(%0)"
        "sv.q C210, 0x10(%0)"
        "sv.q C220, 0x20(%0)"
        "sv.q C230, 0x30(%0)"
        : "+m"(*m)
        : "m"(angle)
    );
#endif
}
inline void jointRotateZ(ScePspFMatrix4 *m, float angle) {
#ifdef __MWERKS__
    __asm__ (
        "lv.s S100, %1"
        "vcst.s S101, VFPU_2_PI"
        "vmul.s S100, S100, S101"
        "vrot.q C000, S100, [C,S,0,0]"
        "vrot.q C010, S100, [-S,C,0,0]"
        "vidt.q C020"
        "vidt.q C030"
        "lv.q C100, 0x0(%0)"
        "lv.q C110, 0x10(%0)"
        "lv.q C120, 0x20(%0)"
        "lv.q C130, 0x30(%0)"
        "vmmul.q M200, M000, M100"
        "sv.q C200, 0x0(%0)"
        "sv.q C210, 0x10(%0)"
        "sv.q C220, 0x20(%0)"
        "sv.q C230, 0x30(%0)"
        : "+m"(*m)
        : "m"(angle)
    );
#endif
}
extern "C" void func_eboot_08860054(Joint *joint, u8 *data) {
    float *src = (float *)data;
    float *r = (float *)&joint->f0x120;
    float *b = (float *)&joint->bind.scale;
    r[0] = src[7]; r[1] = src[8]; r[2] = src[9];
    r[3] = src[11]; r[4] = src[12]; r[5] = src[13];
    r[6] = src[15]; r[7] = src[16]; r[8] = src[17];
    b[0] = src[7]; b[1] = src[8]; b[2] = src[9];
    b[3] = src[11]; b[4] = src[12]; b[5] = src[13];
    b[6] = src[15]; b[7] = src[16]; b[8] = src[17];
    joint->f0x110 = *(u32 *)(data + 0);
    joint->f0x116 = *(u32 *)(data + 0xC);
    *(u32 *)&joint->f0x118 = *(u32 *)(data + 0x4C);
    joint->key = *(u32 *)(data + 0x50);
    vmidt_q(&joint->localPose);
    float sz = src[9], sy = src[8], sx = src[7];
    vmidt_q(&joint->bind.transform);
    joint->bind.transform.x.x = sx;
    joint->bind.transform.y.y = sy;
    joint->bind.transform.z.z = sz;
    float ax = src[11];
    jointRotateX(&joint->bind.transform, ax);
    float ay = src[12];
    jointRotateY(&joint->bind.transform, ay);
    float az = src[13];
    jointRotateZ(&joint->bind.transform, az);
    joint->bind.transform.w.x = src[15];
    joint->bind.transform.w.y = src[16];
    joint->bind.transform.w.z = src[17];
}

extern "C" float func_eboot_08863644(ScePspFMatrix4 *, void *);

extern "C" float func_eboot_08860254(Joint *joint, u8 *data) {
    float max = 0.0f;
    u8 *group = data + 0x14;
    u8 *item;
    u32 i = 0;
    while (i < *(u32 *)(data + 4)) {
        item = group + 0xC;
        for (u32 j = 0; j < *(u32 *)(group + 4); ++j) {
            float v = func_eboot_08863644(&joint->bind.transform, item);
            if (v > max) max = v;
            item += *(u32 *)(item + 8);
        }
        group += *(u32 *)(group + 8);
        ++i;
    }
    return max;
}

void Joint::update(ScePspFMatrix4 *parentGlobalPose, ScePspFMatrix4 *parentLocalPose, float x, float y, float z) {
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

INCLUDE_ASM("asm/eboot/nonmatchings/joint", func_eboot_08860534);

extern "C" void func_eboot_08860640(Joint *joint) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C100, 0x0(%1)"
        "lv.q C110, 0x10(%1)"
        "lv.q C120, 0x20(%1)"
        "lv.q C130, 0x30(%1)"
        "lv.q C200, 0x0(%2)"
        "lv.q C210, 0x10(%2)"
        "lv.q C220, 0x20(%2)"
        "lv.q C230, 0x30(%2)"
        "vmmul.q E000, E100, E200"
        "sv.q C000, 0x0(%0)"
        "sv.q C010, 0x10(%0)"
        "sv.q C020, 0x20(%0)"
        "sv.q C030, 0x30(%0)"
        : "=m" (joint->f0x50)
        : "m" (joint->f0x10), "m" (joint->localPose)
    );
#else
    vmmulr_q(&joint->f0x50, &joint->localPose, &joint->f0x10);
#endif
    if (joint->child) {
        func_eboot_08860640(joint->child);
    }
    if (joint->sibling) {
        func_eboot_08860640(joint->sibling);
    }
}

extern "C" void func_eboot_088606C8(Joint *joint, float x, float y, float z) {
    joint->bind.scale.x = x;
    joint->bind.scale.y = y;
    joint->bind.scale.z = z;
}

extern "C" void func_eboot_088606D8(Joint *joint, float x, float y, float z) {
    joint->bind.scale.x += x;
    joint->bind.scale.y += y;
    joint->bind.scale.z += z;
}

extern "C" void func_eboot_08860700(Joint *joint, float z) {
    joint->bind.scale.z = z;
}

Joint::~Joint() {}

void *Joint::operator new(u32 size, void *p) {
    return p;
}

void Joint::operator delete(void *p) {
    return;
}
