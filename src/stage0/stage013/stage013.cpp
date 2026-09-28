#include "common.h"
#include "stage_base.hpp"
#include "stage_manager.hpp"

extern u8 D_stage013_09D5EB90[0x470];
extern "C" void func_game_task_09AEDEB8(void *, int);

struct Stage013 : StageBase {
    Stage013();
    virtual ~Stage013();

    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual stage_draw_commands *vtable_0x48();
    virtual void vtable_0x50();
    virtual void vtable_0x98(pmo *, void *);
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    static void operator delete(void *);

    u8 unknown_0x460[0x10];
};

// Unknown layouts retain their exact 32-bit words from stage013.ovl.

u32 D_stage013_09D5E600[2] = {
    0x00000000, 0x00000080
};

u32 D_stage013_09D5E608[12] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x4639F000, 0x463B8000, 0x00020001
};

u32 D_stage013_09D5E638[24] = {
    0x3F800000, 0x3F800000, 0x3F800000, 0x00000000,
    0x3E99999A, 0x3E99999A, 0x3E99999A, 0x00000000,
    0x3DCCCCCD, 0x3DCCCCCD, 0x3DCCCCCD, 0x00000000,
    0x3DCCCCCD, 0x3DCCCCCD, 0x3DCCCCCD, 0x00000000,
    0x3DCCCCCD, 0x3DCCCCCD, 0x3DCCCCCD, 0x00000000,
    0x3DCCCCCD, 0x3DCCCCCD, 0x3DCCCCCD, 0x00000000
};

u32 D_stage013_09D5E698[30] = {
    0x00010000, 0x45983000, 0x00000000, 0x45C3A000,
    0x43C80000, 0x00070000, 0x000D0000, 0x459D7000,
    0x00000000, 0x45A4D000, 0x42C80000, 0x00000000,
    0x000B0000, 0x45910000, 0x41C80000, 0x45A9B000,
    0x43160000, 0x0000E001, 0x000C0000, 0x45AA5000,
    0x00000000, 0x45B2E800, 0x43160000, 0x0000C000,
    0x00070000, 0x45A9A000, 0x00000000, 0x45ABB800,
    0x42A00000, 0x00000000
};

u32 D_stage013_09D5E710[32] = {
    0x00000000, 0x00000003, 0x00000023, 0x00000017,
    0x45ABAD9A, 0x40000000, 0x45B16C00, 0x00000000,
    0x00000000, 0x00000003, 0x00000023, 0x00000017,
    0x45ABEC00, 0x40000000, 0x45A15400, 0x00000000,
    0x00000000, 0x00000003, 0x00000038, 0x00000002,
    0x4593A800, 0x00000000, 0x459F6000, 0x00000000,
    0x00000000, 0x00000003, 0x00000024, 0x00000016,
    0x45A67000, 0x00000000, 0x459FD000, 0x00000000
};

stage_definitions D_stage013_09D5E790 = {
    0, 0, 0,
    (u8 *)D_stage013_09D5E600,
    (u16 *)D_stage013_09D5E698,
    0xFFFF,
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    -1,
    (stage_definitions_0x28_t *)D_stage013_09D5E608,
    (stage_definitions_0x2C_t *)D_stage013_09D5E638,
    NULL,
    (stage_sound *)D_stage013_09D5E710,
    NULL,
    0, 0, 5, 4, 0, 0, 0
};

u32 padding_09D5E7D4 = 0;

u32 D_stage013_09D5E7D8[14] = {
    0x00800601, 0x00000000, 0x01800001, 0x00000000,
    0x02800001, 0x00000000, 0x03800001, 0x00000000,
    0x04800001, 0x00000000, 0x05800001, 0x00000000,
    0x06000001, 0x00000000
};

u32 D_stage013_09D5E810[2] = {
    0x00000100, 0x00000000
};

stage_draw_command D_stage013_09D5E818[3] = {
    {1, 0, 0x80, 0x19},
    {0x13, 0, 0x80, 0, D_stage013_09D5E810},
    {0x13, 0, 0x80, 0, (u8 *)D_stage013_09D5E810 + 1}
};

stage_draw_commands D_stage013_09D5E830 = {
    7, 3, (stage_draw_command *)D_stage013_09D5E7D8, D_stage013_09D5E818
};

u32 padding_09D5E83C = 0;

u32 D_stage013_09D5E840[20] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x3F800000, 0x3F800000, 0x3F800000, 0x00000000,
    0x00000800, 0x00000014, 0x00000000, 0x00000000
};

u32 D_stage013_09D5E890[4] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

u32 D_stage013_09D5E8A0[4] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

u32 D_stage013_09D5E8B0[4] = {
    0x45941800, 0x42180000, 0x45A47800, 0x00000000
};

u32 D_stage013_09D5E8C0[4] = {
    0x45A6FC00, 0x43468000, 0x459E4400, 0x00000000
};

u32 D_stage013_09D5E8D0[12] = {
    0x00150001, 0x00000000, 0x00000000, 0x00000000,
    0x3F800000, 0x3F800000, 0x3F800000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

u32 D_stage013_09D5E900[12] = {
    0x00160002, 0x00000000, 0x00000000, 0x00000000,
    0x3F800000, 0x3F800000, 0x3F800000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

u32 D_stage013_09D5E930[40] = {
    0x42480000, 0x00000000, 0x00000000, 0x00000000,
    0x4593F000, 0x00000000, 0x459CE800, 0x00000000,
    0x425C0000, 0x00000000, 0x00000000, 0x00000000,
    0x459D5800, 0x00000000, 0x45AA2800, 0x00000000,
    0x42480000, 0x00000000, 0x00000000, 0x00000000,
    0x458FF000, 0x00000000, 0x45AD9000, 0x00000000,
    0x42480000, 0x00000000, 0x00000000, 0x00000000,
    0x45907800, 0x00000000, 0x45AAF000, 0x00000000,
    0x42480000, 0x00000000, 0x00000000, 0x00000000,
    0x45931800, 0x00000000, 0x45A77800, 0x00000000
};

u32 D_stage013_09D5E9D0[36] = {
    0xFFE8FF00, 0x0057128B, 0x00001337, 0x3F4CCCCD,
    0x3F4CCCCD, 0x3F4CCCCD, 0xFFE20001, 0x0057130B,
    0x00001337, 0x3F800000, 0x3F800000, 0x3F800000,
    0x00000101, 0x0057135C, 0x00001337, 0x3F800000,
    0x3F800000, 0x3F800000, 0x00000001, 0x0011130C,
    0x0000135D, 0x3F800000, 0x3F800000, 0x3F800000,
    0x00000101, 0x00111361, 0x0000135D, 0x3F800000,
    0x3F800000, 0x3F800000, 0x00000301, 0x00111412,
    0x0000135D, 0x3F800000, 0x3F800000, 0x3F800000
};

u32 D_stage013_09D5EA60[4] = {
    0x45947000, 0x42D20000, 0x4599B000, 0x00000000
};

u32 D_stage013_09D5EA70[36] = {
    0x00000000, 0xBF060A92, 0x41200000, 0x3E32B8C3,
    0x41400000, 0xBF060A92, 0x41500000, 0x3E32B8C3,
    0x41600000, 0x00000000, 0x41800000, 0x3E32B8C3,
    0x41900000, 0xBF060A92, 0x43000000, 0xBF060A92,
    0xBF800000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

Stage013::Stage013() {
}

Stage013::~Stage013() {
}

// Complex prop selection remains in assembly.
INCLUDE_ASM("asm/stage0/stage013/nonmatchings/stage013", vtable_0x24__8Stage013Fv);

stage_definitions *Stage013::definitions() {
    return &D_stage013_09D5E790;
}

void Stage013::operator delete(void *) {
}

stage_draw_commands *Stage013::vtable_0x48() {
    return &D_stage013_09D5E830;
}

#if defined(BUILD_NONMATCHING)
void Stage013::vtable_0x50() {
    int remainder = unknown_0x1C0 % 20;
    if (remainder == 4 || remainder == 2 || remainder == 0) {
        func_game_task_09AEDEB8(D_stage013_09D5EA60, 1);
    }
    StageBase::vtable_0x50();
}
#else
INCLUDE_ASM("asm/stage0/stage013/nonmatchings/stage013", vtable_0x50__8Stage013Fv);
#endif

// Draw callback uses VFPU vector/matrix instructions.
INCLUDE_ASM("asm/stage0/stage013/nonmatchings/stage013", vtable_0x98__8Stage013FP3pmoPv);

bool Stage013::vtable_0xA0() {
    return true;
}

bool Stage013::vtable_0xA4() {
    return false;
}

bool Stage013::vtable_0xA8() {
    return false;
}

int Stage013::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *Stage013::vtable_0xB0() {
    return definitions()->sounds;
}

u8 Stage013::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *Stage013::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
