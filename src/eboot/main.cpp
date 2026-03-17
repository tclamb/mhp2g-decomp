#include "common.h"

#include "data_loader.hpp"
#include "game.hpp"
#include "singleton.hpp"
#include "ge_manager.hpp"
#include "lighting_manager.hpp"
#include "cache.hpp"
#include "tagged_cache.hpp"

data_loader *singleton<data_loader>::INSTANCE;

void *D_eboot_089C7508; // singleton
u8 D_eboot_089C7510[0x90000] __attribute__((aligned(16)));
u8 D_eboot_08A57510[0x24];
u8 D_eboot_08A57534[0x67DC];

ge_manager *singleton<ge_manager>::INSTANCE;
u8 D_eboot_08A5DD14;
volatile bool GE_END_REACHED;
u32 D_eboot_08A5DD18;
u32 D_eboot_08A5DD1C;

void *D_eboot_08A5DD20; // singleton

void *D_eboot_08A5DD24; // singleton

lighting_manager *singleton<lighting_manager>::INSTANCE;

void *D_eboot_08A5DD2C; // singleton

void *D_eboot_08A5DD30; // singleton

void *D_eboot_08A5DD34; // singleton
u32 D_eboot_08A5DD38;
u32 D_eboot_08A5DD3C;
u32 D_eboot_08A5DD40;
u32 D_eboot_08A5DD44;
u16 D_eboot_08A5DD48;
u16 D_eboot_08A5DD4A;

void *D_eboot_08A5DD4C; // singleton
u8 D_eboot_08A5DD50[0xC];
u8 D_eboot_08A5DD5C;
u8 D_eboot_08A5DD5D;
u8 D_eboot_08A5DD5E;
u8 D_eboot_08A5DD5F;
u32 D_eboot_08A5DD60[3];
u32 D_eboot_08A5DD6C[5];
void *D_eboot_08A5DD80;

void *D_eboot_08A5DD84; // singleton
u8 D_eboot_08A5DD88[0x88];
u8 D_eboot_08A5DE10[0x44];

void *D_eboot_08A5DE54; // singleton
u8 D_eboot_08A5DE58;

void *D_eboot_08A5DE5C; // singleton
void *D_eboot_08A5DE60;
void *D_eboot_08A5DE64;
ScePspFMatrix4 D_eboot_08A5DE70[0x10];

game *game::instance;
SceUID D_eboot_08A5E274;

void *D_eboot_08A5E278; // singleton
u32 D_eboot_08A5E27C[3];
cache D_eboot_08A5E288;
u8 D_eboot_08A5E2B0[0x28000] __attribute__((aligned(16)));
u32 D_eboot_08A862B0;
u32 D_eboot_08A862B4;

void *D_eboot_08A862B8; // singleton
u32 D_eboot_08A862C0[16] __attribute__((aligned(16)));
u32 D_eboot_08A86300[57 * 16] __attribute__((aligned(16)));
u32 D_eboot_08A87140;
u32 D_eboot_08A87144;

u8 D_eboot_08A87148[0x70];
u8 D_eboot_08A871B8[0x10];
u8 D_eboot_08A871C8[0xE00];
u8 D_eboot_08A87FC8[0x30];
u8 D_eboot_08A87FF8[0x1D0];
u8 D_eboot_08A881C8[0x200];
u8 D_eboot_08A883C8[0x4];
u8 D_eboot_08A883CC[0x8];
u8 D_eboot_08A883D4[0x4];
u8 D_eboot_08A883D8[0x4];
u8 D_eboot_08A883DC[0x4];
u8 D_eboot_08A883E0[0xC];
u8 D_eboot_08A883EC[0x14];
u8 D_eboot_08A88400[0xF7EE80];

tagged_cache *singleton<tagged_cache>::INSTANCE;

u8 D_eboot_09A07284[0x4];
u8 D_eboot_09A07288[0x4];
u8 D_eboot_09A0728C[0x4];
u8 D_eboot_09A07290[0x4];
u8 D_eboot_09A07294[0x4];
u8 D_eboot_09A07298[0x4];
u8 D_eboot_09A0729C[0x4];

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889ACA0);

data_loader::~data_loader() {
    // empty
}

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889AD5C);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B1D4);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B1E4);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B1F4);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B204);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B214);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B224);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B234);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B248);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B25C);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B26C);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B280);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B290);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B2A0);

data_loader::data_loader() {
    // empty
}

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B32C);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B33C);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B394);

INCLUDE_ASM("asm/eboot/nonmatchings/main", func_eboot_0889B3A4);
