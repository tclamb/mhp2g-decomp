#pragma once

#include "common.h"
#include "game_sys.hpp"
#include "player.hpp"

struct MixDefinition {
    s16 secondMaterialItemId;
    s16 successItemId;
    u8 rateId;
    u8 quantityId;
    u8 id;
    u8 flags;
    u8 oldCombinationId;
};

struct MixFlag {
    enum {
        ALCHEMY = 0x1,
        BOMB = 0x2,
        REVEAL_SUM = 0x4,
        REVEAL_ADD = 0x8,
        TRAP = 0x10,
    };
private:
    MixFlag();
};

struct MixRateId {
    enum {
        _100_PERCENT,
        _95_PERCENT,
        _90_PERCENT,
        _85_PERCENT,
        _75_PERCENT,
        _65_PERCENT,
        _55_PERCENT,
    };
private:
    MixRateId();
};

struct MixQuantityId {
    enum {
        _1,
        _1_TO_2,
        _2,
        _1_TO_3,
        _2_TO_4,
        _3,
        _5,
    };
private:
    MixQuantityId();
};

struct MixIndexEntry {
    u8 count;
    u8 firstId;
};

struct ItemDefinition {
    u8 type;
    u8 toolbelt;
    u8 rarity;
    u8 stackSize;
    u8 flags;
    u8 iconId;
    u8 colorId;
    u16 ammoBit;
    u16 itemGetSoundType;
    u32 buyValue;
    u32 sellValue;
    u16 unused;
    u16 decorationId;
};

struct ItemIconId {
    enum {
        SMOKEBOMB,
        BALL,
        BOMB,
        MEAT,
        BAIT,
        FISH,
        BOX,
        WHETSTONE,
        DUNG,
        MONSTER_PART,
        BONE,
        BINOCULARS,
        MUSHROOM,
        BUGNET,
        PELT,
        HERB,
        PICKAXE,
        BARREL,
        SEED,
        SPIT,
        BUG,
        TRAP,
        SPIDERWEB,
        SCALE,
        MEDICINE,
        EGG,
        SHOT,
        ORE,
        HUSK,
        MAP,
        FLUTE,
        CLAW,
        WEBBING,
        RUSTSTONE,
        COIN,
        SAC,
        BOOK,
        TICKET,
        KNIFE,
        JEWEL = 0x2B,
        BOOMERANG = 0x31,
        COATING,
        BOTTLE,
        CRAB,
        PALLIUM = 0x36,
        ARMOR_SPHERE,
        NONE = 0xFF,
    };
private:
    ItemIconId();
};

struct ItemType {
    enum {
        NORMAL,
        TRANSPORT,
        BOWGUN_SHOT,
        BOW_COATING,
    };
private:
    ItemType();
};

struct ItemToolbeltFlag {
    enum {
        USE = 1,
        BOWGUN_SHOT = 2,
        BOW_COATING = 4,
    };
private:
    ItemToolbeltFlag();
};

struct ItemGetSoundType {
    enum {
        NORMAL = 0,
        EXTENDED = 2,
        FANFARE = 3,
    };
private:
    ItemGetSoundType();
};

struct ItemFlag {
    enum {
        EDIBLE = 1,
        ACCOUNT = 2,
        SUPPLY = 4,
        TREASURE = 0x10,
        UNIQUE = 0x20,
    };
private:
    ItemFlag();
};

struct ItemSearchResult {
    u16 itemId;
    u16 bagQuantity;
    u16 boxQuantity;
};

struct CombinationCountFilter {
    enum {
        DEFAULT,
        IGNORE_ALCHEMY,
        ANY,
        ONLY_ALCHEMY,
    };
private:
    CombinationCountFilter();
};

struct MixQuantity {
    u8 minimum;
    u8 percents[3];
};

struct ItemManager : Singleton<ItemManager> {
    void setBagItem(u16 itemId);
    u16 bagAdd(u16 itemId, s16 addend);
    s16 bagQuantity(u16 itemId);
    s16 bagSpace(u16 itemId);
    s16 bagRemove(s16 index);
    u16 knownMixCount(u8 filter);
    bool canMix(Player *player, MixDefinition * mix, bool isItemBox);
    s32 transportItemId(u16 itemId);
    bool isMixIngredient(s16 itemId, bool isItemBox, int filter);
    MixDefinition *findMix(s16 firstMaterialItemId, s16 secondMaterialItemId, int flags, bool isItemBox, int filter);
    bool getMixBit(MixDefinition *mix, int flags, bool isItemBox);
    bool getMixBit(s16 firstMaterialItemId, s16 secondMaterialItemId);
    s16 combine(u8 *quantityOut, Player *player, s16 firstMaterialItemId, s16 secondMaterialItemId, int isItemBox);
    s16 boxRemove(u16 itemId, s16 quantity);
    s16 boxRemove(u16 itemId, s16 quantity, s16 index);
    u16 boxFind(u16 itemId);
    s8 mixRate(s16 firstMaterialItemId, s16 secondMaterialItemId, int isItemBox);
    s8 mixRate(MixDefinition *mix, bool isItemBox);
    void setMixBit(MixDefinition *mix);
    u8 maxMixQuantity(MixDefinition *mix);
    MixDefinition *nextMix(u8 *mixId, s8 index, u16 *firstMaterialItemId, u16 *secondMaterialItemId, bool flags);
    int boxStackQuantity(u16 itemId);
    int boxTotalQuantity(u16 itemId);
    int boxTotalSpace(u16 itemId);
    int boxStackSpace(u16 itemId);
    ItemDefinition *bagDefinition(int index);
    void setItemBit(u16 itemId);
    bool getItemBit(u16 itemId);
    int isDecoration(u16 itemId);
    int search(u16 *itemIds, s32 count, ItemSearchResult *results);
    s16 convertMixId2nd(u16 mixId);

    ItemManager();

private:
    u16 knownCombinationCountImpl(UserData &, u8 filter);
};

extern ItemDefinition ITEM_DEFINITIONS[1261];

u8 mixRateColorId(s8 mixRate);
void sort(InventoryEntry *, u32);
