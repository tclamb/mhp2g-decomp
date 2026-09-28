#include "cockpit.hpp"

#include "common.h"
#include "item_manager.hpp"
#include "psptypes.h"
#include "result_check.hpp"
#include "singleton.hpp"
#include "system_font.hpp"
#include "game_sys.hpp"
#include "game_task.hpp"
#include "ge.hpp"
#include "immediate_ge.hpp"
#include "ge_packet.hpp"
#include "vram_manager.hpp"
#include "resource_manager.hpp"
#include "system.hpp"
#include "vfpu.h"
#include "toast_notification.hpp"
#include "equip_manager.hpp"
#include "quest.hpp"

extern "C" int func_eboot_0881F830(Cockpit *);
extern "C" void func_eboot_0881CFB0(Cockpit *, void *, int, u8);
extern "C" void func_eboot_08848CE0();
extern "C" void func_eboot_0883BAB4(Cockpit *, void *);
extern "C" u32 D_lobby_task_09AFECD8;

template<> Cockpit *Singleton<Cockpit>::objectPtr;

// sprintf buffer
char D_eboot_089C6CC0[0x400];

extern u16 D_eboot_089A9834[11];
extern u16 D_eboot_089A9588[2];
extern u16 D_eboot_089A9604[2];

char D_eboot_08935AB8[4] = "%s";

CockpitTextbox D_eboot_08935ABC = { 0xA8, 0x28, 0xA0, 0x30, 14, 14, 5, 1, 0xAE, 0x31, 0x10 };
CockpitTextbox D_eboot_08935AD0 = { 0xA8, 0x58, 0xA0, 0x30, 14, 14, 5, 1, 0xAE, 0x61, 0x10 };
CockpitTextbox D_eboot_08935AE4 = { 0xC, 0x28, 0x9C, 0x60, 14, 14, 5, 1, 0x14, 0x30, 0x10 };
CockpitMenu D_eboot_08935AF8 = { 0xF8, 0xC8, 14, 14, 0, 1, 9, 2, &D_eboot_089A9604[0], 0x8000C040 };

char D_eboot_08935B0C[8] = "%d/%d";

char D_eboot_08935B14[4] = "%d";

char D_eboot_08935B18[8] = "  %s";

s16 D_eboot_08935B20[4] = {0, -32, -12, 0};

char D_eboot_08935B28[4] = "%2d";

struct SortTooltip {
    u16 left;
    u16 top;
    bool boxed;
};

SortTooltip D_eboot_08935B2C[5] = {
    {110, 72, false},
    {404, 208, true},
    {260, 176, true},
    {172, 42, false},
    {172, 80, false},
};

// dummy header text for item combination list
char *D_eboot_08935B4C[1] = {};

char D_eboot_08935B50[4] = "%dz";

char D_eboot_08935B54[12] = "%02d:%02d";

char D_eboot_08935B60[4] = "%4d";

struct EquipmentIconNameIndents {
    u16 iconLeft;
    u16 nameLeft;
};

EquipmentIconNameIndents D_eboot_08935B64[2] = {
    {248, 280},
    {276, 304},
};

// equipment type to skill tree column number
u8 D_eboot_08935B6C[7] = {
    5, 1, 2, 3, 4, 0, 0,
};

char D_eboot_08935B74[8] = "%+3d";

char D_eboot_08935B7C[8] = "=%3d";

// equipment comparison box column offsets
ScePspSVector2 D_eboot_08935B84[3] = {
    {12, 140},
    {8, 134},
    {8, 134},
};

char D_eboot_08935B90[4] = "%3d";

char D_eboot_08935B94[4] = "%+d";

char D_eboot_08935B98[8] = "%3+d";

char D_eboot_08935BA0[8] = "%s%2d";

char D_eboot_08935BA8[4] = "%d:";

char D_eboot_08935BAC[8] = "%3d%%";

char D_eboot_08935BB4[8] = "%s%3+d";

struct WeaponPropertyId {
    enum {
        FIRE,
        WATER,
        THUNDER,
        DRAGON,
        POISON,
        PARALYSIS,
        SLEEP,
        ICE,
        AFFINITY,
    };
};

u8 D_eboot_08935BBC[8] = {
        WeaponPropertyId::FIRE,
        WeaponPropertyId::WATER,
        WeaponPropertyId::THUNDER,
        WeaponPropertyId::DRAGON,
        WeaponPropertyId::ICE,
        WeaponPropertyId::POISON,
        WeaponPropertyId::PARALYSIS,
        WeaponPropertyId::SLEEP,
};

struct WeaponAttributeId {
    enum {
        FIRE,
        WATER,
        THUNDER,
        DRAGON,
        ICE,
        POISON,
        PARALYSIS,
        SLEEP,
        NONE = 0xFF,
    };
};

// bit indices
u8 D_eboot_08935BC4[9] = {
    WeaponAttributeId::SLEEP,
    WeaponAttributeId::POISON,
    WeaponAttributeId::PARALYSIS,
    WeaponAttributeId::NONE,
    WeaponAttributeId::FIRE,
    WeaponAttributeId::WATER,
    WeaponAttributeId::THUNDER,
    WeaponAttributeId::ICE,
    WeaponAttributeId::DRAGON,
};

// player money box position, size & text position
ScePspSVector2 D_eboot_08935BD0[3] = {
    {360, 4},
    {112, 24},
    {460, 10},
};

char D_eboot_08935BDC[8] = "%7dz";

CockpitMenu D_eboot_08935BE4 = {
    8, 28,
    14, 14,
    FontColor::WHITE, 2,
    22, 3, NULL,
    0x8000C040
};

CockpitTextbox D_eboot_08935BF8 = {
    8, 100,
    144, 32,
    14, 14,
    FontColor::WHITE, 0,
    16, 109, 16
};

// square button icon position for "show item details"
ScePspSVector2 D_eboot_08935C0C = {402, 243};

CockpitTextbox D_eboot_08935C10 = {
    216, 204,
    256, 64,
    14, 14,
    FontColor::WHITE, 0,
    224, 213, 16
};

char D_eboot_08935C24[20] = "~C%02d%s~C%02d%7dz";

CockpitTextbox D_eboot_08935C38 = {
    216, 204,
    256, 64,
    14, 14,
    FontColor::WHITE, 0,
    224, 213, 16
};

// vertical spacing for equipment comparison box
u16 D_eboot_08935C4C[2] = {0, 6};

// blademaster exclusive skill types
u8 D_eboot_08935C50[8] = {
    SkillType::SHARPNESS,
    SkillType::ARTISAN,
    SkillType::FENCING,
    SkillType::SWORD_SHARPENER,
    SkillType::GUARD,
    SkillType::SWORD_DRAW,
    SkillType::EDGEMASTER,
    SkillType::NONE,
};

// bowgun exclusive skill types
u8 D_eboot_08935C58[7] = {
    SkillType::RECOIL,
    SkillType::NORMAL_SHOT_ADD,
    SkillType::PIERCE_SHOT_ADD,
    SkillType::PELLET_SHOT_ADD,
    SkillType::CRAG_SHOT_ADD,
    SkillType::CLUSTER_SHOT_ADD,
    SkillType::NONE,
};

// bow exclusive skill types
u8 D_eboot_08935C60[6] = {
    SkillType::POISON_COATING_ADD,
    SkillType::PARALYSIS_COATING_ADD,
    SkillType::SLEEP_COATING_ADD,
    SkillType::POWER_COATING_ADD,
    SkillType::CLOSE_RANGE_COATING_ADD,
    SkillType::NONE,
};

// gunner exclusive skill types
u8 D_eboot_08935C68[19] = {
    SkillType::RELOAD,
    SkillType::RECOIL,
    SkillType::NORMAL_SHOT_UP,
    SkillType::PIERCE_SHOT_UP,
    SkillType::PELLET_SHOT_UP,
    SkillType::NORMAL_SHOT_ADD,
    SkillType::PIERCE_SHOT_ADD,
    SkillType::PELLET_SHOT_ADD,
    SkillType::CRAG_SHOT_ADD,
    SkillType::CLUSTER_SHOT_ADD,
    SkillType::PRECISION,
    SkillType::POISON_COATING_ADD,
    SkillType::PARALYSIS_COATING_ADD,
    SkillType::SLEEP_COATING_ADD,
    SkillType::POWER_COATING_ADD,
    SkillType::CLOSE_RANGE_COATING_ADD,
    SkillType::STEADY_HAND,
    SkillType::AUTO_RELOAD,
    SkillType::NONE,
};

CockpitMenu D_eboot_08935C7C = {
    232, 104,
    14, 14,
    FontColor::WHITE, 3,
    11, 3, NULL,
    0x8000C040,
};

CockpitMenu D_eboot_08935C90 = {
    242, 134,
    14, 14,
    FontColor::WHITE, 3,
    11, 3, NULL,
    0x8000C040,
};

char D_eboot_08935CA4[8] = "%02d";

struct MixDiscoveryAnimationParams {
    s16 widthIncrement;
    s16 alphaIncrement;
    s16 incrementFrames;
    s16 alphaDecrement;
    s16 height;
} __attribute__((aligned(0x4)));

MixDiscoveryAnimationParams D_eboot_08935CAC = {
    .widthIncrement=10,
    .alphaIncrement=0xC,
    .incrementFrames=10,
    .alphaDecrement=0x18,
    .height=20,
};

struct MixSuccessAnimationParams {
    s16 hazeSize;
    s16 alphaStart;
    s16 alphaDecrement;
    s16 starSizeIncrement;
    s16 unusedFramesRemaining;
    s16 starSizeDecrement;
};

MixSuccessAnimationParams D_eboot_08935CB8 = {
    .hazeSize = 48,
    .alphaStart = 0x80,
    .alphaDecrement = 0x10,
    .starSizeIncrement = 12,
    .unusedFramesRemaining = 4,
    .starSizeDecrement = 2,
};

struct MixFailureAnimationParams {
    s16 hazeSize;
    s16 hazeSizeIncrement;
    s16 alphaStart;
    s16 alphaDecrement;
    s16 dustSizeStart;
    s16 dustSizeIncrement;
    s16 dustBrightness;
    s16 hazeBrightness;
};

MixFailureAnimationParams D_eboot_08935CC4 = {
    .hazeSize = 32,
    .hazeSizeIncrement = 16,
    .alphaStart = 0xF0,
    .alphaDecrement = 0x1E,
    .dustSizeStart = 16,
    .dustSizeIncrement = 4,
    .dustBrightness = 0x40,
    .hazeBrightness = 0xF0,
};

// for calculating rotated texture vertices about their centers
ScePspFVector2 D_eboot_08935CD4[4] = {
    {-0.5, -0.5},
    {-0.5,  0.5},
    { 0.5, -0.5},
    { 0.5,  0.5},
};
// for calculated rotated texture vertices about an offset
ScePspFVector2 D_eboot_08935CF4[4] = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1},
};

ScePspSVector4 D_eboot_08935D14[3] = {
    {256, 126},
    {240, 120},
    {240, 126},
};

CockpitTextbox D_eboot_08935D2C = {
    8, 88,
    176, 176,
    14, 14,
    FontColor::LIGHT_YELLOW, 0,
    16, 96, 16
};

char D_eboot_08935D40[8] = "%s%s";

struct RotateTooltip {
    u16 iconLeft;
    u16 iconTop;
    s16 textLeft; // s16: read with lh for printfUtf8 (func_eboot_08832D88)
    s16 textTop;
};

RotateTooltip D_eboot_08935D48[2] = {
    {400, 63, 428, 64},
    {20, 203, 48, 204},
};

char D_eboot_08935D58[8] = "%2d/%2d";

extern CockpitTextbox D_eboot_089386A0;
extern s8 D_eboot_08938474[7];

#define SORT_DIALOG_OPEN 0x10
#define SORT_TOOLTIP_HIDDEN 0x20
#define LOWER_ITEM_DESCRIPTION 0x200

#define ITEM_ID_GARBAGE 0x9B

// xy + color
#define xycprintf(x, y, c,...) SystemFont::objectPtr->printfUtf8(x, y, c, __VA_ARGS__)
#define xyprintf(x, y, ...) SystemFont::objectPtr->printfUtf8(x, y, __VA_ARGS__)
// xy + control sequences?
#define xyprintfx(x, y, ...) SystemFont::objectPtr->printfUtf8x(x, y, __VA_ARGS__)
#define ITEM_NAME(itemId) GameSys::objectPtr->method_08851448(itemId)
#define UI_STRING(id) GameSys::objectPtr->method_0885143C(id)
#define INVENTORY_QTY(itemId) GameSys::objectPtr->bagQuantity(itemId)

// Immediate 2D packet (GePacket / GeStatePacket / geDrawPacket): see include/ge_packet.hpp.


// Cockpit texture atlas (12-byte entries): source rectangle in the texture plus
// the texture/resource ids passed to func_eboot_088315C8.
struct CockpitTexture {
    s16 u;
    s16 v;
    s16 width;
    s16 height;
    u16 texture;  // 0x8000 set: picture (texture & 0xFFF) of TMH resource ; else VRAM texture slot (texture & 0xFF)
    u16 resource;
};
extern CockpitTexture D_eboot_089366F0[];
// binds a cockpit texture (TEXMODE/TEXFORMAT/TBP0/TBW0/TSIZE0/CLUT load/TEXFLUSH) in the Cockpit render slot
extern "C" void func_eboot_088315C8(Cockpit *, int texture, int resource);
extern "C" void func_eboot_08831984(Cockpit *, CockpitTexture *);
extern "C" void func_eboot_08832CC0(Cockpit *, int linear);

// 8-bit r,g,b -> GE 5551 colour (alpha bit set), as written at every use site
#define COCKPIT_RGB5551(r, g, b) (((u32)((r) << 5) >> 8) | (((u32)((g) << 5) >> 8) << 5) | (((u32)((b) << 5) >> 8) << 10) | 0x8000)

Cockpit::Cockpit() {
    memset(this, 0, 0x1220);
    mix = NULL;
    memset(toasts, 0, 0x2D0);
    unknown_0x1198 = 0;
    unknown_0x119C = 0;
    unknown_0x119E = 0;
    flags_0x528 = 0;
    unknown_0x608 = 0;
}

Toast::~Toast() {
    // empty
}

Toast::Toast() {
    // empty
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", __dt__7CockpitFv);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088179A4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08817CE4);

void Cockpit::method_08818098() {
    InventoryEntry *inventory;
    if ((bool)(GameSys::objectPtr->flags_0x6AF14 & 0x200) == true) {
        inventory = GameSys::objectPtr->method_088567AC(0);
    } else {
        inventory = GameSys::objectPtr->userData.inventory;
    }
    u32 inventoryCursorColor = 0;
    u32 inventoryFlags = 5;
    if (((s32)mixState < 3) && !(flags_0x528 & SORT_DIALOG_OPEN)) {
        method_0882FE94(&D_eboot_08935ABC, UI_STRING(D_eboot_089A9834[0]), -1);
        if (mixState == 0) {
            xycprintf(D_eboot_08935ABC.textLeft + (D_eboot_08935ABC.textLineSpacing * 3) + 8, D_eboot_08935ABC.textTop, 3U, D_eboot_08935AB8, UI_STRING(D_eboot_089A9834[9]));
            if ((method_08828D80(inventoryCursorIndex, inventory[inventoryCursorIndex].itemId)) == 1) {
                method_08818B7C(D_eboot_08935ABC.textLeft, D_eboot_08935ABC.textTop + D_eboot_08935ABC.textLineSpacing, inventoryCursorIndex, inventory[inventoryCursorIndex].itemId, false);
            } else {
                method_08818B7C(D_eboot_08935ABC.textLeft, D_eboot_08935ABC.textTop + D_eboot_08935ABC.textLineSpacing, -1, inventory[inventoryCursorIndex].itemId, false);
            }
        } else {
            method_08818B7C(D_eboot_08935ABC.textLeft, D_eboot_08935ABC.textTop + D_eboot_08935ABC.textLineSpacing, mixFirstMaterialIndex, mixFirstMaterialItemId, true);
        }
        if ((s32)mixState > 0) {
            method_0882FE94(&D_eboot_08935AD0, UI_STRING(D_eboot_089A9834[1]), 0xFF);
            if (mixState == 1) {
                xycprintf(D_eboot_08935AD0.textLeft + (D_eboot_08935AD0.textLineSpacing * 3) + 8, D_eboot_08935AD0.textTop, 3U, D_eboot_08935AB8, UI_STRING(D_eboot_089A9834[9]));
                if (ItemManager::objectPtr->findMix(mixFirstMaterialItemId, inventory[inventoryCursorIndex].itemId, 0U, false, 0) != NULL) {
                    method_08818B7C(D_eboot_08935AD0.textLeft, D_eboot_08935AD0.textTop + D_eboot_08935AD0.textLineSpacing, inventoryCursorIndex, inventory[inventoryCursorIndex].itemId, false);
                } else {
                    method_08818B7C(D_eboot_08935AD0.textLeft, D_eboot_08935AD0.textTop + D_eboot_08935AD0.textLineSpacing, -1, inventory[inventoryCursorIndex].itemId, false);
                }
            } else {
                method_08818B7C(D_eboot_08935AD0.textLeft, D_eboot_08935AD0.textTop + D_eboot_08935AD0.textLineSpacing, mixSecondMaterialIndex, mixSecondMaterialItemId, true);
            }
        }
        if ((s32) mixState < 2) {
            inventoryCursorColor = 0x8000C040;
        }
        inventoryFlags = (mixState > 0 ? 8 : 4) | SORT_DIALOG_OPEN | 1;
    }
    s16 resultIndent = (u16) D_eboot_08935AE4.textLeft + (D_eboot_08935AE4.fontWidth * 5);
    switch (mixState) {
    case 0:
    case 1: {
        if ((flags_0x528 & SORT_DIALOG_OPEN)) {
            inventoryFlags |= SORT_TOOLTIP_HIDDEN | 1;
            inventoryCursorColor = 0;
            SystemFont::objectPtr->setFontSize(14, 14);
            xyprintfx(((u16) D_eboot_089386A0.textLeft + 0x62), ((u16) D_eboot_089386A0.textTop + (D_eboot_089386A0.textLineSpacing * 2)), D_eboot_08935AB8, UI_STRING(D_eboot_089A9588[yesNoIndex]));
        } else {
            if ((flags_0x528 & SORT_TOOLTIP_HIDDEN)) {
                inventoryFlags |= SORT_DIALOG_OPEN;
            } else {
                inventoryFlags |= 0x40 | SORT_DIALOG_OPEN;
            }
            inventoryCursorColor = 0x8000C040;
        }
    }
        break;
    case 2: {
        CockpitMenubox menubox;
        menubox.method_0882FC68(&D_eboot_08935AF8, 0, 0, 0);
        method_0882F7C4(&menubox, 0, mixConfirmMenuIndex, 0xFF, 0);
        u32 fontColor;
        if (mixIsValid == false) {
            fontColor = FontColor::WHITE;
        } else {
            fontColor = FontColor::DARK_GRAY;
        }
        char *str = UI_STRING(D_eboot_089A9834[6]);
        xycprintf(menubox.textLeft, menubox.textTop, fontColor, D_eboot_08935AB8, str);
    }
        // fallthrough
    case 3:
    case 4: {
        method_0882FE94(&D_eboot_08935AE4, UI_STRING(D_eboot_089A9834[2]), 0xFF);
        if (mixSuccessItemId > 0) {
            SystemFont::objectPtr->setFontColor(FontColor::WHITE);
            char *resultName = ITEM_NAME(mix->successItemId);
            xyprintf(D_eboot_08935AE4.textLeft, D_eboot_08935AE4.textTop + D_eboot_08935AE4.textLineSpacing, D_eboot_08935AB8, resultName);
            s8 mixRate = ItemManager::objectPtr->mixRate(mix, false);
            SystemFont::objectPtr->setFontColor(D_eboot_08938474[mixRateColorId(mixRate)]);
            xyprintf(resultIndent, D_eboot_08935AE4.textTop + (D_eboot_08935AE4.textLineSpacing * 2), UI_STRING(D_eboot_089A9834[4]), mixRate);
            SystemFont::objectPtr->setFontColor(FontColor::WHITE);
            xyprintf(resultIndent, D_eboot_08935AE4.textTop + (D_eboot_08935AE4.textLineSpacing * 3), D_eboot_08935AB8, method_0882595C(mix));
            for (int i = 0; i < 24; ++i) {
                if (mixSuccessItemId == inventory[i].itemId) {
                    s16 overflowStatus = method_088295D8(&inventory[i]);
                    if (overflowStatus >= 0) {
                        switch (overflowStatus) {
                        case 0:
                            SystemFont::objectPtr->setFontColor(FontColor::FLAMINGO);
                            break;
                        case 1:
                            SystemFont::objectPtr->setFontColor(FontColor::LIGHT_ORANGE);
                            break;
                        }
                    }
                    break;
                }
            }
            u16 inventoryQty = INVENTORY_QTY(mixSuccessItemId);
            xyprintf(resultIndent, D_eboot_08935AE4.textTop + (D_eboot_08935AE4.textLineSpacing * 4), D_eboot_08935B0C, inventoryQty, ITEM_DEFINITIONS[mixSuccessItemId].stackSize);
            SystemFont::objectPtr->setFontColor(FontColor::WHITE);
        } else {
            SystemFont::objectPtr->setFontColor(FontColor::DARK_GRAY);
            char *unknown = UI_STRING(D_eboot_089A9834[3]);
            xyprintf(D_eboot_08935AE4.textLeft, D_eboot_08935AE4.textTop + D_eboot_08935AE4.textLineSpacing, D_eboot_08935AB8, unknown);
            xyprintf(resultIndent, D_eboot_08935AE4.textTop + (D_eboot_08935AE4.textLineSpacing * 2), D_eboot_08935AB8, unknown);
            xyprintf(resultIndent, D_eboot_08935AE4.textTop + (D_eboot_08935AE4.textLineSpacing * 3), D_eboot_08935AB8, unknown);
            xyprintf(resultIndent, D_eboot_08935AE4.textTop + (D_eboot_08935AE4.textLineSpacing * 4), D_eboot_08935AB8, unknown);
        }
    }
        break;
    case 5:
    case 6: {
            u16 qty;
            u16 successFailureStringId;
            if (mixOutcomeItemId != ITEM_ID_GARBAGE) {
                qty = mixOutcomeQuantity;
                successFailureStringId = 7;
            } else {
                successFailureStringId = 8;
                qty = 1;
            }
            method_0882FE94(&D_eboot_08935AE4, UI_STRING(D_eboot_089A9834[successFailureStringId]), 0xFF);
            u16 inventoryQty = INVENTORY_QTY(mixOutcomeItemId);
            u16 stackSize = ITEM_DEFINITIONS[mixOutcomeItemId].stackSize;
            SystemFont::objectPtr->setFontColor(FontColor::WHITE);
            xyprintf(D_eboot_08935AE4.textLeft, D_eboot_08935AE4.textTop + D_eboot_08935AE4.textLineSpacing, D_eboot_08935AB8, ITEM_NAME(mixOutcomeItemId));
            xyprintf(resultIndent, D_eboot_08935AE4.textTop + D_eboot_08935AE4.textLineSpacing * 3, D_eboot_08935B14, qty);
            for (int i = 0; i < 24; ++i) {
                if (mixSuccessItemId == inventory[i].itemId) {
                    s16 overflowStatus = method_088295D8(&inventory[i]);
                    if (overflowStatus >= 0) {
                        switch (overflowStatus) {
                        case 0:
                            SystemFont::objectPtr->setFontColor(FontColor::FLAMINGO);
                            break;
                        case 1:
                            SystemFont::objectPtr->setFontColor(FontColor::LIGHT_ORANGE);
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                }
            }
            xyprintf(resultIndent, D_eboot_08935AE4.textTop + (D_eboot_08935AE4.textLineSpacing * 4), D_eboot_08935B0C, inventoryQty, stackSize);
            SystemFont::objectPtr->setFontColor(FontColor::WHITE);
            break;
        }
    }
    method_0881A280(inventoryCursorIndex, inventoryCursorColor, inventoryFlags, 0);
}

void Cockpit::method_08818B7C(s16 left, s16 top, u8 index, u16 itemId, bool locked) {
    if (itemId != 0xFFFF) {
        s8 color;
        if (locked == true) {
            color = FontColor::LIGHT_YELLOW;
        } else {
            u8 unlockedColor;
            if (index != 0xFF) {
                unlockedColor = FontColor::WHITE;
            } else {
                unlockedColor = FontColor::DARK_GRAY;
            }
            color = unlockedColor;
        }
        char *itemName = GameSys::objectPtr->method_08851448(itemId);
        SystemFont::objectPtr->printfUtf8(left, top, color, D_eboot_08935AB8, itemName);
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08818C20);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08818DB8);

extern "C" void func_eboot_0881B5C0(Cockpit *, Player *, u8);

// Forward the stored player and the menu selection byte at Cockpit+0x57F.
extern "C" void func_eboot_088193FC(Cockpit *this_) {
    func_eboot_0881B5C0(this_, this_->player, this_->pad_0x57E[1]);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08819408);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088194A4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08819774);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08819A54);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08819B6C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08819C08);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08819F30);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0881A280__7CockpitFUcUiUiUi);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881A3C4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881A7D8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881A8F0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881ADA4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881B23C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881B378);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881B3EC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881B4F4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881B5C0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881BEA0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881BF24);

extern "C" void func_eboot_0881C114(Cockpit *, Player *, s16, s16, s8 *);

// Forward the player and coordinates with six default text-color selectors.
extern "C" void func_eboot_0881C0E0(Cockpit *this_, Player *player, s16 x, s16 y) {
    s8 colors[6];
    colors[0] = colors[1] = colors[2] = colors[3] = colors[4] = colors[5] = 0;
    func_eboot_0881C114(this_, player, x, y, colors);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881C114);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881C30C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881C4BC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881CAD4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881CBEC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881CD1C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881CD94);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881CE38);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881CEEC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881CFB0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D074);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D12C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D1E0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D3D8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D498);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D588);

extern "C" void func_eboot_08827824(Cockpit *, Player *, Player *);

// Rebuild the stored player's entries without a comparison player, then close
// this selection. The menu dispatcher consumes the zero return value.
extern "C" int func_eboot_0881D61C(Cockpit *this_) {
    func_eboot_08827824(this_, this_->player, NULL);
    this_->pad_0x57E[1] = 0;
    this_->unknown_0x5E8 &= ~4;
    return 0;
}

// Clear the menu selection bytes at +0x57F/+0x582 and bit 2 of +0x5E8.
// The menu dispatcher consumes the zero return value as its next status.
extern "C" int func_eboot_0881D660(Cockpit *this_) {
    this_->pad_0x57E[1] = 0;
    this_->pad_0x57E[4] = 0;
    this_->unknown_0x5E8 &= ~4;
    return 0;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D680);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881E6B0);

// Initialize menu 2 and reset its selections. pad_0x5B9 actually starts at
// +0x5BB in the current layout; its index 0x1E is the byte at +0x5D9.
extern "C" int func_eboot_0881F44C(Cockpit *this_) {
    this_->pad_0x57E[0] = 0;
    this_->pad_0x57E[1] = 0;
    this_->pad_0x5B9[0] = 0;
    this_->mix = NULL;
    this_->pad_0x5B9[0x1E] = 0;
    this_->unknown_0x5E8 |= 4;
    this_->itemBoxMenuId = 2;
    this_->itemBoxMenuCursor = 0;
    return 0;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F480);

extern "C" int func_eboot_0881F64C(Cockpit *self, int value) {
    self->unknown_0x5E8 &= ~4U;
    if ((u8)func_eboot_0881F830(self) == 1) {
        func_eboot_0881CFB0(self, (u8 *)self + 0x57F, value,
                            *((u8 *)self + 0x581));
    }
    return value;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F6B4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F780);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F830);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F8B8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F9D8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088213E8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08821630);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08821828);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088218F4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08821C58);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08821D00);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08821FE8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882209C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822104);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822198);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822284);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822350);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088223CC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822448);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882251C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088225BC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088229F0);

struct CockpitRgb {
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
};
extern u16 D_eboot_089A997C;          // UI string id of the decoration-slots label
extern u8 D_eboot_089B2FB8[][3];      // per-equipment decoration slot kinds (3 slots)
extern CockpitRgb D_eboot_08938228[]; // decoration slot colours
extern "C" int func_eboot_0885CDB0(EquipManager *, int equipId);
extern "C" void func_eboot_0882C0A4(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b, int textureId);

// Decoration slots line of an equipment entry: the label at (x + 0x62, y), then three 12x16 slot
// sprites (texture 0x177) at x + 0x98 + 16 * i, y - 2, each tinted with its slot kind's colour.
extern "C" void func_eboot_08822A84(Cockpit *this_, int equipId, u16 x, u16 y) {
    SystemFont::objectPtr->printfUtf8(x + 0x62, y, 0, D_eboot_08935AB8, GameSys::objectPtr->method_0885143C(D_eboot_089A997C));
    x += 0x98;
    u8 *slots = D_eboot_089B2FB8[(u8)func_eboot_0885CDB0(EquipManager::objectPtr, equipId)];
    for (int i = 3; i > 0; i--) {
        func_eboot_0882C0A4(this_, x, y - 2, 12, 16, D_eboot_08938228[*slots].r, D_eboot_08938228[*slots].g, D_eboot_08938228[*slots].b, 0x177);
        slots++;
        x += 16;
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822BA0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822E10);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822F6C);

// Equipment UI predicate: player byte +0x4E9 is 6, or byte +0x5F2 is positive.
extern "C" bool func_eboot_08823214(Cockpit *this_, Player *player) {
    if (player->padding_0x480[0x69] == 6) {
        return true;
    }
    return player->padding_0x558[0x9A] >= 1;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882323C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08823444);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088234A8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088239CC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08823AF8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088241B8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08824254);

extern "C" void func_eboot_088243B0(Cockpit *this_);

// Toast layer: unless GameSys func_eboot_088566DC reports 1, switch to render slot 14 (font layer 5),
// emit the 2D render state, run func_eboot_088243B0 and update/draw every active toast (20 slots at
// Cockpit+0xE28). The toast being written out twice (no pointer local) gives the target's beqzl.
extern "C" void func_eboot_088242A8(Cockpit *this_) {
    if ((u8)func_eboot_088566DC(GameSys::objectPtr) == 1) {
        return;
    }
    this_->renderGroup = 14;
    SystemFont::objectPtr->setLayer(5);
    this_->method_08826600();
    func_eboot_088243B0(this_);
    for (int i = 0; i < 20; i++) {
        if (((ToastNotification *)&this_->toasts[i])->active) {
            func_eboot_0885A0E8((ToastNotification *)&this_->toasts[i]);
        }
    }
}

// Search the pending notification ring for the byte tag at slot+0x1B.
// Both the queued count and its read cursor are signed bytes.
extern "C" bool func_eboot_0882433C(Cockpit *this_, u8 tag) {
    int count = (s8)this_->pad_0x10F8[0];
    if (count == 0) {
        return false;
    }
    s16 i = 0;
    if (count > 0) {
        int cursor = (s8)this_->pad_0x10F8[1];
        do {
            s16 slot = cursor % 20;
            if (this_->toasts[slot].padding[0x1B] == tag) {
                return true;
            }
            ++i;
            ++cursor;
        } while (i < count);
    }
    return false;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088243B0);

// Active notifications plus the signed pending count at Cockpit+0x10F8.
extern "C" int func_eboot_088244DC(Cockpit *this_) {
    int count = 0;
    for (int i = 0; i < 20; ++i) {
        if (((ToastNotification *)&this_->toasts[i])->active) {
            ++count;
        }
    }
    return count + (s8)this_->pad_0x10F8[0];
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_08824514__7CockpitFbi);

// Initialize the result menu; preserve the item and confirmation cursors.
extern "C" void func_eboot_08824608(Cockpit *this_) {
    this_->resultMenuState = 0;
    this_->resultMenuCursor = 0;
    this_->resultDescriptionFlags = 0;
    this_->unknown_0x120F = 0;
    this_->resultFramesRemaining = 3600;
    this_->isOtomoAiruResults = 0;
    this_->itemBoxMenuId = 6;
    this_->itemBoxMenuCursor = 4;
    this_->unknown_0x5E8 |= 4;
}

extern "C" void func_eboot_08824644(Cockpit *this_) {
    func_eboot_08824608(this_);
    this_->isOtomoAiruResults = 1;
}

// SystemFont offsets
const u8 D_eboot_0892E138[6][2] = {
    {   0, 0x10},
    {0x70, 0x10},
    {0x70, 0x20},
    {0x70, 0x30},
    {   0, 0x20},
    {   0, 0x30},
};

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08824670);

extern u16 D_eboot_089A9590[8];
extern u16 D_eboot_089AA0F8[4];
extern u16 D_eboot_089B0584;
extern u16 D_eboot_089B0704[4];
extern u16 D_eboot_089B070C[4];

// draw send item to box
void Cockpit::method_0882536C(SceBool isLobby) {
    renderGroup = render_group::GROUP_10;
    SystemFont::objectPtr->setLayer(1);
    method_08826600();

    if (isLobby == false) {
        renderGroup = render_group::GROUP_9;
        SystemFont::objectPtr->setLayer(0);
        method_08826600();
        // draw quest freeze frame
        method_088259A0();
    }
    renderGroup = render_group::GROUP_10;
    SystemFont::objectPtr->setLayer(1);
    method_08826600();

    ScePspUnion32 iconPos;
    iconPos.s[0] = D_eboot_08935C0C.x;
    iconPos.s[1] = D_eboot_08935C0C.y;

    if (unknown_0x5E8 & 4) {
        if ((resultDescriptionFlags & 1) != false) {
            if (resultMenuState - 1 < 2U) {
                method_08826334(1, GameSys::objectPtr->itemBox[resultItemCursor].itemId + 0x18, -1);
            } else {
                if (resultMenuState != 6 && resultMenuState != 5) {
                    method_08826334(1, GameSys::objectPtr->method_088567AC(inventoryCursorIndex)->itemId + 0x18, -1);
                }
            }
        } else {
            // show menu option description window
            switch (resultMenuState) {
            case 1:
            case 3:
                method_0882FE94(&D_eboot_08935C10, UI_STRING(D_eboot_089A9590[5]), 0xFF);
                break;
            case 2:
                method_0882FE94(&D_eboot_08935C10, UI_STRING(D_eboot_089A9590[6]), 0xFF);
                break;
            case 4:
                method_0882DEE4(D_eboot_08935C10.boxLeft, D_eboot_08935C10.boxTop, D_eboot_08935C10.boxWidth, D_eboot_08935C10.boxHeight, 0xFF);
                SystemFont::objectPtr->setFontSize(14, 14);
                xyprintfx(224, 213, UI_STRING(D_eboot_089A9590[resultConfirmCursor]));
                break;
            case 0:
                if (isLobby) {
                    method_08826334(itemBoxMenuId, itemBoxMenuCursor, -1);
                }
                break;
            case 5:
            case 6:
                break;
            }
        }
    }

    switch (resultMenuState) {
        case 0:
            D_eboot_08935BE4.cursorColor = 0x8000C040;
            break;
        case 1:
            D_eboot_08935BE4.cursorColor = 0x80808080;
            method_08824514(false, isOtomoAiruResults);
            method_08825CB4(resultItemCursor, false, resultItemWouldOverflow);
            if (!(resultDescriptionFlags & 1)) {
                method_0882B078(iconPos.us[0], iconPos.us[1], CockpitTextureId::SQUARE_BUTTON);
            }
            break;
        case 2:
            D_eboot_08935BE4.cursorColor = 0x80808080;
            method_08824514(false, isOtomoAiruResults);
            method_08825CB4(resultItemCursor, false, -1);
            if (resultItemWouldOverflow >= 0) {
                SystemFont::objectPtr->setFontColor(FontColor::FLAMINGO);
                func_eboot_0887CDC8(456, 180, UI_STRING(D_eboot_089A9590[7]), 14);
            }
            if (!(resultDescriptionFlags & 1)) {
                method_0882B078(iconPos.us[0], iconPos.us[1], CockpitTextureId::SQUARE_BUTTON);
            }
            break;
        case 3:
            D_eboot_08935BE4.cursorColor = 0x80808080;
            method_08824514(false, isOtomoAiruResults);
            method_08825CB4(resultItemCursor, true, resultItemWouldOverflow);
            if (!(resultDescriptionFlags & 1)) {
                method_0882B078(iconPos.us[0], iconPos.us[1], CockpitTextureId::SQUARE_BUTTON);
            }
            break;
        case 4:
            D_eboot_08935BE4.cursorColor = 0x80808080;
            method_08824514(false, isOtomoAiruResults);
            method_08825CB4(-1, 0, 0);
            break;
        case 5:
        case 6:
        default:
            break;
    }

    if (isOtomoAiruResults) {
        D_eboot_08935BE4.optionStringIds = D_eboot_089B070C;
        D_eboot_08935BE4.optionMaxLength = 24;
        method_0882FBEC(&D_eboot_08935BE4, 0, resultMenuCursor, 0xFF, StringTableId::QUEST_1);
    } else {
        if (isLobby) {
            D_eboot_08935BE4.optionStringIds = D_eboot_089AA0F8;
            D_eboot_08935BE4.optionMaxLength = 22;
            method_0882FBEC(&D_eboot_08935BE4, 0, resultMenuCursor, 0xFF, StringTableId::MESSAGE_0);
        } else {
            D_eboot_08935BE4.optionStringIds = D_eboot_089B0704;
            D_eboot_08935BE4.optionMaxLength = 22;
            method_0882FBEC(&D_eboot_08935BE4, 0, resultMenuCursor, 0xFF, StringTableId::QUEST_1);
        }
    }

    char buffer[64];
    if (!isLobby) {
        sprintf(buffer, GameTask::objectPtr->method_09A5E4E0(D_eboot_089B0584), resultFramesRemaining / 30);
        method_0882FE94(&D_eboot_08935BF8, buffer, 0xFF);
    }

    if (resultMenuState == 3) {
        renderGroup = render_group::GROUP_11;
        SystemFont::objectPtr->setLayer(2);
        method_08826600();
        method_0881A280(inventoryCursorIndex, 0x8000C040, 0x10, 2);
    }
}

extern u16 D_eboot_089AA01C[7];

char *Cockpit::method_0882595C(MixDefinition *mix) {
    if (mix == NULL) {
        return NULL;
    }
    u16 stringId = D_eboot_089AA01C[mix->quantityId];
    return GameSys::objectPtr->method_0885143C(stringId);
}

// Quest-clear background: draws the frozen copy of the last frame
// (Cockpit::snapshot, 0x44000 bytes = 512 x 272 x 16-bit, filled by
// DrawManager::vram_transfer on quest clear) as one full-screen sprite,
// zoomed 4/3 around the screen centre and half-transparent.
void Cockpit::method_088259A0() {
    if (GameSys::objectPtr->snapshot_draw_0D == 0) {
        return;
    }
    u16 *snapshot = this->snapshot;
    u32 *list = Ge::objectPtr->write_head();
    list[0] = 0x22000000;                       // alpha test off
    list[1] = 0x21000001;                       // alpha blend on
    list[2] = 0xE0FFFFFF;                       // FIXA = white
    list[3] = 0xDF00003A;                       // blend: src*FIXA + dst*(1-srcA)
    list[4] = 0xC6000107;                       // tex filter min 7, mag linear
    list[5] = 0x23000000;                       // depth test off
    list[6] = 0xC9000000;                       // tex func modulate, RGB
    list[7] = 0x1E000001;                       // texture on
    list[8] = 0xC2000000;                       // tex mode: no swizzle, 1 level
    list[9] = 0xC3000001;                       // tex format 1 = ABGR5551
    list[10] = 0xA0000000 | (((u32)snapshot << 8) >> 8);                 // TBP0
    list[11] = (((u32)snapshot & 0xFF000000) >> 8) | 0xA8000200;         // TBW0 = 512
    list[12] = 0xB8000000 | (Ge::objectPtr->method_0885973C(0x110) << 8) // TSIZE0: 512 x 512
             | Ge::objectPtr->method_0885973C(0x200);
    list[13] = 0xCB000000;                      // tex flush
    list[14] = 0x10000000;                      // BASE/JUMP placeholders
    list[15] = 0x08000000;
    Ge::objectPtr->method_088595E8(list, 16, renderGroup);
    Ge::objectPtr->set_write_head(list + 16);

    // one through-mode sprite: tc16 + col8888 + pos16, 2 vertices (8 words)
    GePacket<8, 9> sprite;
    sprite.vertexWords = 8;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;            // texture on
    sprite.commands[2] = 0x50000001;            // gouraud
    sprite.commands[3] = 0x1280011E;            // vtype: through, tc16, col8888, pos16
    sprite.commands[6] = 0x04060002;            // PRIM sprites, 2 vertices
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;            // tex func modulate, RGBA
    sprite.vertices[0] = (34 << 16) | 60;       // uv (60, 34)
    sprite.vertices[1] = 0x80FFFFFF;            // white, alpha 0x80
    sprite.vertices[2] = (0 << 16) | 0;         // xy (0, 0)
    sprite.vertices[3] = 0;                     // z
    sprite.vertices[4] = (238 << 16) | 420;     // uv (420, 238)
    sprite.vertices[5] = 0x80FFFFFF;
    sprite.vertices[6] = (272 << 16) | 480;     // xy (480, 272)
    sprite.vertices[7] = 0;
    geDrawPacket(Ge::objectPtr, &sprite, renderGroup);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_08825CB4__7CockpitFUibi);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_08826334__7CockpitFUcUsSc);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826590);

// Render state for Cockpit 2D in slot renderGroup (12 words): depth test off, ZTST 0, alpha blend on with
// BLEND 0xDF000032 (src*srcA + dst*(1-srcA)), alpha test off, ATST 0xDBFF0003, TFUNC modulate RGBA,
// TMAP UV, TWRAP repeat, TFLT nearest.
void Cockpit::method_08826600() {
    u32 *list = Ge::objectPtr->write_head();
    list[0] = 0x23000000;
    list[1] = 0xDE000000;
    list[2] = 0x21000001;
    list[3] = 0xDF000032;
    list[4] = 0x22000000;
    list[5] = 0xDBFF0003;
    list[6] = 0xC9000100;
    list[7] = 0xC0000000;
    list[8] = 0xC7000000;
    list[9] = 0xC6000000;
    list[10] = 0x10000000;
    list[11] = 0x08000000;
    Ge::objectPtr->method_088595E8(list, 12, renderGroup);
    Ge::objectPtr->set_write_head(list + 12);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088266D0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826740);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882692C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088269F4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826B08);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826D4C);

// Reset the item-combination selection, its result and bit 2 of +0x528.
extern "C" void func_eboot_08826EE0(Cockpit *this_) {
    this_->mixState = 0;
    this_->inventoryCursorIndex = 0;
    this_->mixConfirmMenuIndex = 0;
    this_->mixSecondMaterialIndex = 0xFF;
    this_->mixFirstMaterialIndex = 0xFF;
    this_->mixSecondMaterialItemId = 0xFFFF;
    this_->mixFirstMaterialItemId = 0xFFFF;
    this_->mixSuccessItemId = -1;
    this_->mix = NULL;
    this_->flags_0x528 &= ~4;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826F24);

// The inventory lookup takes a byte index; this accessor returns the item id
// as a signed halfword, preserving the original lh and caller's seh.
extern "C" s16 func_eboot_08826F8C(Cockpit *this_, u8 index) {
    return (s16)GameSys::objectPtr->method_088567AC(index)->itemId;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826FB4);

extern "C" bool func_eboot_08827138(Cockpit *self, u16 *value) {
    u8 *raw = (u8 *)self;
    if (raw[0x52F] != 0) {
        return false;
    }
    s16 state = *(s16 *)(raw + 0x119E);
    if (state <= 0) {
        u16 next = *(u16 *)(raw + 0x119C) + 1;
        *(u16 *)(raw + 0x119C) = next;
        *value = next;
        *(s16 *)(raw + 0x119E) = 4;
        return true;
    }
    if (*value == *(u16 *)(raw + 0x119C)) {
        *(s16 *)(raw + 0x119E) = 4;
        return true;
    }
    return false;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827194);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827218);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", vtable_0xA0__9StageBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882774C);

extern "C" u8 func_eboot_088277D8(Cockpit *self, int divisor) {
    u8 *entry = (u8 *)self + 0x684;
    int count = 0;
    for (; count < 54; ++count, entry += 0x12) {
        if (entry[2] == 0) {
            break;
        }
    }
    if (count == 0) {
        return 1;
    }
    return (u8)(((count - 1) / divisor) + 1);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827824);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088279EC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827CD4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827E08);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827FA4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828034);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828170);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828290);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088288C8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828BC0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828CF4);

bool Cockpit::method_08828D80(int index, u16 itemId) {
    u8 i = index;
    if (i == 0xFF) {
        return false;
    }
    if (GameSys::objectPtr->method_088567AC(i)->quantity == 0) {
        return false;
    }
    if (itemId != GameSys::objectPtr->method_088567AC(i)->itemId) {
        return false;
    }
    return ItemManager::objectPtr->isMixIngredient(itemId, false, 0) != false;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828E20);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828F20);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828FD4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829588);

// Whether an inventory index is one of the two selected mix ingredients.
extern "C" bool func_eboot_088295B0(Cockpit *this_, u16 index) {
    if (index == this_->mixFirstMaterialIndex || index == this_->mixSecondMaterialIndex) {
        return true;
    }
    return false;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_088295D8__7CockpitFP14InventoryEntry);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088296A8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829740);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829850);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829948);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829C5C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829D10);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829EB8);

extern "C" void *func_eboot_08829F80() {
    if (((GameSys::objectPtr->flags_0x6AF14 & 1) != 0) == true) {
        return &D_lobby_task_09AFECD8;
    }
    return *(void **)((u8 *)Quest::objectPtr + 0x68);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829FC4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A0E4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A1F4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A508);

extern "C" void func_eboot_0882A5A0(Cockpit *this_, int type, s16 x, s16 y);

// Start a HUD effect of the requested type at either fixed anchor.
extern "C" void func_eboot_0882A588(Cockpit *this_, int type) {
    func_eboot_0882A5A0(this_, type, 48, 48);
}

extern "C" void func_eboot_0882A594(Cockpit *this_, int type) {
    func_eboot_0882A5A0(this_, type, 246, 104);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A5A0);

extern "C" void func_eboot_08832BC4(Cockpit *this_, int additive);
extern "C" void func_eboot_0882A928(Cockpit *this_, CockpitHudFx *fx);
extern "C" void func_eboot_0882AAD8(Cockpit *this_, CockpitHudFx *fx);
extern "C" void func_eboot_0882ACFC(Cockpit *this_, CockpitHudFx *fx);

// HUD effect layer: render slot 12 (font layer 3), 2D state, additive blend; draws every active
// CockpitHudFx by type, then restores the normal blend.
extern "C" void func_eboot_0882A804(Cockpit *this_) {
    this_->renderGroup = 12;
    SystemFont::objectPtr->setLayer(3);
    this_->method_08826600();
    func_eboot_08832BC4(this_, 1);
    for (int i = 0; i < 6; i++) {
        if (this_->hudFx[i].active == 1) {
            switch (this_->hudFx[i].type) {
            case 0:
                func_eboot_0882A928(this_, &this_->hudFx[i]);
                break;
            case 1:
                func_eboot_0882AAD8(this_, &this_->hudFx[i]);
                break;
            case 2:
                func_eboot_0882ACFC(this_, &this_->hudFx[i]);
                break;
            }
        }
    }
    func_eboot_08832BC4(this_, 0);
}

// Find the first inactive HUD effect slot. The caller initializes and activates
// the returned entry; a full array returns NULL without changing any slot.
extern "C" CockpitHudFx *func_eboot_0882A8E4(Cockpit *this_) {
    for (int i = 0; i < 6; ++i) {
        if (this_->hudFx[i].active == 0) {
            return &this_->hudFx[i];
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A928);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882AAD8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882ACFC);

// Only the local player (or a null query) may pass. The UI state must be zero
// and the signed halfword at +0x119E nonpositive. Its current field name is 119C.
extern "C" bool func_eboot_0882AF00(Cockpit *this_, Player *player) {
    if (player != NULL) {
        if ((bool)(player->pl_id == GameSys::objectPtr->player_id) == false) {
            return false;
        }
    }
    if (this_->unknown_0x52F) {
        return false;
    }
    return (s16)this_->unknown_0x119C <= 0;
}

extern "C" u8 *func_eboot_0883B450(Cockpit *, int);

// Classify the selected lobby entry: absent -> 3; byte +0xBF of 5 -> 1,
// zero -> 2, any other value -> 0.
extern "C" u8 func_eboot_0882AF54(Cockpit *this_, int index) {
    u8 *entry = func_eboot_0883B450(this_, index);
    if (entry == NULL) {
        return 3;
    }
    switch (entry[0xBF]) {
    case 0:
        return 2;
    case 5:
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882AF98);

extern "C" void func_eboot_0882B024(Cockpit *self, void *arg) {
    u8 *player = (u8 *)self->player;
    if ((player[0x1E0] != 0) == 1) {
        func_eboot_08848CE0();
    }
    func_eboot_0883BAB4(self, arg);
}

// Draws cockpit texture `textureId` (D_eboot_089366F0 atlas entry) 1:1 at (left, top):
// one through-mode sprite, UV = the atlas rectangle, colour 0xFFFF (opaque white).
void Cockpit::method_0882B078(u16 left, u16 top, int textureId) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this, tex->texture, tex->resource);
    GePacket<6, 9> sprite;
    sprite.vertexWords = 6;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x12800116;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    sprite.vertices[0] = (tex->v << 16) | tex->u;
    sprite.vertices[1] = (left << 16) | 0xFFFF;
    sprite.vertices[2] = top;
    sprite.vertices[3] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
    sprite.vertices[4] = ((left + tex->width) << 16) | 0xFFFF;
    sprite.vertices[5] = (u16)(top + tex->height);
    geDrawPacket(Ge::objectPtr, &sprite, renderGroup);
}

// Draws the texture bound by func_eboot_08831984 as a w x h sprite at (x, y), UV (0,0)-(w,h), opaque white.
extern "C" void func_eboot_0882B2B4(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, int textureId) {
    using namespace immediate_ge;
    func_eboot_08831984(this_, &D_eboot_089366F0[textureId]);
    u32 *start = Ge::objectPtr->write_head();
    u32 *out = start;
    *out++ = 0;
    *out++ = (x << 16) | 0xFFFF;
    *out++ = y;
    *out++ = (h << 16) | w;
    *out++ = ((x + w) << 16) | 0xFFFF;
    *out++ = (u16)(y + h);
    ge::vaddr(&out, start);
    ge::texturemapenable(&out, true);
    ge::shademode(&out, GE_SHADE_GOURAUD);
    ge::vertextype(&out, GE_VTYPE_TC_16BIT, 5, GE_VTYPE_NRM_NONE, GE_VTYPE_POS_16BIT, GE_VTYPE_WEIGHT_NONE, GE_VTYPE_IDX_NONE, 0, 0, true);
    ge::prim(&out, GE_PRIM_RECTANGLES, 2);
    ge::jump(&out, 0);
    Ge::objectPtr->method_088595E8(start + 6, 8, this_->renderGroup);
    Ge::objectPtr->set_write_head(start + 14);
}

// Same as func_eboot_0882B2B4 but tinted: vertex colour (a<<24 | b<<16 | g<<8 | r), 8888 vertices.
extern "C" void func_eboot_0882B40C(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b, u8 a, int textureId) {
    using namespace immediate_ge;
    func_eboot_08831984(this_, &D_eboot_089366F0[textureId]);
    u32 *start = Ge::objectPtr->write_head();
    u32 *out = start;
    ge::jump(&out, 0);
    Ge::objectPtr->method_088595E8(start, 2, this_->renderGroup);
    Ge::objectPtr->set_write_head(start + 2);

    start = Ge::objectPtr->write_head();
    out = start;
    u32 color = r | (g << 8) | (b << 16) | (a << 24);
    *out++ = 0;
    *out++ = color;
    *out++ = (y << 16) | x;
    *out++ = 0;
    *out++ = (h << 16) | w;
    *out++ = color;
    *out++ = ((y + h) << 16) | (u16)(x + w);
    *out++ = 0;
    ge::vaddr(&out, start);
    ge::texturemapenable(&out, true);
    ge::shademode(&out, GE_SHADE_GOURAUD);
    ge::vertextype(&out, GE_VTYPE_TC_16BIT, GE_VTYPE_COL_8888, GE_VTYPE_NRM_NONE, GE_VTYPE_POS_16BIT, GE_VTYPE_WEIGHT_NONE, GE_VTYPE_IDX_NONE, 0, 0, true);
    ge::prim(&out, GE_PRIM_RECTANGLES, 2);
    ge::jump(&out, 0);
    Ge::objectPtr->method_088595E8(start + 8, 8, this_->renderGroup);
    Ge::objectPtr->set_write_head(start + 16);
}

// Draws atlas texture `textureId` stretched to w x h at (x, y), opaque white.
extern "C" void func_eboot_0882B60C(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, int textureId) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this_, tex->texture, tex->resource);
    GePacket<6, 9> sprite;
    sprite.vertexWords = 6;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x12800116;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    sprite.vertices[0] = (tex->v << 16) | tex->u;
    sprite.vertices[1] = (x << 16) | 0xFFFF;
    sprite.vertices[2] = y;
    sprite.vertices[3] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
    sprite.vertices[4] = ((x + w) << 16) | 0xFFFF;
    sprite.vertices[5] = (u16)(y + h);
    geDrawPacket(Ge::objectPtr, &sprite, this_->renderGroup);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882B868);

// Draws atlas texture `textureId` 1:1 at (x, y), tinted with an 8888 colour (r, g, b, a).
extern "C" void func_eboot_0882BB90(Cockpit *this_, u16 x, u16 y, u8 r, u8 g, u8 b, u8 a, int textureId) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this_, tex->texture, tex->resource);
    GePacket<8, 9> sprite;
    sprite.vertexWords = 8;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x1280011E;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    sprite.vertices[0] = (tex->v << 16) | tex->u;
    sprite.vertices[1] = r | (g << 8) | (b << 16) | (a << 24);
    sprite.vertices[2] = (y << 16) | x;
    sprite.vertices[3] = 0;
    sprite.vertices[4] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
    sprite.vertices[5] = r | (g << 8) | (b << 16) | (a << 24);
    sprite.vertices[6] = ((y + tex->height) << 16) | (u16)(x + tex->width);
    sprite.vertices[7] = 0;
    geDrawPacket(Ge::objectPtr, &sprite, this_->renderGroup);
}

// Draws atlas texture `textureId` 1:1 at (x, y), tinted with the 8888 colour rgba[0..3].
extern "C" void func_eboot_0882BE2C(Cockpit *this_, u16 x, u16 y, u8 *rgba, int textureId) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this_, tex->texture, tex->resource);
    GePacket<8, 9> sprite;
    sprite.vertexWords = 8;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x1280011E;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    sprite.vertices[0] = (tex->v << 16) | tex->u;
    sprite.vertices[1] = rgba[0] | (rgba[1] << 8) | (rgba[2] << 16) | (rgba[3] << 24);
    sprite.vertices[2] = (y << 16) | x;
    sprite.vertices[3] = 0;
    sprite.vertices[4] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
    sprite.vertices[5] = rgba[0] | (rgba[1] << 8) | (rgba[2] << 16) | (rgba[3] << 24);
    sprite.vertices[6] = ((y + tex->height) << 16) | (u16)(x + tex->width);
    sprite.vertices[7] = 0;
    geDrawPacket(Ge::objectPtr, &sprite, this_->renderGroup);
}

// Draws atlas texture `textureId` stretched to w x h at (x, y), tinted with a 5551 colour built from r,g,b (8-bit -> 5-bit).
extern "C" void func_eboot_0882C0A4(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b, int textureId) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this_, tex->texture, tex->resource);
    GePacket<6, 9> sprite;
    sprite.vertexWords = 6;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x12800116;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    sprite.vertices[0] = (tex->v << 16) | tex->u;
    sprite.vertices[1] = (x << 16) | (((u32)(r << 5) >> 8) | (((u32)(g << 5) >> 8) << 5) | (((u32)(b << 5) >> 8) << 10) | 0x8000);
    sprite.vertices[2] = y;
    sprite.vertices[3] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
    sprite.vertices[4] = ((x + w) << 16) | (((u32)(r << 5) >> 8) | (((u32)(g << 5) >> 8) << 5) | (((u32)(b << 5) >> 8) << 10) | 0x8000);
    sprite.vertices[5] = (u16)(y + h);
    geDrawPacket(Ge::objectPtr, &sprite, this_->renderGroup);
}

// Draws atlas texture `textureId` stretched to w x h at (x, y), tinted with an 8888 colour (r, g, b, a).
extern "C" void func_eboot_0882C360(Cockpit *this_, u16 x, u16 y, s16 w, s16 h, u8 r, u8 g, u8 b, u8 a, int textureId) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this_, tex->texture, tex->resource);
    GePacket<8, 9> sprite;
    sprite.vertexWords = 8;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x1280011E;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    sprite.vertices[0] = (tex->v << 16) | tex->u;
    sprite.vertices[1] = r | (g << 8) | (b << 16) | (a << 24);
    sprite.vertices[2] = (y << 16) | x;
    sprite.vertices[3] = 0;
    sprite.vertices[4] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
    sprite.vertices[5] = r | (g << 8) | (b << 16) | (a << 24);
    sprite.vertices[6] = ((y + h) << 16) | (u16)(x + w);
    sprite.vertices[7] = 0;
    geDrawPacket(Ge::objectPtr, &sprite, this_->renderGroup);
}

// Draws one part of atlas texture `textureId` stretched to w x h at (x, y), 5551 tint:
// part 0 = right half, 1 = left half, 2 = last quarter, 3 = first quarter of the atlas rectangle (UV crop only).
extern "C" void func_eboot_0882C614(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b, int part, int textureId) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this_, tex->texture, tex->resource);
    GePacket<6, 9> sprite;
    sprite.vertexWords = 6;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x12800116;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    switch (part) {
    case 0:
        sprite.vertices[0] = (tex->v << 16) | (tex->u + tex->width / 2);
        sprite.vertices[3] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
        break;
    case 1:
        sprite.vertices[0] = (tex->v << 16) | tex->u;
        sprite.vertices[3] = ((tex->v + tex->height) << 16) | (tex->u + tex->width / 2);
        break;
    case 2:
        sprite.vertices[0] = (tex->v << 16) | (tex->u + tex->width - tex->width / 4);
        sprite.vertices[3] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
        break;
    case 3:
        sprite.vertices[0] = (tex->v << 16) | tex->u;
        sprite.vertices[3] = ((tex->v + tex->height) << 16) | (tex->u + tex->width / 4);
        break;
    }
    sprite.vertices[1] = (x << 16) | COCKPIT_RGB5551(r, g, b);
    sprite.vertices[2] = y;
    sprite.vertices[4] = ((x + w) << 16) | COCKPIT_RGB5551(r, g, b);
    sprite.vertices[5] = (u16)(y + h);
    geDrawPacket(Ge::objectPtr, &sprite, this_->renderGroup);
}

// Stretched RGBA atlas sprite. Modes 0/1 use normal geometry with normal/mirrored U;
// modes 2/3 reverse the geometry's Y endpoints with the same U choices. Modes 4/5
// retain normal geometry and reverse V / both UV axes. Callers supply mode 0..5.
// Coordinates and texture come from the caller; packets use this_->renderGroup.
extern "C" void func_eboot_0882C9F8(Cockpit *this_, u16 x, u16 y, s16 w, s16 h, u8 r, u8 g, u8 b, u8 a, int textureId, int flip) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this_, tex->texture, tex->resource);
    GePacket<8, 9> sprite;
    sprite.vertexWords = 8;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x1280011E;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    sprite.vertices[1] = r | (g << 8) | (b << 16) | (a << 24);
    sprite.vertices[3] = 0;
    sprite.vertices[5] = r | (g << 8) | (b << 16) | (a << 24);
    sprite.vertices[7] = 0;
    switch ((u8)flip) {
    case 0:
        sprite.vertices[2] = (y << 16) | x;
        sprite.vertices[6] = ((y + h) << 16) | (u16)(x + w);
        sprite.vertices[0] = (tex->v << 16) | tex->u;
        sprite.vertices[4] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
        break;
    case 1:
        sprite.vertices[2] = (y << 16) | x;
        sprite.vertices[6] = ((y + h) << 16) | (u16)(x + w);
        sprite.vertices[0] = (tex->v << 16) | (tex->u + tex->width);
        sprite.vertices[4] = ((tex->v + tex->height) << 16) | tex->u;
        break;
    case 2:
        sprite.vertices[2] = ((y + h) << 16) | x;
        sprite.vertices[6] = (y << 16) | (u16)(x + w);
        sprite.vertices[0] = (tex->v << 16) | tex->u;
        sprite.vertices[4] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
        break;
    case 3:
        sprite.vertices[2] = ((y + h) << 16) | x;
        sprite.vertices[6] = (y << 16) | (u16)(x + w);
        sprite.vertices[0] = (tex->v << 16) | (tex->u + tex->width);
        sprite.vertices[4] = ((tex->v + tex->height) << 16) | tex->u;
        break;
    case 4:
        sprite.vertices[2] = (y << 16) | x;
        sprite.vertices[6] = ((y + h) << 16) | (u16)(x + w);
        sprite.vertices[0] = ((tex->v + tex->height) << 16) | tex->u;
        sprite.vertices[4] = (tex->v << 16) | (tex->u + tex->width);
        break;
    case 5:
        sprite.vertices[2] = (y << 16) | x;
        sprite.vertices[6] = ((y + h) << 16) | (u16)(x + w);
        sprite.vertices[0] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
        sprite.vertices[4] = (tex->v << 16) | tex->u;
        break;
    }
    geDrawPacket(Ge::objectPtr, &sprite, this_->renderGroup);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882CEE0);

// Textured quad (triangle strip, 4 corners pts[0..3] = TL, BL, TR, BR) of atlas texture `textureId`, 8888 tint.
extern "C" void func_eboot_0882D19C(Cockpit *this_, ScePspSVector2 *pts, u8 r, u8 g, u8 b, u8 a, int textureId) {
    CockpitTexture *tex = &D_eboot_089366F0[textureId];
    func_eboot_088315C8(this_, tex->texture, tex->resource);
    GePacket<16, 8> quad;
    quad.vertexWords = 16;
    quad.commandWords = 8;
    quad.commands[0] = 0x1E000001;
    quad.commands[1] = 0x50000001;
    quad.commands[2] = 0x1280011E;
    quad.commands[5] = 0x04040004;
    quad.commands[6] = 0x10000000;
    quad.commands[7] = 0x08000000;
    quad.vertices[0] = (tex->v << 16) | tex->u;
    quad.vertices[1] = r | (g << 8) | (b << 16) | (a << 24);
    quad.vertices[2] = (pts[0].y << 16) | (u16)pts[0].x;
    quad.vertices[3] = 0;
    quad.vertices[4] = ((tex->v + tex->height) << 16) | tex->u;
    quad.vertices[5] = r | (g << 8) | (b << 16) | (a << 24);
    quad.vertices[6] = (pts[1].y << 16) | (u16)pts[1].x;
    quad.vertices[7] = 0;
    quad.vertices[8] = (tex->v << 16) | (tex->u + tex->width);
    quad.vertices[9] = r | (g << 8) | (b << 16) | (a << 24);
    quad.vertices[10] = (pts[2].y << 16) | (u16)pts[2].x;
    quad.vertices[11] = 0;
    quad.vertices[12] = ((tex->v + tex->height) << 16) | (tex->u + tex->width);
    quad.vertices[13] = r | (g << 8) | (b << 16) | (a << 24);
    quad.vertices[14] = (pts[3].y << 16) | (u16)pts[3].x;
    quad.vertices[15] = 0;
    geDrawPacket(Ge::objectPtr, &quad, this_->renderGroup);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882D464);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882D6B4);

// Filled rectangle (x, y, w, h), untextured, 5551 colour from 8-bit r,g,b: GE sprite primitive.
extern "C" void func_eboot_0882D934(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b) {
    
    GePacket<4, 8> rect;
    rect.vertexWords = 4;
    rect.commandWords = 8;
    rect.commands[0] = 0x1E000000;
    rect.commands[1] = 0x50000001;
    rect.commands[2] = 0x12800114;
    rect.commands[5] = 0x04060002;
    rect.commands[6] = 0x10000000;
    rect.commands[7] = 0x08000000;
    rect.vertices[0] = (x << 16) | COCKPIT_RGB5551(r, g, b);
    rect.vertices[1] = y;
    rect.vertices[2] = ((x + w) << 16) | COCKPIT_RGB5551(r, g, b);
    rect.vertices[3] = (u16)(y + h);
    geDrawPacket(Ge::objectPtr, &rect, this_->renderGroup);
}

// Filled rectangle (x, y, w, h), untextured, 8888 colour (r, g, b, a): GE sprite primitive.
extern "C" void func_eboot_0882DB20(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b, u8 a) {
    GePacket<6, 8> rect;
    rect.vertexWords = 6;
    rect.commandWords = 8;
    rect.commands[0] = 0x1E000000;
    rect.commands[1] = 0x50000001;
    rect.commands[2] = 0x1280011C;
    rect.commands[5] = 0x04060002;
    rect.commands[6] = 0x10000000;
    rect.commands[7] = 0x08000000;
    rect.vertices[0] = (r | (g << 8) | (b << 16) | (a << 24));
    rect.vertices[1] = (y << 16) | x;
    rect.vertices[2] = 0;
    rect.vertices[3] = (r | (g << 8) | (b << 16) | (a << 24));
    rect.vertices[4] = ((y + h) << 16) | (u16)(x + w);
    rect.vertices[5] = 0;
    geDrawPacket(Ge::objectPtr, &rect, this_->renderGroup);
}

// Line (x0, y0)-(x1, y1), untextured, 5551 colour from 8-bit r,g,b: GE line primitive.
extern "C" void func_eboot_0882DD04(Cockpit *this_, u16 x0, u16 y0, u16 x1, u16 y1, u8 r, u8 g, u8 b) {
    GePacket<4, 8> line;
    line.vertexWords = 4;
    line.commandWords = 8;
    line.commands[0] = 0x1E000000;
    line.commands[1] = 0x50000001;
    line.commands[2] = 0x12800114;
    line.commands[5] = 0x04010002;
    line.commands[6] = 0x10000000;
    line.commands[7] = 0x08000000;
    line.vertices[0] = (x0 << 16) | COCKPIT_RGB5551(r, g, b);
    line.vertices[1] = y0;
    line.vertices[2] = (x1 << 16) | COCKPIT_RGB5551(r, g, b);
    line.vertices[3] = y1;
    geDrawPacket(Ge::objectPtr, &line, this_->renderGroup);
}

extern "C" void func_eboot_0882DEF4(Cockpit *, u16 left, u16 top, u16 width, u16 height, u8 alpha, int textureId);

// Box styles select consecutive atlas tiles beginning at 0x40 or 0x9E.
void Cockpit::method_0882DEE4(u16 left, u16 top, u16 width, u16 height, u8 alpha) {
    func_eboot_0882DEF4(this, left, top, width, height, alpha, 0x40);
}

extern "C" void func_eboot_0882DEEC(Cockpit *this_, u16 left, u16 top, u16 width, u16 height, u8 alpha) {
    func_eboot_0882DEF4(this_, left, top, width, height, alpha, 0x9E);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882DEF4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E314);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E504);

extern CockpitRgb D_eboot_08937FA4[]; // item icon colours, indexed by the item's colour id
extern u16 D_eboot_08938014[][56];    // item icon CockpitTexture ids: [small][kind]
extern u8 D_eboot_08938468[];
extern "C" int func_eboot_088305FC(Cockpit *this_, u16 kind, u8 small);

// Item icon (w x h) of icon kind `kind` (0xFF = none) tinted with colour `color`, via the 5551 sprite
// func_eboot_0882C0A4; icons up to 20 px wide use the small atlas row.
extern "C" void func_eboot_0882E780(Cockpit *this_, u8 kind, u8 color, u16 x, u16 y, u16 w, u16 h) {
    if (kind == 0xFF) {
        return;
    }
    int tex = func_eboot_088305FC(this_, kind, w < 0x15);
    func_eboot_0882C0A4(this_, x, y, w, h, D_eboot_08937FA4[color].r, D_eboot_08937FA4[color].g, D_eboot_08937FA4[color].b, tex);
}

extern "C" void func_eboot_0882C9F8(Cockpit *this_, u16 x, u16 y, s16 w, s16 h, u8 r, u8 g, u8 b, u8 a, int textureId, int flip);

// Pair of end caps (texture 0x8E) around a span of width w: the left one mirrored (func_eboot_0882C9F8,
// flip 1) at x, the right one at x + w (func_eboot_0882C360), both tinted r,g,b,a.
extern "C" void func_eboot_0882E848(Cockpit *this_, u16 x, u16 y, u16 w, u8 r, u8 g, u8 b, u8 a) {
    CockpitTexture *cap = &D_eboot_089366F0[0x8E];
    func_eboot_0882C9F8(this_, x, y, cap->width, cap->height, r, g, b, a, 0x8E, 1);
    func_eboot_0882C360(this_, x + w, y, cap->width, cap->height, r, g, b, a, 0x8E);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E934);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E9F8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882EAB0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882F6E8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0882F7C4__7CockpitFP14CockpitMenuboxiUcUci);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0882FBEC__7CockpitFP11CockpitMenuPciUci);

void CockpitMenubox::method_0882FC68(CockpitMenu *menu, int hasTitle, int offsetX, int offsetY) {
    boxLeft = menu->boxLeft + (s16)offsetX;
    boxTop = menu->boxTop + (s16)offsetY;
    fontWidth = menu->fontWidth;
    fontHeight = menu->fontHeight;
    fontColor = menu->fontColor;
    boxStyle = menu->boxStyle;
    stringCount = menu->optionCount;
    stringIds = menu->optionStringIds;
    cursorColor = menu->cursorColor;
    bool title = hasTitle != 0;
    s16 lineSpacing = menu->fontHeight + 2;
    boxWidth = (((menu->fontWidth * menu->optionMaxLength) >> 1) + 0x17) & 0x7FF8;
    boxHeight = lineSpacing * (menu->optionCount + title) + 0x10;
    textLeft = boxLeft + 8;
    textTop = boxTop + (title ? 0x19 : 0xA);
    textLineSpacing = lineSpacing;
    cursorLeft = boxLeft + 6;
    cursorTop = textTop - 2;
    cursorWidth = boxWidth - 0xC;
    cursorHeight = lineSpacing + 1;
    titleLeft = boxLeft + 8;
    titleTop = boxTop + 7;
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882FD7C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0882FE94__7CockpitFP14CockpitTextboxPcUc);

const MonsterListDefinition *Cockpit::method_0882FFAC(u8 enemyType) {
    const MonsterListDefinition *p = &D_eboot_0892E2E0[0];
    for (int i = 0; i < 62; ++i, ++p) {
        if (p->enemyType == enemyType) {
            return p;
        }
    }
    return NULL;
}

extern "C" void func_eboot_08830078(Cockpit *this_, u16 x, u16 y, u16 u, u16 v);

// Monster list entry frame: an 88x64 panel (texture 0xAF) at (x, y), then the monster icon of
// enemyType (MonsterListDefinition bytes +1/+2 = atlas cell) at (x + 20, y + 7) via func_eboot_08830078.
extern "C" void func_eboot_0882FFE8(Cockpit *this_, u16 x, u16 y, u8 enemyType) {
    func_eboot_0882B60C(this_, x, y, 0x58, 0x40, 0xAF);
    const MonsterListDefinition *def = this_->method_0882FFAC(enemyType);
    if (def) {
        func_eboot_08830078(this_, x + 20, y + 7, def->unknown_0x1, def->unknown_0x2);
    }
}

// Draws a 51x51 icon at (x, y) from TMH picture 2 of resource 0xB, UV (u, v)-(u+36, v+36)
// (scaled 36 -> 51 with linear filtering switched on around it).
extern "C" void func_eboot_08830078(Cockpit *this_, u16 x, u16 y, u16 u, u16 v) {
    func_eboot_08832CC0(this_, 1);
    func_eboot_088315C8(this_, 0x8002, 0xB);
    GePacket<6, 9> sprite;
    sprite.vertexWords = 6;
    sprite.commandWords = 9;
    sprite.commands[0] = 0x1E000001;
    sprite.commands[2] = 0x50000001;
    sprite.commands[3] = 0x12800116;
    sprite.commands[6] = 0x04060002;
    sprite.commands[7] = 0x10000000;
    sprite.commands[8] = 0x08000000;
    sprite.commands[1] = 0xC9000100;
    sprite.vertices[0] = (v << 16) | u;
    sprite.vertices[1] = (x << 16) | 0xFFFF;
    sprite.vertices[2] = y;
    sprite.vertices[3] = ((v + 36) << 16) | (u + 36);
    sprite.vertices[4] = ((x + 51) << 16) | 0xFFFF;
    sprite.vertices[5] = (u16)(y + 51);
    geDrawPacket(Ge::objectPtr, &sprite, this_->renderGroup);
    func_eboot_08832CC0(this_, 0);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088302BC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088304E0);

extern u8 D_eboot_08938454[];  // rarity -> palette index (entries 10.. = alternate set)
extern u32 D_eboot_08938414[]; // palette of 0xBBGGRR item rarity colours

// Item rarity tint: alpha << 24 | D_eboot_08938414[D_eboot_08938454[rarity (+10 if alt)]].
// `alt != 0` (not `if (alt)`) gives the target's andi before the test.
extern "C" u32 func_eboot_08830598(Cockpit *this_, u8 rarity, u8 alpha, u8 alt) {
    if (alt != 0) {
        rarity += 10;
    }
    return (alpha << 24) | D_eboot_08938414[D_eboot_08938454[rarity]];
}

extern "C" u8 func_eboot_088305E4(Cockpit *this_, u8 index) {
    return D_eboot_08938468[index];
}

// Icon texture id for item icon kind `kind`, small (<= 20 px) or large variant.
extern "C" int func_eboot_088305FC(Cockpit *this_, u16 kind, u8 small) {
    return D_eboot_08938014[small][kind];
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0883062C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830794);

// Reset the pending notification ring, its 20 slots and the 0x500-byte cache.
extern "C" void func_eboot_08830920(Cockpit *this_) {
    this_->pad_0x10F8[0] = 0;
    this_->pad_0x10F8[1] = 0;
    this_->pad_0x10F8[2] = 0;
    memset(this_->toasts, 0, sizeof(this_->toasts));
    this_->cache.reset(this_->slab, sizeof(this_->slab));
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0883096C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830A00);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830A94);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830BA0);

extern u16 D_eboot_089A9868[2];
extern u16 D_eboot_089A986C;

extern "C" void func_eboot_08830F80(Cockpit *this_) {
    if (this_->unknown_0x5F0 & 0x10) {
        return;
    }
    this_->method_0882FE94(&D_eboot_08935D2C, NULL, 0xFF);
    if (this_->unknown_0x5F0 & 0x20) {
        return;
    }
    SystemFont::objectPtr->setLineSpacing(16);
    SystemFont::objectPtr->setFontColor(FontColor::WHITE);
    int remaining;
    int width;
    int count;
    int n;
    s16 top;
    char *str;
    char buf[8];
    char *next;
    char *next2;
    remaining = this_->unknown_0x5F2;
    str = this_->unknown_0x5EC;
    top = 0x60;
    while (remaining > 0) {
        n = 0;
        width = 22;
        count = 0;
        while (remaining > 0 && width > 0) {
            s16 type = SystemFont::objectPtr->nthCharacterUtf8x(n, buf, str, &next);
            n++;
            if (buf[0] == '\n') {
                break;
            }
            remaining--;
            count++;
            if (type == CharacterType::FULLWIDTH || type == CharacterType::ICON) {
                width--;
            }
            width--;
        }
        SystemFont::objectPtr->setCursor(16, top);
        SystemFont::objectPtr->printTextUtf8x(count, (u8 *)str);
        top += 16;
        if (width == 0) {
            SystemFont::objectPtr->nthCharacterUtf8x(n, buf, str, &next2);
            if (buf[0] == '\n') {
                str = next2;
            } else {
                str = next;
            }
        } else {
            str = next;
        }
    }
    if (this_->unknown_0x5F0 & 2) {
        int idx;
        switch (this_->unknown_0x5F0 & 4) {
        case 0:
            idx = 1;
            break;
        default:
            idx = 0;
            break;
        }
        SystemFont::objectPtr->printfUtf8x(16, top, D_eboot_08935AB8, GameSys::objectPtr->method_0885143C(D_eboot_089A9868[idx]));
    }
    if (this_->unknown_0x5F0 & 8) {
        SystemFont::objectPtr->printfUtf8(0x80, top, FontColor::FLAMINGO, D_eboot_08935AB8, GameSys::objectPtr->method_0885143C(D_eboot_089A986C));
    }
}

// UI-state predicate used by player and camera input checks. The first byte
// is GameSys+0x10, followed by Cockpit+0x604 and the state at Cockpit+0x52F.
extern "C" bool func_eboot_088311B8(Cockpit *this_) {
    if (GameSys::objectPtr->pad_0xE[2]) {
        return true;
    }
    if (this_->unknown_0x604) {
        return true;
    }
    return this_->unknown_0x52F != 0;
}

// Horizontal bar of width w: body texture 0xB5 stretched over w - capWidth, then the end cap 0xB6
// (1:1) at x + w - capWidth. The cap is drawn first.
extern "C" void func_eboot_088311F0(Cockpit *this_, u16 x, u16 y, u16 w, u16 h) {
    CockpitTexture *cap = &D_eboot_089366F0[0xB6];
    this_->method_0882B078(x + w - cap->width, y, 0xB6);
    func_eboot_0882B60C(this_, x, y, w - cap->width, h, 0xB5);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08831290);

void Cockpit::method_0883139C(s16 a, s16 b, u16 *c, s16 d, s8 *e) {
    while (*c != 0xFFFF) {
        if (e != NULL) {
            SystemFont::objectPtr->setFontColor(*e);
            ++e;
        }
        xyprintfx(a, b, D_eboot_08935AB8, UI_STRING(*c));
        ++c;
        b += d;
    }
}

// 12-byte item reference built on the stack for func_eboot_088304E0, which returns the icon
// CockpitTexture id; only kind (+1) and id (+2) are filled here.
struct CockpitItemRef {
    u8 unk0;
    u8 kind;
    u16 id;
    u32 unk4;
    u32 unk8;
};
extern "C" int func_eboot_088304E0(Cockpit *this_, CockpitItemRef *item, int flag);

extern "C" u32 func_eboot_08830598(Cockpit *this_, u8 rarity, u8 alpha, u8 alt); // alpha << 24 | rarity colour
extern "C" int func_eboot_0885C834(EquipManager *, int kind, int id);

// Draws an equipment/item icon (w x h) at (x, y), tinted with the item's rarity colour: EquipManager
// func_eboot_0885C834 gives the rarity, func_eboot_08830598 the colour (B,G,R,A bytes); icons narrower
// than 24 px use the small variant (func_eboot_088304E0 flag).
extern "C" void func_eboot_08831460(Cockpit *this_, u8 kind, u16 id, u16 x, u16 y, u16 w, u16 h, u8 alpha, u8 alt) {
    int small = w < 0x18 ? 1 : 0;
    CockpitItemRef item;
    item.kind = kind;
    item.id = id;
    u32 color = func_eboot_08830598(this_, func_eboot_0885C834(EquipManager::objectPtr, kind, id), alpha, alt);
    int tex = func_eboot_088304E0(this_, &item, small);
    u8 *c = (u8 *)&color;
    func_eboot_0882C360(this_, x, y, w, h, c[2], c[1], c[0], c[3], tex);
}

// Draws a 16x16 item icon at (x, y), tinted by a B,G,R,A byte quad (func_eboot_0882C360, 8888 sprite).
extern "C" void func_eboot_08831540(Cockpit *this_, u8 kind, u16 id, u16 x, u16 y, u8 *bgra) {
    CockpitItemRef item;
    item.kind = kind;
    item.id = id;
    func_eboot_0882C360(this_, x, y, 16, 16, bgra[2], bgra[1], bgra[0], bgra[3], func_eboot_088304E0(this_, &item, 1));
}

// Binds a Cockpit texture in render slot Cockpit::renderGroup: a 12-word state packet
// TEXMODE(swizzled) / TEXFORMAT / TBP0 / TBW0 / TSIZE0 / CLUTFORMAT / CBP / CBPH / LOADCLUT / TEXFLUSH.
// texture & 0x8000: picture (texture & 0xFFF) of TMH resource resource; else VRAM texture slot (texture & 0xFF).
extern "C" void func_eboot_088315C8(Cockpit *this_, int texture, int resource) {
    GeStatePacket packet;
    packet.vertexWords = 0;
    packet.commandWords = 12;
    packet.commands[9] = 0xCB000000;
    packet.commands[10] = 0x10000000;
    packet.commands[11] = 0x08000000;
    if (texture & 0x8000) {
        tmh_header *tmh = (tmh_header *)ResourceManager::objectPtr->find(resource);
        GeTexture tex;
        Ge::objectPtr->method_08859768(tmh, texture & 0xFFF, 0, 0, &tex);
        packet.commands[0] = 0xC2000001;
        packet.commands[1] = 0xC3000000 | tex.format;
        packet.commands[2] = 0xA0000000 | (((u32)tex.data << 8) >> 8);
        u32 width = tex.width;
        packet.commands[3] = 0xA8000000 | (((u32)tex.data & 0xFF000000) >> 8) | width;
        packet.commands[4] = 0xB8000000 | (Ge::objectPtr->method_0885973C(tex.height) << 8) | Ge::objectPtr->method_0885973C(width);
        packet.commands[5] = 0xC500FF00 | tex.palette_width;
        packet.commands[6] = 0xB0000000 | (((u32)tex.palette_data << 8) >> 8);
        packet.commands[7] = 0xB1000000 | (((u32)tex.palette_data & 0xFF000000) >> 8);
        u32 entries = 256;
        if (tex.format == 4) {
            entries = 16;
        }
        packet.commands[8] = 0xC4000000 | ((entries + 7) >> 3);
    } else {
        VramAllocation alloc;
        VramManager::objectPtr->method_08813364(texture & 0xFF, &alloc);
        VramTexture &t = alloc.texture;
        packet.commands[0] = 0xC2000001;
        packet.commands[1] = 0xC3000000 | t.imageFormat;
        packet.commands[2] = 0xA0000000 | (((u32)t.vramAddress << 8) >> 8);
        packet.commands[3] = 0xA8000000 | (((u32)t.vramAddress & 0xFF000000) >> 8) | t.alignedWidth;
        packet.commands[4] = 0xB8000000 | (Ge::objectPtr->method_0885973C(t.alignedHeight) << 8) | Ge::objectPtr->method_0885973C(t.alignedWidth);
        packet.commands[5] = 0xC500FF00 | t.paletteWidth;
        packet.commands[6] = 0xB0000000 | (((u32)t.vramBlockAddress << 8) >> 8);
        packet.commands[7] = 0xB1000000 | (((u32)t.vramBlockAddress & 0xFF000000) >> 8);
        u32 entries = 256;
        if (t.imageFormat == 4) {
            entries = 16;
        }
        packet.commands[8] = 0xC4000000 | ((entries + 7) >> 3);
    }
    geDrawPacket(Ge::objectPtr, &packet, this_->renderGroup);
}

// Binds an atlas sub-rectangle of a Cockpit texture (slot renderGroup, 10 state words + BASE/JUMP):
// same TEXMODE/TEXFORMAT/TSIZE/CLUT words as func_eboot_088315C8, but TBP0 is moved to the swizzled
// block containing (ct->u, ct->v): offset = width * (v / 8) * (128 / ppb) + (u / ppb) * 128, with ppb =
// pixels per 16-byte block row (formats 0-3: 8, 4: 32, 5: 16, else 4). TSIZE is log2 of the sub-rectangle
// (ct->width, ct->height), so the following sprites use UV (0,0)-(w,h). ct->texture & 0x8000 selects a
// TMH picture of resource ct->resource, else VRAM slot ct->texture & 0xFF.
extern "C" void func_eboot_08831984(Cockpit *this_, CockpitTexture *ct) {
    int n = 0;
    u32 *list = Ge::objectPtr->write_head();
    if (ct->texture & 0x8000) {
        tmh_header *tmh = (tmh_header *)ResourceManager::objectPtr->find(ct->resource);
        GeTexture tex;
        Ge::objectPtr->method_08859768(tmh, ct->texture & 0xFFF, 0, 0, &tex);
        list[n++] = 0xC2000001;
        list[n++] = 0xC3000000 | tex.format;
        u32 ppb;
        switch (tex.format) {
        case 0:
        case 1:
        case 2:
        case 3:
            ppb = 8;
            break;
        case 5:
            ppb = 16;
            break;
        case 4:
            ppb = 32;
            break;
        default:
            ppb = 4;
            break;
        }
        u32 blk = 0x80 / ppb;
        u32 offset = tex.width * (ct->v / 8 * blk);
        offset += ppb * (blk * (ct->u / ppb));
        list[n++] = 0xA0000000 | ((((u32)tex.data + offset) << 8) >> 8);
        list[n++] = 0xA8000000 | ((((u32)tex.data + offset) & 0xFF000000) >> 8) | tex.width;
        list[n++] = 0xB8000000 | (Ge::objectPtr->method_0885973C(ct->height) << 8) | Ge::objectPtr->method_0885973C(ct->width);
        list[n++] = 0xC500FF00 | tex.palette_width;
        list[n++] = 0xB0000000 | (((u32)tex.palette_data << 8) >> 8);
        list[n++] = 0xB1000000 | (((u32)tex.palette_data & 0xFF000000) >> 8);
        u32 entries = 256;
        if (tex.format == 4) {
            entries = 16;
        }
        list[n++] = 0xC4000000 | ((entries + 7) >> 3);
        list[n++] = 0xCB000000;
    } else {
        VramAllocation alloc;
        VramManager::objectPtr->method_08813364(ct->texture & 0xFF, &alloc);
        VramTexture &t = alloc.texture;
        list[n++] = 0xC2000001;
        list[n++] = 0xC3000000 | t.imageFormat;
        u32 ppb;
        switch (t.imageFormat) {
        case 0:
        case 1:
        case 2:
        case 3:
            ppb = 8;
            break;
        case 5:
            ppb = 16;
            break;
        case 4:
            ppb = 32;
            break;
        default:
            ppb = 4;
            break;
        }
        u32 blk = 0x80 / ppb;
        u32 offset = t.alignedWidth * (ct->v / 8 * blk);
        offset += ppb * (blk * (ct->u / ppb));
        list[n++] = 0xA0000000 | ((((u32)t.vramAddress + offset) << 8) >> 8);
        list[n++] = 0xA8000000 | ((((u32)t.vramAddress + offset) & 0xFF000000) >> 8) | t.alignedWidth;
        list[n++] = 0xB8000000 | (Ge::objectPtr->method_0885973C(ct->height) << 8) | Ge::objectPtr->method_0885973C(ct->width);
        list[n++] = 0xC500FF00 | t.paletteWidth;
        list[n++] = 0xB0000000 | (((u32)t.vramBlockAddress << 8) >> 8);
        list[n++] = 0xB1000000 | (((u32)t.vramBlockAddress & 0xFF000000) >> 8);
        u32 entries = 256;
        if (t.imageFormat == 4) {
            entries = 16;
        }
        list[n++] = 0xC4000000 | ((entries + 7) >> 3);
        list[n++] = 0xCB000000;
    }
    list[n++] = 0x10000000;
    list[n++] = 0x08000000;
    Ge::objectPtr->method_088595E8(list, n, this_->renderGroup);
    Ge::objectPtr->set_write_head(list + n);
}

// Horizontal glow bar in slot renderGroup: two untextured Gouraud triangle strips (VTYPE 0x1280011C =
// through, col8888, pos16; 3 words per vertex) from (x, y) to (x + w, y + h). Alpha ramps 0 -> a at
// the middle column x + w/2 -> 0 at the right edge; RGB is constant. Written straight at the write head
// (vertices, then BASE/VADDR/TME off/SHADE gouraud/VTYPE/PRIM strip 4/BASE/JUMP), 20 words per half.
extern "C" void func_eboot_08831DD8(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b, u8 a) {
    int width = w;
    int left = x;
    u32 *list = Ge::objectPtr->write_head();
    u32 color0 = r | (g << 8) | (b << 16);
    u32 top = y << 16;
    u32 bottom = (y + h) << 16;
    list[0] = color0;
    list[1] = top | (u16)left;
    list[2] = 0;
    list[3] = color0;
    list[4] = (u16)left | bottom;
    list[5] = 0;
    u16 mid = (s16)(left + (width >> 1));
    u32 color1 = (a << 24) | color0;
    list[6] = color1;
    list[7] = top | mid;
    list[8] = 0;
    list[9] = color1;
    list[10] = mid | bottom;
    list[11] = 0;
    list[12] = 0x10000000 | (((u32)list & 0xFF000000) >> 8);
    list[13] = 0x01000000 | (((u32)list << 8) >> 8);
    list[14] = 0x1E000000;
    list[15] = 0x50000001;
    list[16] = 0x1280011C;
    list[17] = 0x04040004;
    list[18] = 0x10000000;
    list[19] = 0x08000000;
    Ge::objectPtr->method_088595E8(list + 12, 8, this_->renderGroup);
    Ge::objectPtr->set_write_head(list + 20);
    list = Ge::objectPtr->write_head();
    u16 right = left + width;
    list[0] = color1;
    list[1] = top | mid;
    list[2] = 0;
    list[3] = color1;
    list[4] = mid | bottom;
    list[5] = 0;
    list[6] = color0;
    list[7] = top | right;
    list[8] = 0;
    list[9] = color0;
    list[10] = bottom | right;
    list[11] = 0;
    list[12] = 0x10000000 | (((u32)list & 0xFF000000) >> 8);
    list[13] = 0x01000000 | (((u32)list << 8) >> 8);
    list[14] = 0x1E000000;
    list[15] = 0x50000001;
    list[16] = 0x1280011C;
    list[17] = 0x04040004;
    list[18] = 0x10000000;
    list[19] = 0x08000000;
    Ge::objectPtr->method_088595E8(list + 12, 8, this_->renderGroup);
    Ge::objectPtr->set_write_head(list + 20);
}

// Pulsing glow bar: func_eboot_08831DD8 with alpha 0xBF + 48 * sin(2*pi * (System::loopCount & 63) / 64)
// (a 64-frame cycle, alpha 0x8F..0xEF).
extern "C" void func_eboot_08832064(Cockpit *this_, u16 x, u16 y, u16 w, u16 h, u8 r, u8 g, u8 b) {
    u16 angle = (System::objectPtr->loopCount & 0x3F) << 10;
    float s = vsin_s(9.58738e-5f * (u32)angle);
    func_eboot_08831DD8(this_, x, y, w, h, r, g, b, (s8)(48.0f * s) + 0xBF);
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832120);

// The player query reads this halfword as an item id; states 0 and 6 return 0.
extern "C" u16 func_eboot_0883232C(Cockpit *this_) {
    if (this_->unknown_0x52F == 0 || this_->unknown_0x52F == 6) {
        return 0;
    }
    return *(u16 *)&this_->pad_0x587[9];
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832350);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832670);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088327AC);

// Release cockpit texture resources only while the loaded flag is set.
extern "C" void func_eboot_08832960(Cockpit *this_) {
    if (this_->flags_0x528 & 0x1000) {
        ResourceManager::objectPtr->free_all(ResourceType::COCK_TMH);
        this_->flags_0x528 &= ~0x1000;
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088329AC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832A68);

// Blend mode for the following Cockpit sprites (slot renderGroup). additive != 0: alpha blend on,
// BLEND 0xDF0000A2 with FIXB 0xE1FFFFFF (src*srcA + dst*FIX white = additive glow); else the normal
// BLEND 0xDF000032 (src*srcA + dst*(1-srcA)).
extern "C" void func_eboot_08832BC4(Cockpit *this_, int additive) {
    u32 *list = Ge::objectPtr->write_head();
    if (additive) {
        list[0] = 0x21000001;
        list[1] = 0xDF0000A2;
        list[2] = 0xE1FFFFFF;
        list[3] = 0x10000000;
        list[4] = 0x08000000;
        Ge::objectPtr->method_088595E8(list, 5, this_->renderGroup);
        Ge::objectPtr->set_write_head(list + 5);
    } else {
        list[0] = 0x21000001;
        list[1] = 0xDF000032;
        list[2] = 0x10000000;
        list[3] = 0x08000000;
        Ge::objectPtr->method_088595E8(list, 4, this_->renderGroup);
        Ge::objectPtr->set_write_head(list + 4);
    }
}

// Emits TEXFILTER: linear != 0 -> min/mag linear (0xC6000101), else nearest (0xC6000000).
extern "C" void func_eboot_08832CC0(Cockpit *this_, int linear) {
    u32 *list;
    s32 n = 0;
    list = Ge::objectPtr->write_head();
    if (linear == 0) {
        list[n++] = 0xC6000000;
    } else {
        list[n++] = 0xC6000101;
    }
    list[n++] = 0x10000000;
    list[n++] = 0x08000000;
    Ge::objectPtr->method_088595E8(list, n, this_->renderGroup);
    Ge::objectPtr->set_write_head(list + n);
}

extern u16 D_eboot_089A9AD8; // UI string id of the rotate hint

// Rotate tooltip `index` (0: top right, 1: bottom left): icon 0x38 at (iconLeft, iconTop), then the
// UI string D_eboot_089A9AD8 in 14x14 font at (textLeft, textTop).
extern "C" void func_eboot_08832D88(Cockpit *this_, u8 index) {
    RotateTooltip *t = &D_eboot_08935D48[index];
    this_->method_0882B078(t->iconLeft, t->iconTop, 0x38);
    SystemFont::objectPtr->setFontSize(14, 14);
    SystemFont::objectPtr->printfUtf8(t->textLeft, t->textTop, 0, GameSys::objectPtr->method_0885143C(D_eboot_089A9AD8));
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832E0C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832F68);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08833054);

const MonsterListDefinition D_eboot_0892E2E0[62] = {
    { 0, 0xD8, 0xB4, 9, 0, 0, 0, 0x33F, 0x7},
    { 1, 0x0, 0xD8, 23, 0, 0, 0, 0x33F, 0x7},
    { 2, 0x6C, 0xFC, 57, 0, 0, 0, 0x340, 0x7},
    { 3, 0x6C, 0xFC, 79, 0, 0, 0, 0x340, 0x7},
    { 4, 0xD8, 0x24, 19, 0, 0, 0, 0x341, 0x9},
    { 5, 0xD8, 0x24, 80, 0, 0, 0, 0x341, 0x9},
    { 6, 0xD8, 0x48, 24, 0, 0, 0, 0x341, 0x9},
    { 7, 0x90, 0x6C, 56, 0, 0, 0, 0x342, 0x9},
    { 8, 0xB4, 0xD8, 69, 0, 0, 0, 0x343, 0x8},
    { 9, 0xD8, 0xD8, 70, 0, 0, 0, 0x343, 0x8},
    {10, 0xD8, 0x0, 3, 0, 0, 0, 0x343, 0x8},
    {11, 0x24, 0xD8, 4, 0, 0, 0, 0x343, 0x8},
    {12, 0xD8, 0x90, 12, 0, 0, 0, 0x343, 0x8},
    {13, 0xD8, 0x6C, 25, 0, 0, 0, 0x343, 0x8},
    {14, 0x48, 0xD8, 35, 0, 0, 0, 0x344, 0x1},
    {15, 0x6C, 0xD8, 77, 119, 123, 90, 0x344, 0x1},
    {16, 0x0, 0x0, 16, 0, 0, 0, 0x344, 0x1},
    {17, 0x6C, 0x0, 27, 119, 123, 90, 0x344, 0x1},
    {18, 0x24, 0x0, 13, 0, 0, 0, 0x344, 0x1},
    {19, 0x90, 0x0, 28, 119, 123, 90, 0x344, 0x1},
    {20, 0x48, 0x0, 30, 0, 0, 0, 0x344, 0x1},
    {21, 0xB4, 0x0, 31, 124, 136, 90, 0x344, 0x1},
    {22, 0x0, 0x24, 6, 114, 122, 90, 0x344, 0x1},
    {23, 0x24, 0x6C, 40, 113, 121, 91, 0x346, 0x1},
    {24, 0x24, 0x24, 20, 118, 125, 93, 0x344, 0x1},
    {25, 0x24, 0xFC, 82, 115, 121, 91, 0x345, 0x1},
    {26, 0x90, 0xD8, 63, 0, 0, 0, 0x348, 0x2},
    {27, 0xB4, 0x24, 1, 118, 129, 93, 0x347, 0x2},
    {28, 0x90, 0x24, 11, 114, 127, 90, 0x347, 0x2},
    {29, 0x0, 0x48, 15, 118, 135, 93, 0x347, 0x2},
    {30, 0x48, 0x48, 22, 118, 129, 93, 0x347, 0x2},
    {31, 0x24, 0x48, 17, 123, 135, 97, 0x347, 0x2},
    {32, 0x6C, 0x48, 26, 116, 127, 94, 0x347, 0x2},
    {33, 0x90, 0x48, 14, 123, 139, 97, 0x347, 0x2},
    {34, 0x0, 0xB4, 75, 119, 123, 90, 0x349, 0x2},
    {35, 0xB4, 0xFC, 81, 119, 123, 90, 0x34A, 0x2},
    {36, 0x24, 0xB4, 76, 0, 0, 0, 0x34B, 0x2},
    {37, 0x24, 0xB4, 88, 0, 0, 0, 0x34C, 0x2},
    {38, 0x48, 0x24, 34, 0, 0, 0, 0x34D, 0x4},
    {39, 0x48, 0x24, 8, 118, 122, 93, 0x34D, 0x4},
    {40, 0x6C, 0x24, 21, 123, 134, 97, 0x34D, 0x4},
    {41, 0x0, 0xFC, 83, 110, 116, 85, 0x34E, 0x4},
    {42, 0x0, 0x90, 66, 0, 0, 0, 0x34F, 0x5},
    {43, 0x0, 0x90, 48, 110, 123, 88, 0x34F, 0x5},
    {44, 0x24, 0x90, 73, 0, 0, 0, 0x350, 0x5},
    {45, 0x24, 0x90, 67, 114, 120, 94, 0x350, 0x5},
    {46, 0x90, 0xFC, 55, 0, 0, 0, 0x351, 0x5},
    {47, 0x48, 0x6C, 5, 0, 0, 0, 0x352, 0x3},
    {48, 0x48, 0x6C, 68, 113, 130, 98, 0x352, 0x3},
    {49, 0xB4, 0xB4, 62, 0, 0, 0, 0x352, 0x3},
    {50, 0x48, 0xB4, 52, 115, 125, 97, 0x352, 0x3},
    {51, 0x90, 0xB4, 61, 0, 0, 0, 0x352, 0x3},
    {52, 0x6C, 0x6C, 51, 125, 138, 99, 0x352, 0x3},
    {53, 0xB4, 0x6C, 53, 125, 140, 105, 0x353, 0x3},
    {54, 0xB4, 0x48, 33, 130, 177, 97, 0x354, 0x6},
    {55, 0x48, 0x90, 54, 110, 120, 91, 0x355, 0x6},
    {56, 0x6C, 0x90, 59, 120, 141, 96, 0x356, 0x6},
    {57, 0x90, 0x90, 64, 115, 121, 91, 0x357, 0x6},
    {58, 0xB4, 0x90, 65, 110, 125, 88, 0x358, 0x6},
    {59, 0x0, 0x6C, 7, 0, 0, 0, 0x359, 0x6},
    {60, 0x48, 0xFC, 58, 0, 0, 0, 0x35A, 0x6},
    {61, 0x24, 0xB4, 2, 0, 0, 0, 0x35B, 0x6},
};
