#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

inline void sv_q(ScePspFVector4 *v, float x, float y, float z, float w) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.s S000, %1"
        "lv.s S001, %2"
        "lv.s S002, %3"
        "lv.s S003, %4"
        "sv.q C000, %0"
        : "=m"(*v)
        : "m"(x), "m"(y), "m"(z), "m"(w)
    );
#else
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
#endif
}

inline void vsub_q(ScePspFVector4 *v, ScePspFVector4 *a, ScePspFVector4 *b) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C000, %1"
        "lv.q C010, %2"
        "vsub.q C000, C000, C010"
        "sv.q C000, %0"
        : "=m"(*v)
        : "m"(*a), "m"(*b)
    );
#else
    v->x = a->x - b->x;
    v->y = a->y - b->y;
    v->z = a->z - b->z;
    v->w = a->w - b->w;
#endif
}

inline void vscl_q(ScePspFVector4 *v, ScePspFVector4 *a, float b) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C000, %1"
        "lv.s S010, %2"
        "vscl.q C000, C000, S010"
        "sv.q C000, %0"
        : "=m"(*v)
        : "m"(*a), "m"(b)
    );
#else
    v->x = a->x * b;
    v->y = a->y * b;
    v->z = a->z * b;
    v->w = a->w * b;
#endif
}

void vadd_q(ScePspFVector4 *v, ScePspFVector4 *a, ScePspFVector4 *b);
/*
void vadd_q(ScePspFVector4 *v, ScePspFVector4 *a, ScePspFVector4 *b) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C000, %1"
        "lv.q C010, %2"
        "vadd.q C000, C000, C010"
        "sv.q C000, %0"
        : "=m"(*v)
        : "m"(*a), "m"(*b)
    );
#else
    v->x = a->x + b->x;
    v->y = a->y + b->y;
    v->z = a->z + b->z;
    v->w = a->w + b->w;
#endif
}
*/

inline void vmidt_q(ScePspFMatrix4 *m) {
#if defined(__MWERKS__)
    __asm__ (
        "vmidt.q E000"
        "sv.q C000, 0x0(%0)"
        "sv.q C010, 0x10(%0)"
        "sv.q C020, 0x20(%0)"
        "sv.q C030, 0x30(%0)"
        : "=m" (*m)
    );
#else
    m->x.x = 1; m->x.y = 0; m->x.z = 0; m->x.w = 0;
    m->y.x = 0; m->y.y = 1; m->y.z = 0; m->y.w = 0;
    m->z.x = 0; m->z.y = 0; m->z.z = 1; m->z.w = 0;
    m->w.x = 0; m->w.y = 0; m->w.z = 0; m->w.w = 1;
#endif
}

inline void vmmul_t(ScePspFMatrix4 *m, ScePspFMatrix4 *a, ScePspFMatrix4 *b) {
#if defined(__MWERKS__)
    __asm__ (
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
        : "=m" (*m)
        : "m" (*a), "m" (*b)
    );
#else
    // TODO
    vmidt_q(m);
#endif
}

inline void vmmul_q(ScePspFMatrix4 *m0, ScePspFMatrix4 *m1, ScePspFMatrix4 *m2) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C100, 0x0(%2)"
        "lv.q C110, 0x10(%2)"
        "lv.q C120, 0x20(%2)"
        "lv.q C130, 0x30(%2)"
        "lv.q C200, 0x0(%1)"
        "lv.q C210, 0x10(%1)"
        "lv.q C220, 0x20(%1)"
        "lv.q C230, 0x30(%1)"
        "vmmul.q E000, E200, E100"
        "sv.q C000, 0x0(%0)"
        "sv.q C010, 0x10(%0)"
        "sv.q C020, 0x20(%0)"
        "sv.q C030, 0x30(%0)"
        : "=m" (*m0)
        : "m" (*m1), "m" (*m2)
    );
#else
    // TODO
    vmidt_q(m0);
#endif
}

inline void vmmulr_q(ScePspFMatrix4 *m0, ScePspFMatrix4 *m1, ScePspFMatrix4 *m2) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C100, 0x0(%2)"
        "lv.q C110, 0x10(%2)"
        "lv.q C120, 0x20(%2)"
        "lv.q C130, 0x30(%2)"
        "lv.q C200, 0x0(%1)"
        "lv.q C210, 0x10(%1)"
        "lv.q C220, 0x20(%1)"
        "lv.q C230, 0x30(%1)"
        "vmmul.q E000, E100, E200"
        "sv.q C000, 0x0(%0)"
        "sv.q C010, 0x10(%0)"
        "sv.q C020, 0x20(%0)"
        "sv.q C030, 0x30(%0)"
        : "=m" (*m0)
        : "m" (*m1), "m" (*m2)
    );
#else
    // TODO
    vmidt_q(m0);
#endif
}

inline void rotateX(ScePspFMatrix4 *m, float angle) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.s S100, 0x0(%1)"
        "vcst.s S101, VFPU_2_PI"
        "vmul.s S100, S100, S101"
        "vidt.q C000"    // [1,  0, 0, 0]
        "vrot.q C010, S100, [0,  C, S, 0]"
        "vrot.q C020, S100, [0, -S, C, 0]"
        "vidt.q C030"    // [0,  0, 0, 1]
        "lv.q C100, 0x0(%0)"
        "lv.q C110, 0x10(%0)"
        "lv.q C120, 0x20(%0)"
        "vmmul.t E200, E100, E000"
        "lv.q C230, 0x30(%0)"
        "vcst.t R203, 0"
        "sv.q C200, 0x0(%0)"
        "sv.q C210, 0x10(%0)"
        "sv.q C220, 0x20(%0)"
        "sv.q C230, 0x30(%0)"
        : "=m" (*m)
        : "m" (angle)
    );
#else
    // TODO
    vmidt_q(m);
#endif
}

inline void rotateY(ScePspFMatrix4 *m, float angle) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.s S100, 0x0(%1)"
        "vcst.s S101, VFPU_2_PI"
        "vmul.s S100, S100, S101"
        "vrot.q C000, S100, [C, 0, -S, 0]"
        "vidt.q C010"    // [0, 1,  0, 0]
        "vrot.q C020, S100, [S, 0,  C, 0]"
        "vidt.q C030"    // [0, 0,  0, 1]
        "lv.q C100, 0x0(%0)"
        "lv.q C110, 0x10(%0)"
        "lv.q C120, 0x20(%0)"
        "vmmul.t E200, E100, E000"
        "lv.q C230, 0x30(%0)"
        "vcst.t R203, 0"
        "sv.q C200, 0x0(%0)"
        "sv.q C210, 0x10(%0)"
        "sv.q C220, 0x20(%0)"
        "sv.q C230, 0x30(%0)"
        : "=m" (*m)
        : "m" (angle)
    );
#else
    // TODO
    vmidt_q(m);
#endif
}

inline void rotateZ(ScePspFMatrix4 *m, float angle) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.s S100, 0x0(%1)"
        "vcst.s S101, VFPU_2_PI"
        "vmul.s S100, S100, S101"
        "vrot.q C000, S100, [ C, S, 0, 0]"
        "vrot.q C010, S100, [-S, C, 0, 0]"
        "vidt.q C020"    // [ 0, 0, 1, 0]
        "vidt.q C030"    // [ 0, 0, 0, 1]
        "lv.q C100, 0x0(%0)"
        "lv.q C110, 0x10(%0)"
        "lv.q C120, 0x20(%0)"
        "vmmul.t E200, E100, E000"
        "lv.q C230, 0x30(%0)"
        "vcst.t R203, 0"
        "sv.q C200, 0x0(%0)"
        "sv.q C210, 0x10(%0)"
        "sv.q C220, 0x20(%0)"
        "sv.q C230, 0x30(%0)"
        : "=m" (*m)
        : "m" (angle)
    );
#else
    // TODO
    vmidt_q(m);
#endif
}

inline void normalize(ScePspFVector4 *out, ScePspFVector4 *v) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C000, %1"
        "vdot.q S010, C000, C000"
        "vzero.s S011"
        "vcmp.s EZ, S010, S010"
        "nop"
        "vrsq.s S010, S010"
        "vcmovt.s S010, S011, 0"     // S010 <- v^2 == 0 ? 0 : 1/sqrt(v^2)
        "vpfxd 0xFF"                 // clamp to interval -1.0 to +1.0
        "vscl.q C000, C000, S010"
        "sv.q C000, %0"
        : "=m"(*out)
        : "m"(*v)
    );
#else
    float f = v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
    if (f == 0.0f) {
        out->x = out->y = out->z = out->w = 0.0f;
    }
    f = 1.0f / sqrt(f);
    out->x = v->x * f;
    out->y = v->y * f;
    out->z = v->z * f;
    out->w = v->w * f;
#endif
}

inline float vsqrt_s(float x) {
    float result;
#if defined(__MWERKS__)
    __asm__ (
        "lv.s S000, %1"
        "vsqrt.s S000, S000"
        "sv.s S000, %0"
        : "=m"(result)
        : "m"(x)
    );
#else
    result = sqrt(x);
#endif
    return result;
}

inline float vsin_s(float radians) {
    float result;
#if defined(__MWERKS__)
    __asm__ (
        "lv.s S000, %1"
        "vcst.s S001, VFPU_2_PI"
        "vmul.s S000, S000, S001"
        "vsin.s S010, S000"
        "sv.s S010, %0"
        : "=m"(result)
        : "m"(radians)
    );
#else
    result = sinf(x);
#endif
    return result;
}

inline void scaleMatrix(ScePspFMatrix4 *out, float x, float y, float z) {
    vmidt_q(out);
    out->x.x = x;
    out->y.y = y;
    out->z.z = z;
}

inline void rotateXYZ(ScePspFMatrix4 *out, ScePspFVector3 * angle) {
    float z = angle->z;
    float y = angle->y;
    float x = angle->x;
    rotateX(out, x);
    rotateY(out, y);
    rotateZ(out, z);
}

inline void rotateZXY(ScePspFMatrix4 *out, ScePspFVector3 * angle) {
    float z = angle->z;
    float y = angle->y;
    float x = angle->x;
    rotateZ(out, z);
    rotateX(out, x);
    rotateY(out, y);
}

#ifdef __cplusplus
}
#endif
