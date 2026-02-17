#include "common.h"

struct bar {
    ScePspFMatrix4 f0x0;
    ScePspFMatrix3 f0x40;
    u16 f0x64[14];
};

struct foo {
    void *vtable;
    ScePspFVector3 f0x4;
    ScePspFMatrix4 f0x10;
    ScePspFMatrix4 f0x50;
    ScePspFMatrix4 f0x90;
    ScePspFMatrix4 f0xd0;
    u16 f0x110;
    u16 f0x112;
    u16 key;
    u16 f0x116;
    float f0x118;
    float f0x11c;
    ScePspFMatrix3 f0x120;
    foo *parent;
    foo *sibling;
    foo *child;
    bar f0x150;
    bar f0x1d0;
};

inline void vsub_q(ScePspFVector4 *out, ScePspFVector4 *a, ScePspFVector4 *b) {
#if defined(__MWERKS__)
    __asm__ (
        "lv.q C000, %1"
        "lv.q C010, %2"
        "vsub.q C000, C000, C010"
        "sv.q C000, %0"
        : "=m"(*out)
        : "m"(*a), "m"(*b)
    );
#else
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;
    out->w = a->w - b->w;
#endif
}

extern "C" {

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885f840);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885f8e0);

foo *z_un_0885f920(foo *this_, foo *arg1, u32 arg2) {
    foo *temp = arg1;
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

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885f998);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885fa04);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885fb4c);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885fbac);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885fbec);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885fc94);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885fe3c);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_0885ff54);

void z_un_088633c4(ScePspFMatrix4*, ScePspFMatrix3*, ScePspFMatrix3*, ScePspFMatrix4*, u16*, float);

void z_un_0885ffb4(foo *this_, ScePspFVector4 *arg1, s32 arg2, float arg3, float arg4) {
    struct {
        ScePspFMatrix4 b;
        ScePspFMatrix4 a;
        void *pad[2];
        ScePspFMatrix3 c;
    } s;
    ScePspFMatrix3 *new_var3;
    ScePspFVector4 *new_var, *new_var2;
    bar *pafVar2;

    if (arg2 == 0) {
        pafVar2 = &this_->f0x150;
    } else {
        pafVar2 = &this_->f0x1d0;
    }
    new_var3 = &this_->f0x120;
    z_un_088633c4(&pafVar2->f0x0, new_var3, &s.c, &s.a, &pafVar2->f0x64[4], arg3);
    new_var3 = &this_->f0x120;
    z_un_088633c4(&pafVar2->f0x0, new_var3, &s.c, &s.b, &pafVar2->f0x64[4], arg4);

    new_var2 = &s.b.w;
    new_var = &s.a.w;
    vsub_q(arg1, new_var2, new_var);

    arg1->w = 0.0f;
}

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_08860054);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_08860254);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_08860320);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_08860534);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_08860640);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_088606c8);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_088606d8);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_08860700);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_08860708);

INCLUDE_ASM("asm/eboot/nonmatchings/bone", z_un_08860750);

void z_un_08860758() {}

}