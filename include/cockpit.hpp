#pragma once

#include "common.h"
#include "cache.hpp"
#include "player.hpp"
#include "item_manager.hpp"
#include "game_sys.hpp"
#include "system_font.hpp"
#include "singleton.hpp"


struct CockpitTextbox {
    s16 boxLeft;
    s16 boxTop;
    s16 boxWidth;
    s16 boxHeight;
    u8 fontWidth;
    u8 fontHeight;
    s8 fontColor;
    u8 boxStyle;
    u16 textLeft;
    u16 textTop;
    u32 textLineSpacing;
};

struct CockpitMenu {
    s16 boxLeft;
    s16 boxTop;
    u8 fontWidth;
    u8 fontHeight;
    s8 fontColor;
    u8 boxStyle;
    s16 optionMaxLength;
    s16 optionCount;
    u16 *optionStringIds;
    u32 cursorColor;
};

struct CockpitMenubox {
    s16 boxLeft;
    s16 boxTop;
    s16 boxWidth;
    s16 boxHeight;
    u8 fontWidth;
    u8 fontHeight;
    s8 fontColor;
    u8 boxStyle;
    u16 textLeft;
    u16 textTop;
    s16 textLineSpacing;
    u16 stringCount;
    u16 *stringIds;
    u32 cursorColor;
    s16 cursorLeft;
    s16 cursorTop;
    s16 cursorWidth;
    s16 cursorHeight;
    s16 titleLeft;
    s16 titleTop;

    void method_0882FC68(CockpitMenu *, int, int, int);
};

struct MonsterListDefinition {
    u8 listId;
    u8 unknown_0x1; // texture coordinates?
    u8 unknown_0x2;
    u8 enemyType; // pl_type enum
    u16 silverCrownSize;
    u16 goldCrownSize;
    u16 miniCrownSize;
    u16 unknown_0xA; // description string?
    u16 unknown_0xC; // type string?
};

extern const MonsterListDefinition D_eboot_0892E2E0[62];

struct CockpitTextureId {
    enum {
        SQUARE_BUTTON = 0x35,
    };
private:
    CockpitTextureId();
};

struct StringTableId {
    enum {
        MESSAGE_0,
        LOBBY_0,
        QUEST_0,
        // buffer?
        QUEST_1 = 4,
        LOBBY_3,
        ARCADE_0,
    };
private:
    StringTableId();
};

struct Toast {
    u8 padding[0x24];
    Toast();
    ~Toast();
};

struct Cockpit : Singleton<Cockpit> {
    u8 slab[0x500];
    cache cache;
    u8 pad_0x51C[0x528 - 0x51C];
    u32 flags_0x528;
    u8 unknown_0x52C;
    u8 unknown_0x52D;
    u8 unknown_0x52E;
    u8 unknown_0x52F;
    Player *player;
    u8 renderGroup;
    u8 pad_0x535[0x537 - 0x535];
    u8 yesNoIndex;
    u8 pad_0x538[0x554 - 0x538];
    u8 unknown_0x554;
    u8 pad_0x555[0x57C - 0x555];
    u8 unknown_0x57C;
    u8 unknown_0x57D;
    u8 pad_0x57E[0x586 - 0x57E];
    u8 inventoryCursorIndex;
    u8 pad_0x587[0x5A8 - 0x587];
    MixDefinition *mix;
    u16 mixFirstMaterialItemId;
    u16 mixSecondMaterialItemId;
    u8 mixFirstMaterialIndex;
    u8 mixSecondMaterialIndex;
    u16 unknown_0x5B0;
    s16 mixSuccessItemId;
    s16 mixOutcomeItemId;
    u8 mixState;
    s8 mixConfirmMenuIndex;
    s8 mixIsValid;
    u8 pad_0x5B9[0x5E5 - 0x5BB];
    u8 mixOutcomeQuantity;
    u8 pad_0x5E6[0x5E8 - 0x5E6];
    u32 unknown_0x5E8;
    u8 pad_0x5EC[0x5FC - 0x5EC];
    u16 itemBoxMenuCursor;
    u8 itemBoxMenuId;
    u8 pad_0x5FF[0x604 - 0x5FF];
    u8 unknown_0x604;
    u8 pad_0x605[0x608 - 0x605];
    u8 unknown_0x608;
    u8 pad_0x609[0xE28 - 0x609];
    Toast toasts[20];
    u8 pad_0x10F8[0x1198 - 0x10F8];
    u32 unknown_0x1198;
    u16 unknown_0x119E;
    u16 unknown_0x119C;
    u8 pad_0x11A0[0x120A - 0x11A0];
    u8 resultMenuState;
    u8 resultMenuCursor;
    u8 resultItemCursor;
    u8 resultConfirmCursor;
    u8 resultDescriptionFlags;
    u8 unknown_0x120F;
    s8 resultItemWouldOverflow;
    int resultFramesRemaining;
    u8 isOtomoAiruResults;
    u8 pad_0x1219[0x1220 - 0x1219];


    Cockpit();
    ~Cockpit();

    // drawItemCombinationUI()?
    void method_08818098();

    void method_08824514(bool, int);
    void method_0882536C(SceBool unlimitedTime);
    char *method_0882595C(MixDefinition *);

    void method_088259A0();
    void method_08825CB4(u32, bool, int);
    void method_08826334(u8 itemBoxMenuType, u16 selectionId, s8 itemDescriptionPage);
    void method_08826600();

    // draw texture
    void method_0882B078(u16 left, u16 top, int textureId);

    // draw box type A
    void method_0882DEE4(u16 left, u16 top, u16 width, u16 height, u8 alpha);
    // draw menu
    void method_0882FBEC(CockpitMenu *menu, char *header, int size, u8 alpha, int stringTableId);

    const MonsterListDefinition *method_0882FFAC(u8 enemyType);

    void method_0883139C(s16 a, s16 b, u16 *, s16, s8 *);

    void method_0883FAF0();

    int method_088295D8(InventoryEntry *);
    void method_0882FE94(CockpitTextbox *, char *, u8);
    void method_08818B7C(s16 left, s16 top, u8 itemIndex, u16 itemId, bool selected);
    bool method_08828D80(int, u16);
    void method_0882F7C4(CockpitMenubox *, int, u8 cursorIndex, u8, int);
    void method_0881A280(u8, u32, u32, u32);
};
