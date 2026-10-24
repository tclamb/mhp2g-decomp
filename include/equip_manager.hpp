#pragma once

#include "common.h"
#include "game_sys.hpp"
#include "player.hpp"
#include "singleton.hpp"

struct EquipmentType {
    enum {
        INVALID_EQUIPMENT_TYPE = -1,
        ARMOR_LEG,
        ARMOR_HEAD,
        ARMOR_CHEST,
        ARMOR_ARM,
        ARMOR_WAIST,
        WEAPON_BLADEMASTER,
        WEAPON_GUNNER,
        EQUIPMENT_TYPE_COUNT,
    };
private:
    EquipmentType();
};

struct WeaponTypeGroup {
    enum {
        GREATSWORD_LONGSWORD,
        BOWGUN_BOW,
        HAMMER_HUNTING_HORN,
        LANCE_GUNLANCE,
        SWORD_DUAL_BLADES,
    };
private:
    WeaponTypeGroup();
};

struct ArmorApplicabilityFlag {
    enum {
        MALE = 1 << 0,
        FEMALE = 1 << 1,
        BLADEMASTER = 1 << 2,
        GUNNER = 1 << 3,
        ALL = MALE | FEMALE | BLADEMASTER | GUNNER,
    };
private:
    ArmorApplicabilityFlag();
};

struct WeaponTypeFlag {
    enum {
        BLADEMASTER = 1 << 0,
        GUNNER = 1 << 1,
        CAN_BLOCK = 1 << 2,
    };
private:
    WeaponTypeFlag();
};

struct ShopCraftingRecipeType {
    enum {
        WEAPONCRAFT,
        ARMORCRAFT,
        DECORATIONCRAFT,
    };
private:
    ShopCraftingRecipeType();
};

struct ShopCraftingRecipeUnlockType {
    enum {
        NEED_KEY_ITEMS,
        ALWAYS_UNLOCKED,
        COMPLETED_HR9_ABSOLUTE_ZERO,
    };
private:
    ShopCraftingRecipeUnlockType();
};

struct ShopCraftingMaterialSource {
    enum {
        ITEM_BAG_AND_BOX,
        BAG_ONLY,
        BOX_ONLY,
    };
private:
    ShopCraftingMaterialSource();
};

struct ShopCraftingVisibility {
    enum {
        HIDDEN,
        NOT_CRAFTABLE,
        CRAFTABLE,
    };
private:
    ShopCraftingVisibility();
};

struct ShopCraftingRecipeFlag {
    enum {
        SEEN = 1 << 0,
        NEW = 1 << 1,
    };
private:
    ShopCraftingRecipeFlag();
};

struct ArmorSkillInfo {
    u8 type;
    s8 points;
};

struct WeaponStatusId {
    enum {
        NONE,
        POISON,
        PARALYSIS,
        SLEEP,
    };
private:
    WeaponStatusId();
};

struct WeaponElementId {
    enum {
        NONE,
        FIRE,
        WATER,
        THUNDER,
        DRAGON,
        ICE,
        POISON,
        PARALYSIS,
        SLEEP,
    };
private:
    WeaponElementId();
};

struct ArmorInfo {
    u16 maleModelId;
    u16 femaleModelId;
    u8 applicability;
    u8 rarity;
    u32 price;
    u8 defense;
    s8 fireResistance;
    s8 waterResistance;
    s8 thunderResistance;
    s8 dragonResistance;
    s8 iceResistance;
    u8 slots;
    u8 colorSeries;
    u8 armorSpheres[7];
    u16 unknown_0x1C;
    ArmorSkillInfo skills[5];
};

struct BlademasterWeaponInfo {
    u16 modelId;
    u8 rarity;
    u32 price;
    u8 sharpnessId;
    u8 sharpnessCount;
    u16 attack;
    u8 defense;
    u8 affinity;
    u8 elementId;
    u8 elementValue;
    u8 statusId;
    u8 statusValue;
    u8 slots;
    u8 subType;
    u16 sortOrder;
};

struct GunnerWeaponInfo {
    u16 modelId;
    u8 rarity;
    u8 bowgunTableId;
    u32 price;
    u16 attack;
    u8 defense;
    u8 chargeLevels;
    u8 slots;
    u8 affinity;
    u16 sortOrder;
    u8 chargeType;
    u8 elementId;
    u8 elementValue;
    u8 reload;
    u32 pad_0x14;
    u32 ammoTypes;
};

struct DecorationSkillEntry {
    u8 type;
    s8 amount;
};

struct DecorationInfo {
    u8 size;
    u8 applicability;
    u32 unknown_0x4;
    DecorationSkillEntry skills[4];
};

struct DecorationApplicabilityFlag {
    enum {
        NONE = 0,
        BLADEMASTER = 1 << 0,
        GUNNER = 1 << 1,
    };
private:
    DecorationApplicabilityFlag();
};

struct ShopCraftingRecipe {
    u8 type;
    u8 unlockType;
    u16 id;
    InventoryEntry materials[4];
    u8 keyMaterialFlags[4];
    u16 craftingRecipeId2nd;
};

struct DualAttributeWeaponData {
    u16 equipmentId;
    u16 attributeType1;
    u16 attribute1;
    u16 attributeType2;
    u16 attribute2;
};

struct EquipmentUpgrade {
    enum {
        NONE = 0,
        LEVEL_1 = 1,
        LEVEL_2 = 2,
        LEVEL_3 = 3,
        LEVEL_4 = 4,
        LEVEL_5 = 5,
        LEVEL_6 = 6,
        LEVEL_7 = 7,
        LEVEL_8 = 8,
        LEVEL_9 = 9,
        LEVEL_10 = 10,
        LEVEL_11 = 11,
        LEVEL_12 = 12,
        LEVEL_13 = 13,
        LEVEL_14 = 14,
        LEVEL_15 = 15,
        SILENCER = 1 << 4,
        POWER_BARREL = 1 << 5,
        ZOOM_SCOPE = 1 << 6,
        SHIELD = 1 << 7,
        LEVEL_MASK = 0xF,
        ATTACHMENT_MASK = 0xF0,
    };
private:
    EquipmentUpgrade();
};

struct EquipmentAttribute {
    enum {
        ATTACK,
        DEFENSE,
        FIRE_RESISTANCE,
        WATER_RESISTANCE,
        THUNDER_RESISTANCE,
        DRAGON_RESISTANCE,
        FIRE_ELEMENT,
        WATER_ELEMENT,
        THUNDER_ELEMENT,
        DRAGON_ELEMENT,
        POISON,
        PARALYSIS,
        SLEEP,
        ICE_RESISTANCE,
    };
private:
    EquipmentAttribute();
};

struct WeaponTypeInfo {
    u8 typeGroup;
    u8 typeFlags;
};

extern ArmorInfo ARMOR_INFO_HEAD[];
extern ArmorInfo ARMOR_INFO_CHEST[];
extern ArmorInfo ARMOR_INFO_ARM[];
extern ArmorInfo ARMOR_INFO_WAIST[];
extern ArmorInfo ARMOR_INFO_LEG[];
extern ArmorInfo *ARMOR_INFO_TABLES[];
extern BlademasterWeaponInfo BLADEMASTER_WEAPON_INFO[];
extern GunnerWeaponInfo GUNNER_WEAPON_INFO[];
extern DecorationInfo DECORATION_INFO[];
extern u8 WEAPON_MODEL_TO_TYPE_TABLE[];
extern ShopCraftingRecipe SHOP_CRAFTING_RECIPE_WEAPON[];
extern ShopCraftingRecipe SHOP_CRAFTING_RECIPE_ARMOR[];
extern ShopCraftingRecipe SHOP_CRAFTING_RECIPE_DECORATION[];
extern DualAttributeWeaponData DUAL_ATTRIBUTE_WEAPON_DATA[];

struct BowgunGrowthEntry {
    s16 attack;
    float unknown_0x4;
    float unknown_0x8;
    float unknown_0xC;
    float unknown_0x10;
    float unknown_0x14;
};

extern BowgunGrowthEntry BOWGUN_GROWTH_TABLE[];
extern BowgunGrowthEntry *BOWGUN_GROWTH_TABLES[];

struct ArmorUpgradeCost {
    float rate;
    float base;
};

extern ArmorUpgradeCost ARMOR_UPGRADE_COST[];

extern s16 SILENCER_ATTACK_BONUS;

extern "C" bool func_eboot_088579A0(GameSys *, u16 unlockId);

union EquipmentInfo {
    ArmorInfo armor;
    BlademasterWeaponInfo blademaster;
    GunnerWeaponInfo gunner;
};

struct EquipManager : Singleton<EquipManager> {
    EquipManager();

    u8 rarity(u8 type, u16 equipmentId);
    char *name(u8 type, u16 id);
    EquipmentInfo *info(EquipmentEntry *equipment);
    ArmorInfo *armorInfo(u8 type, s16 id);
    u32 price(u8 type, u16 id);
    u8 weaponModelType(u16 modelId);
    u8 weaponType(EquipmentEntry *equipment);
    u8 weaponType(u8 type, u16 id);
    u8 weaponSubType(EquipmentEntry *equipment);
    u8 weaponTypeGroup(u8 playerType);
    u8 weaponTypeFlags(u8 playerType);
    s16 gunnerAttack(EquipmentEntry *equipment);
    s8 skillValue(EquipmentEntry *equipment, u8 skillType);
    s8 applyDecorationAdjustment(u16 itemId, u8 skillType);
    s8 torsoSkillValue(Player *player, u8 skillType);
    s16 equipmentValue(Player *player, u8 attribute, u8 usePlayer);
    u16 elementValue(u16 type, u16 id, u16 elementId);
    u16 statusValue(u16 type, u16 id, u16 statusId);
    u8 armorEligibilityFlags(EquipmentEntry *equipment);
    int sellPrice(EquipmentEntry *equipment);
    bool isEquippable(EquipmentEntry *equipment);
    u16 weaponModelId(EquipmentEntry *equipment);
    u8 shopCraftingRecipeVisibility(u16 recipeType, s16 recipeId, u32 materialSource);
    bool boxEquipmentIsGunnerWeapon(s16 index);
    u8 hasBowgunAttachment(s16 index, int attachment);
    u16 bowgunLevel(s16 index);
    void bowgunLevelUp(s16 index);
    void setSilencer(s16 index, int enable);
    void setPowerBarrel(s16 index, int enable);
    void setZoomScope(s16 index, int enable);
    void setShield(s16 index, int enable);
    int bowgunLevelUpPrice(u8 type, u16 id, int level);
    int usedSlots(EquipmentEntry *equipment);
    int emptySlots(EquipmentEntry *equipment);
    DecorationInfo *decorationInfo(u16 itemId);
    bool isDecorationApplicable(EquipmentEntry *equipment, u16 itemId);
    void addDecoration(EquipmentEntry *equipment, u16 itemId);
    void compactDecorations(EquipmentEntry *equipment);
    void removeDecoration(EquipmentEntry *equipment, int slot);
    void copyDecorations(EquipmentEntry *equipment, u16 *out);
    u16 defense(EquipmentEntry *equipment);
    u8 slots(EquipmentEntry *equipment);
    int armorSellPrice(EquipmentEntry *equipment);
    int weaponSellPrice(EquipmentEntry *equipment);
    int decorationSellValue(EquipmentEntry *equipment);
    bool equipmentEquals(EquipmentEntry *a, EquipmentEntry *b);
    u8 maxArmorLevel(EquipmentEntry *equipment);
    int armorUpgradeTier(EquipmentEntry *equipment);
    int armorUpgradePrice(EquipmentEntry *equipment);
    void setRecipeFlag(u8 recipeType, u16 recipeId, u8 flag);
    bool checkRecipeFlag(u8 recipeType, u16 recipeId, u8 flag);
    void clearAllRecipeNewFlags();
    int dualWeaponAttributeId(EquipmentEntry *equipment);
    DualAttributeWeaponData *dualWeaponAttribute(int index);
};
