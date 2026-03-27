#include "obj_base.hpp"


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

u32 ptmf_eboot_089A30F4[3] = {
    0, 0x38, 0
};

ObjBase::~ObjBase() {

}

void *ObjBase::operator new(u32, void *p) {
    return p;
}

void ObjBase::operator delete(void *) {
    // empty
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x10__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x14__7ObjBaseFv);

void ObjBase::vtable_0x18() {
    // empty
}

void ObjBase::vtable_0x1C() {
    // empty
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", draw__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_088646E8);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08864774);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_0886503C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x20__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_088652D4);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_088653C8);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865478);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865528);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_088655D8);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_088655F0);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865640);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x24__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x28__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_0886592C);

void ObjBase::vtable_0x3C() {
    // empty
}

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865A24);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865A5C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865A94);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865AD4);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865BA4);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865BC8);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865C24);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865C80);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865CB0);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865CE4);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865D4C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865D7C);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x30__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", vtable_0x34__7ObjBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/obj_base", func_eboot_08865DCC);

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
