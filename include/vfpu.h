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


inline float atan2f_s(float y, float x) {
    float result;
#if defined (__MWERKS__)
    // implementation using asin and the identity:
    //   atan(y/x) == asin((y/x) / sqrt(1 + (y/x)^2))
    //
    // per the pspdev accuracy tests, vasin.s must be
    // restricted to the interval [-0.5, 0.5] for accuracy,
    // forcing the handlelargey branch reflecting [0.5, 1.0] onto [0.5, 0.0]
    // (recall that the VFPU scales angles by 2 / PI)
    __asm__ (
        ".set push"
        ".set noreorder"
        "lv.s       S000, %1"
        "lv.s       S001, %2"
        "vcmp.p     ES, C000, C000"
        "bvt        4, any_special" // !isfinite(x) || !isfinite(y)
        "vzero.s    S002"
        "vcmp.s     GE, S001, S002"
        "vcst.s     S010, VFPU_PI"
        "vcmovt.s   S010, S002, 0"
        "vcmp.s     EZ, S000, S000"
        "bvt        0, done" // y == 0
        "vcst.s     S002, VFPU_PI_2"
        "vrcp.s     S003, S001"
        "vmul.s     S003, S000, S003"
        "vcmp.s     EN, S003, S003"
        "bvt        0, done" // isnan(y / x)
        "vmov.s     S010, S003"
        "vcmp.s     ES, S003, S003"
        "bvtl       0, fixquadrant" // isinf(y / x)
          "vsgn.s   S010, S000"
        "vmul.s     S011,S003,S003"
        "vcmp.s     ES,S011,S011"
        "bvt        0, handleinf2" // isinf((y / x) ** 2)
        "vone.s     S003"
        "vpfxs      0x00FE4" // absolute value source
        "vcmp.s     LT, S010, S003"
        "vadd.s     S003,S011,S003"
        "bvfl       0, handlelargey"
          "vrcp.s   S003, S003"
        "vrsq.s     S003, S003"
        "vmul.s     S010, S010, S003"
        "b          fixquadrant" // return fixquadrant(asin( (y/x) / (1 + 2*(y/x) ** 2 + (y/x) ** 4) ))
          "vasin.s   S010,S010"
    any_special:
        "vcmp.p     EN, C000, C000"
        "vfim.s     S010, -1.0f"
        "bvt        4, done" // isnan(x) || isnan(y)
        "vrsq.s     S010, S010"
        "vcmp.p     EI, C000, C000" // isinf(x) && isinf(y)
        "bvt        5, bothinf"
        "nop"
        "vi2f.s     S000, S000, 0"
        "vpfxt      0xF0FF"
        "vadd.s     S000, S000, S000"
        "vsgn.s     S000, S000"
        "vcst.s     S002, VFPU_PI_2"
        "bvt        0, scaleresult"
        "vone.s     S010"
        "vzero.s    S003"
        "vcmp.s     GE, S001, S003"
        "vcmovt.s   S010, S003, 0"
        "b          scaleresult"
          "vcst.s     S002, VFPU_PI"
    bothinf:
        "vsgn.p     C000, C000"
        "vpfxs      0x0F0AA" // substitute 2 for source
        "vsub.s     S010, S010, S001" // result <- 2 - x
        "b          scaleresult"
          "vcst.s   S002, VFPU_PI_4"
    handleinf2:
        "vsgn.s     S000, S000"
        "vcst.s     S002, VFPU_PI_2"
        "bvt        0, scaleresult"
        "vone.s     S010"
        "vsgn.s     S011, S001"
        "b          scaleresult"
          "vsub.s     S010, S010, S011"
    handlelargey:
        "vmul.s     S003, S011, S003"
        "vpfxd      0x055" // clamp destination to [0, 1]
        "vocp.s     S003, S003" // reflect across y = x
        "vsqrt.s    S003, S003"
        "vasin.s    S003, S003"
        "vocp.s     S010, S003" // unreflect result
    fixquadrant:
        "vpfxd      0x004" // clamp y destination to [0, 1]
        "vpfxs      0x200E4" // negate y source
        "vsgn.p     C000, C000"
        "vcmp.s     EZ, S001, S001"
        "bvt        0, scaleresult" // skip if positive x
        "nop"
        "vpfxs      0x0F0AA" // constant 2 source
        "vpfxt      0x00FE4" // absolute value target
        "vsub.s     S010, S010, S010" // v = 2 - abs(v)
    scaleresult:
        "vpfxs      0x00FE4" // absolute value source
        "vmul.s     S010, S010, S000" // correct sign
        "vmul.s     S010, S010, S002" // scale to radians
    done:
        "sv.s S010, %0"
        ".set pop"
        : "=m" (result)
        : "m" (y), "m" (x)
    );
#else
    result = atan2f(y, x);
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
