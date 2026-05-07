#include "cockpit.hpp"

#include "common.h"
#include "item_manager.hpp"
#include "psptypes.h"
#include "result_check.hpp"
#include "singleton.hpp"
#include "system_font.hpp"
#include "game_sys.hpp"
#include "game_task.hpp"

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
    u16 textLeft;
    u16 textTop;
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
#define xycprintf(x, y, c,...) SystemFont::objectPtr->method_08891070(x, y, c, __VA_ARGS__)
#define xyprintf(x, y, ...) SystemFont::objectPtr->method_08890F34(x, y, __VA_ARGS__)
// xy + control sequences?
#define xyprintfx(x, y, ...) SystemFont::objectPtr->method_08891B68(x, y, __VA_ARGS__)
#define ITEM_NAME(itemId) GameSys::objectPtr->method_08851448(itemId)
#define UI_STRING(id) GameSys::objectPtr->method_0885143C(id)
#define INVENTORY_QTY(itemId) GameSys::objectPtr->bagQuantity(itemId)

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
        SystemFont::objectPtr->method_08891070(left, top, color, D_eboot_08935AB8, itemName);
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08818C20);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08818DB8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088193FC);

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

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881C0E0);

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

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D61C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D660);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881D680);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881E6B0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F44C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F480);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0881F64C);

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

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822A84);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822BA0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822E10);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08822F6C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08823214);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882323C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08823444);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088234A8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088239CC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08823AF8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088241B8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08824254);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088242A8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882433C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088243B0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088244DC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_08824514__7CockpitFbi);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08824608);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08824644);

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

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_088259A0__7CockpitFv);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_08825CB4__7CockpitFUibi);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_08826334__7CockpitFUcUsSc);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826590);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_08826600__7CockpitFv);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088266D0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826740);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882692C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088269F4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826B08);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826D4C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826EE0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826F24);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826F8C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08826FB4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827138);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827194);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08827218);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", vtable_0xA0__9StageBaseFv);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882774C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088277D8);

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

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_08828D80__7CockpitFiUs);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828E20);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828F20);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08828FD4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829588);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088295B0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_088295D8__7CockpitFP14InventoryEntry);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088296A8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829740);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829850);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829948);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829C5C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829D10);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829EB8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829F80);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08829FC4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A0E4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A1F4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A508);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A588);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A594);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A5A0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A804);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A8E4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882A928);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882AAD8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882ACFC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882AF00);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882AF54);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882AF98);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882B024);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0882B078__7CockpitFUsUsi);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882B2B4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882B40C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882B60C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882B868);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882BB90);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882BE2C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882C0A4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882C360);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882C614);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882C9F8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882CEE0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882D19C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882D464);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882D6B4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882D934);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882DB20);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882DD04);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0882DEE4__7CockpitFUsUsUsUsUc);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882DEEC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882DEF4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E314);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E504);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E780);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E848);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E934);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882E9F8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882EAB0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882F6E8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0882F7C4__7CockpitFP14CockpitMenuboxiUcUci);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0882FBEC__7CockpitFP11CockpitMenuPciUci);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", method_0882FC68__14CockpitMenuboxFP11CockpitMenuiii);

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

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0882FFE8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830078);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088302BC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088304E0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830598);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088305E4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088305FC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0883062C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830794);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830920);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0883096C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830A00);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830A94);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830BA0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08830F80);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088311B8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088311F0);

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

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08831460);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08831540);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088315C8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08831984);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08831DD8);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832064);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832120);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_0883232C);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832350);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832670);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088327AC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832960);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_088329AC);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832A68);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832BC4);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832CC0);

INCLUDE_ASM("asm/eboot/nonmatchings/cockpit", func_eboot_08832D88);

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
