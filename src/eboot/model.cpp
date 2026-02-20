#include "common.h"

#include "vfpu.h"

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088615DC);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088615FC);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08861674);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_0886168C);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08861714);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08861C74);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08861D74);

static int log2table[257] = {
    [0 ... 256] =  0xff,
    [1] = 0,
    [2] = 1,
    [4] = 2,
    [8] = 3,
    [16] = 4,
    [32] = 5,
    [64] = 6,
    [128] = 7,
    [256] = 8,
};

void world_matrix (
    ScePspFVector3 *scale,
    ScePspFVector3 *angle,
    ScePspFVector3 *position,
    int mode,
    ScePspFMatrix4 *out)

{
    ScePspFVector4 pos;
    pos.x = position->x;
    pos.y = position->y;
    pos.z = position->z;

    scaleMatrix(out, scale->x, scale->y, scale->z);

    float x, y, z, w;
    switch (mode & 0xffff) {
    case 6:
        rotateXYZ(out, angle);
        break;
    case 7:
        rotateZXY(out, angle);
        break;
    case 4:
        rotateX(out, angle->x);
        rotateY(out, angle->y);
        break;
    case 5:
        rotateZ(out, angle->z);
        rotateY(out, angle->y);
        break;
    case 1:
        rotateX(out, angle->x);
        break;
    case 2:
        rotateY(out, angle->y);
        break;
    case 3:
        rotateZ(out, angle->z);
        break;
    default:
        break;
    }
    out->w.x = pos.x;
    out->w.y = pos.y;
    out->w.z = pos.z;
}

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_0886234C);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088623D4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862508);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862510);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088626C8);

void func_eboot_088626D4(void) {
}

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088626DC);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862708);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862714);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862724);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862740);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862760);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088627A4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088627AC);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088627F8);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088629D4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862BB0);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862D2C);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862DA4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862DF8);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862E4C);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862E5C);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08862FD8);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088630C8);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863190);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088632D0);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088633C4);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863644);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863660);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_088637BC);

INCLUDE_ASM("asm/eboot/nonmatchings/model", func_eboot_08863B68);
}
