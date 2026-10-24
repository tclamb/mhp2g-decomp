#include "equip_manager.hpp"

#include "item_manager.hpp"

template<> EquipManager *Singleton<EquipManager>::objectPtr;

u8 WEAPON_TYPE_FALLBACK[10] = {
    PlType::GREATSWORD,
    PlType::LANCE,
    PlType::SWORD_AND_SHIELD,
    PlType::HAMMER,
    PlType::GREATSWORD,
    PlType::LANCE,
    PlType::SWORD_AND_SHIELD,
    PlType::HAMMER,
    PlType::LIGHT_BOWGUN,
    PlType::LIGHT_BOWGUN,
};

u8 EquipManager::rarity(u8 type, u16 equipmentId) {
    if (equipmentId > 1500) {
        return 0;
    }
    switch (type) {
    case EquipmentType::ARMOR_LEG:
        return ARMOR_INFO_LEG[equipmentId].rarity;
    case EquipmentType::ARMOR_HEAD:
        return ARMOR_INFO_HEAD[equipmentId].rarity;
    case EquipmentType::ARMOR_CHEST:
        return ARMOR_INFO_CHEST[equipmentId].rarity;
    case EquipmentType::ARMOR_ARM:
        return ARMOR_INFO_ARM[equipmentId].rarity;
    case EquipmentType::ARMOR_WAIST:
        return ARMOR_INFO_WAIST[equipmentId].rarity;
    case EquipmentType::WEAPON_BLADEMASTER:
        return BLADEMASTER_WEAPON_INFO[equipmentId].rarity;
    case EquipmentType::WEAPON_GUNNER:
        return GUNNER_WEAPON_INFO[equipmentId].rarity;
    default:
        return 0;
    }
}

char *EquipManager::name(u8 type, u16 id) {
    switch (type) {
    case EquipmentType::ARMOR_LEG:
        return GameSys::objectPtr->armorLegName(id);
    case EquipmentType::ARMOR_HEAD:
        return GameSys::objectPtr->armorHeadName(id);
    case EquipmentType::ARMOR_CHEST:
        return GameSys::objectPtr->armorChestName(id);
    case EquipmentType::ARMOR_ARM:
        return GameSys::objectPtr->armorArmName(id);
    case EquipmentType::ARMOR_WAIST:
        return GameSys::objectPtr->armorWaistName(id);
    case EquipmentType::WEAPON_BLADEMASTER:
        return GameSys::objectPtr->weaponBlademasterName(id);
    case EquipmentType::WEAPON_GUNNER:
        return GameSys::objectPtr->weaponGunnerName(id);
    default:
        return NULL;
    }
}

EquipmentInfo *EquipManager::info(EquipmentEntry *equipment) {
    u8 type = equipment->id.type;
    switch (type) {
    case EquipmentType::ARMOR_LEG:
        return (EquipmentInfo *)&ARMOR_INFO_LEG[equipment->id.id];
    case EquipmentType::ARMOR_HEAD:
        return (EquipmentInfo *)&ARMOR_INFO_HEAD[equipment->id.id];
    case EquipmentType::ARMOR_CHEST:
        return (EquipmentInfo *)&ARMOR_INFO_CHEST[equipment->id.id];
    case EquipmentType::ARMOR_ARM:
        return (EquipmentInfo *)&ARMOR_INFO_ARM[equipment->id.id];
    case EquipmentType::ARMOR_WAIST:
        return (EquipmentInfo *)&ARMOR_INFO_WAIST[equipment->id.id];
    case EquipmentType::WEAPON_BLADEMASTER:
        return (EquipmentInfo *)&BLADEMASTER_WEAPON_INFO[equipment->id.id];
    case EquipmentType::WEAPON_GUNNER:
        return (EquipmentInfo *)&GUNNER_WEAPON_INFO[equipment->id.id];
    default:
        return NULL;
    }
}

ArmorInfo *EquipManager::armorInfo(u8 type, s16 id) {
    return ARMOR_INFO_TABLES[type] + id;
}

u32 EquipManager::price(u8 type, u16 id) {
    switch (type) {
    case EquipmentType::ARMOR_LEG:
        return ARMOR_INFO_LEG[id].price;
    case EquipmentType::ARMOR_HEAD:
        return ARMOR_INFO_HEAD[id].price;
    case EquipmentType::ARMOR_CHEST:
        return ARMOR_INFO_CHEST[id].price;
    case EquipmentType::ARMOR_ARM:
        return ARMOR_INFO_ARM[id].price;
    case EquipmentType::ARMOR_WAIST:
        return ARMOR_INFO_WAIST[id].price;
    case EquipmentType::WEAPON_BLADEMASTER:
        return BLADEMASTER_WEAPON_INFO[id].price;
    case EquipmentType::WEAPON_GUNNER:
        return GUNNER_WEAPON_INFO[id].price;
    default:
        return 9999999;
    }
}

const WeaponTypeInfo WEAPON_TYPE_INFO[12] = {
    { WeaponTypeGroup::GREATSWORD_LONGSWORD, WeaponTypeFlag::BLADEMASTER | WeaponTypeFlag::CAN_BLOCK },
    { WeaponTypeGroup::BOWGUN_BOW,           WeaponTypeFlag::GUNNER                                  },
    { WeaponTypeGroup::HAMMER_HUNTING_HORN,  WeaponTypeFlag::BLADEMASTER                             },
    { WeaponTypeGroup::LANCE_GUNLANCE,       WeaponTypeFlag::BLADEMASTER | WeaponTypeFlag::CAN_BLOCK },
    { WeaponTypeGroup::SWORD_DUAL_BLADES,    WeaponTypeFlag::BLADEMASTER | WeaponTypeFlag::CAN_BLOCK },
    { WeaponTypeGroup::BOWGUN_BOW,           WeaponTypeFlag::GUNNER                                  },
    { WeaponTypeGroup::SWORD_DUAL_BLADES,    WeaponTypeFlag::BLADEMASTER                             },
    { WeaponTypeGroup::GREATSWORD_LONGSWORD, WeaponTypeFlag::BLADEMASTER                             },
    { WeaponTypeGroup::HAMMER_HUNTING_HORN,  WeaponTypeFlag::BLADEMASTER                             },
    { WeaponTypeGroup::LANCE_GUNLANCE,       WeaponTypeFlag::BLADEMASTER | WeaponTypeFlag::CAN_BLOCK },
    { WeaponTypeGroup::BOWGUN_BOW,           WeaponTypeFlag::GUNNER                                  },
    { WeaponTypeGroup::GREATSWORD_LONGSWORD, 0                                                       },
};
u8 EquipManager::weaponModelType(u16 modelId) {
    return WEAPON_MODEL_TO_TYPE_TABLE[modelId];
}

u8 EquipManager::weaponType(EquipmentEntry *equipment) {
    return WEAPON_MODEL_TO_TYPE_TABLE[weaponModelId(equipment)];
}

u8 EquipManager::weaponType(u8 type, u16 id) {
    if (id > 1500) {
        return WEAPON_TYPE_FALLBACK[id - 1501];
    }
    if (type == EquipmentType::WEAPON_BLADEMASTER) {
        return WEAPON_MODEL_TO_TYPE_TABLE[BLADEMASTER_WEAPON_INFO[id].modelId];
    }
    if (type == EquipmentType::WEAPON_GUNNER) {
        return WEAPON_MODEL_TO_TYPE_TABLE[GUNNER_WEAPON_INFO[id].modelId];
    }
    return PlType::GREATSWORD;
}

u8 EquipManager::weaponSubType(EquipmentEntry *equipment) {
    if (equipment->id.id == 0) {
        return 0;
    }
    if (equipment->id.type == EquipmentType::WEAPON_BLADEMASTER) {
        return BLADEMASTER_WEAPON_INFO[equipment->id.id].subType;
    }
    return 0;
}

u8 EquipManager::weaponTypeGroup(u8 playerType) {
    return WEAPON_TYPE_INFO[playerType].typeGroup;
}

u8 EquipManager::weaponTypeFlags(u8 playerType) {
    return WEAPON_TYPE_INFO[playerType].typeFlags;
}

s16 EquipManager::gunnerAttack(EquipmentEntry *equipment) {
    if (equipment->id.type != EquipmentType::WEAPON_GUNNER) {
        return -1;
    }
    s16 baseAttack = GUNNER_WEAPON_INFO[equipment->id.id].attack;
    s16 value = baseAttack;
    u8 type = weaponType(equipment->id.type, equipment->id.id);
    if (type != PlType::BOW) {
        value += BOWGUN_GROWTH_TABLES[GUNNER_WEAPON_INFO[equipment->id.id].bowgunTableId]
                                    [equipment->upgrades & EquipmentUpgrade::LEVEL_MASK].attack;
        if (equipment->upgrades & EquipmentUpgrade::SILENCER) {
            value += SILENCER_ATTACK_BONUS;
        }
        if (equipment->upgrades & EquipmentUpgrade::POWER_BARREL) {
            if (type == PlType::HEAVY_BOWGUN) {
                value = value + baseAttack / 10;
            } else {
                value = value + baseAttack / 15;
            }
        }
    }
    return value;
}

s8 EquipManager::skillValue(EquipmentEntry *equipment, u8 skillType) {
    u16 equipmentId = equipment->id.id;
    if (equipmentId == 0) {
        return 0;
    }
    s8 value = 0;
    switch (equipment->id.type) {
    case EquipmentType::ARMOR_LEG:
    case EquipmentType::ARMOR_HEAD:
    case EquipmentType::ARMOR_CHEST:
    case EquipmentType::ARMOR_ARM:
    case EquipmentType::ARMOR_WAIST: {
        ArmorInfo *info = ARMOR_INFO_TABLES[equipment->id.type];
        info += equipmentId;
        if (info->skills[0].type == skillType) {
            value += info->skills[0].points;
        }
        if (info->skills[1].type == skillType) {
            value += info->skills[1].points;
        }
        if (info->skills[2].type == skillType) {
            value += info->skills[2].points;
        }
        if (info->skills[3].type == skillType) {
            value += info->skills[3].points;
        }
        if (info->skills[4].type == skillType) {
            value += info->skills[4].points;
        }
        for (int i = 0; i < 3; ++i) {
            if (equipment->decorationIds[i] != 0) {
                value += applyDecorationAdjustment(equipment->decorationIds[i], skillType);
            }
        }
        break;
    }
    case EquipmentType::WEAPON_BLADEMASTER:
    case EquipmentType::WEAPON_GUNNER:
        for (int i = 0; i < 3; ++i) {
            if (equipment->decorationIds[i] != 0) {
                value += applyDecorationAdjustment(equipment->decorationIds[i], skillType);
            }
        }
        break;
    }
    return value;
}

s8 EquipManager::applyDecorationAdjustment(u16 itemId, u8 skillType) {
    s8 value = 0;
    DecorationInfo *info = decorationInfo(itemId);
    if (info != NULL) {
        if (info->skills[0].type == skillType) {
            value += info->skills[0].amount;
        }
        if (info->skills[1].type == skillType) {
            value += info->skills[1].amount;
        }
        if (info->skills[2].type == skillType) {
            value += info->skills[2].amount;
        }
        if (info->skills[3].type == skillType) {
            value += info->skills[3].amount;
        }
    }
    return value;
}

s8 EquipManager::torsoSkillValue(Player *player, u8 skillType) {
    s8 value = skillValue(&player->equipment.armor[EquipmentType::ARMOR_CHEST], skillType);
    if (value != 0) {
        s8 multiplier = 1;
        if (skillValue(&player->equipment.armor[EquipmentType::ARMOR_HEAD], SkillType::TORSO_INCREASE)) {
            ++multiplier;
        }
        if (skillValue(&player->equipment.armor[EquipmentType::ARMOR_CHEST], SkillType::TORSO_INCREASE)) {
            ++multiplier;
        }
        if (skillValue(&player->equipment.armor[EquipmentType::ARMOR_ARM], SkillType::TORSO_INCREASE)) {
            ++multiplier;
        }
        if (skillValue(&player->equipment.armor[EquipmentType::ARMOR_WAIST], SkillType::TORSO_INCREASE)) {
            ++multiplier;
        }
        if (skillValue(&player->equipment.armor[EquipmentType::ARMOR_LEG], SkillType::TORSO_INCREASE)) {
            ++multiplier;
        }
        if (skillValue(&player->equipment.weapon, SkillType::TORSO_INCREASE)) {
            ++multiplier;
        }
        value *= multiplier;
    }
    return value;
}

static inline void copyEquipmentEntry(EquipmentEntry *dst, const EquipmentEntry *src) {
    dst->id = src->id;
    dst->upgrades = src->upgrades;
    dst->decorationIds = src->decorationIds;
}

s16 EquipManager::equipmentValue(Player *player, u8 attribute, u8 usePlayer) {
    EquipmentSet equip;
    GameSys *gs = GameSys::objectPtr;

    if ((bool)(gs->flags_0x6AF14 & 4) == true ||
        (bool)(gs->flags_0x6AF14 & 8) == true ||
        player->Pl_master_ck() == false ||
        usePlayer == 1) {
        copyEquipmentEntry(&equip.weapon, &player->equipment.weapon);
        for (int i = 0; i < 5; ++i) {
            copyEquipmentEntry(&equip.armor[i], &player->equipment.armor[i]);
        }
    } else {
        UserData *userData = &gs->userData;
        copyEquipmentEntry(&equip.weapon, &userData->equipment.weapon);
        copyEquipmentEntry(&equip.armor[EquipmentType::ARMOR_HEAD], &userData->equipment.armor[EquipmentType::ARMOR_HEAD]);
        copyEquipmentEntry(&equip.armor[EquipmentType::ARMOR_CHEST], &userData->equipment.armor[EquipmentType::ARMOR_CHEST]);
        copyEquipmentEntry(&equip.armor[EquipmentType::ARMOR_ARM], &userData->equipment.armor[EquipmentType::ARMOR_ARM]);
        copyEquipmentEntry(&equip.armor[EquipmentType::ARMOR_WAIST], &userData->equipment.armor[EquipmentType::ARMOR_WAIST]);
        copyEquipmentEntry(&equip.armor[EquipmentType::ARMOR_LEG], &userData->equipment.armor[EquipmentType::ARMOR_LEG]);
    }

    s16 result = 0;

    switch (attribute) {
    case EquipmentAttribute::ATTACK:
        if (equip.weapon.id.type == EquipmentType::WEAPON_GUNNER) {
            result = gunnerAttack(&equip.weapon);
        } else {
            result = BLADEMASTER_WEAPON_INFO[equip.weapon.id.id].attack;
        }
        break;
    case EquipmentAttribute::DEFENSE:
        result += defense(&equip.armor[EquipmentType::ARMOR_HEAD]);
        result += defense(&equip.armor[EquipmentType::ARMOR_CHEST]);
        result += defense(&equip.armor[EquipmentType::ARMOR_ARM]);
        result += defense(&equip.armor[EquipmentType::ARMOR_WAIST]);
        result += defense(&equip.armor[EquipmentType::ARMOR_LEG]);
        if (equip.weapon.id.type == EquipmentType::WEAPON_GUNNER) {
            result += GUNNER_WEAPON_INFO[equip.weapon.id.id].defense;
        } else {
            result += BLADEMASTER_WEAPON_INFO[equip.weapon.id.id].defense;
        }
        break;
    case EquipmentAttribute::FIRE_RESISTANCE:
        result += ARMOR_INFO_HEAD[equip.armor[EquipmentType::ARMOR_HEAD].id.id].fireResistance;
        result += ARMOR_INFO_CHEST[equip.armor[EquipmentType::ARMOR_CHEST].id.id].fireResistance;
        result += ARMOR_INFO_ARM[equip.armor[EquipmentType::ARMOR_ARM].id.id].fireResistance;
        result += ARMOR_INFO_WAIST[equip.armor[EquipmentType::ARMOR_WAIST].id.id].fireResistance;
        result += ARMOR_INFO_LEG[equip.armor[EquipmentType::ARMOR_LEG].id.id].fireResistance;
        break;
    case EquipmentAttribute::WATER_RESISTANCE:
        result += ARMOR_INFO_HEAD[equip.armor[EquipmentType::ARMOR_HEAD].id.id].waterResistance;
        result += ARMOR_INFO_CHEST[equip.armor[EquipmentType::ARMOR_CHEST].id.id].waterResistance;
        result += ARMOR_INFO_ARM[equip.armor[EquipmentType::ARMOR_ARM].id.id].waterResistance;
        result += ARMOR_INFO_WAIST[equip.armor[EquipmentType::ARMOR_WAIST].id.id].waterResistance;
        result += ARMOR_INFO_LEG[equip.armor[EquipmentType::ARMOR_LEG].id.id].waterResistance;
        break;
    case EquipmentAttribute::THUNDER_RESISTANCE:
        result += ARMOR_INFO_HEAD[equip.armor[EquipmentType::ARMOR_HEAD].id.id].thunderResistance;
        result += ARMOR_INFO_CHEST[equip.armor[EquipmentType::ARMOR_CHEST].id.id].thunderResistance;
        result += ARMOR_INFO_ARM[equip.armor[EquipmentType::ARMOR_ARM].id.id].thunderResistance;
        result += ARMOR_INFO_WAIST[equip.armor[EquipmentType::ARMOR_WAIST].id.id].thunderResistance;
        result += ARMOR_INFO_LEG[equip.armor[EquipmentType::ARMOR_LEG].id.id].thunderResistance;
        break;
    case EquipmentAttribute::DRAGON_RESISTANCE:
        result += ARMOR_INFO_HEAD[equip.armor[EquipmentType::ARMOR_HEAD].id.id].dragonResistance;
        result += ARMOR_INFO_CHEST[equip.armor[EquipmentType::ARMOR_CHEST].id.id].dragonResistance;
        result += ARMOR_INFO_ARM[equip.armor[EquipmentType::ARMOR_ARM].id.id].dragonResistance;
        result += ARMOR_INFO_WAIST[equip.armor[EquipmentType::ARMOR_WAIST].id.id].dragonResistance;
        result += ARMOR_INFO_LEG[equip.armor[EquipmentType::ARMOR_LEG].id.id].dragonResistance;
        break;
    case EquipmentAttribute::FIRE_ELEMENT:
        result = elementValue(equip.weapon.id.type, equip.weapon.id.id, WeaponElementId::FIRE);
        break;
    case EquipmentAttribute::WATER_ELEMENT:
        result = elementValue(equip.weapon.id.type, equip.weapon.id.id, WeaponElementId::WATER);
        break;
    case EquipmentAttribute::THUNDER_ELEMENT:
        result = elementValue(equip.weapon.id.type, equip.weapon.id.id, WeaponElementId::THUNDER);
        break;
    case EquipmentAttribute::DRAGON_ELEMENT:
        result = elementValue(equip.weapon.id.type, equip.weapon.id.id, WeaponElementId::DRAGON);
        break;
    case EquipmentAttribute::POISON:
        result = statusValue(equip.weapon.id.type, equip.weapon.id.id, WeaponStatusId::POISON);
        break;
    case EquipmentAttribute::PARALYSIS:
        result = statusValue(equip.weapon.id.type, equip.weapon.id.id, WeaponStatusId::PARALYSIS);
        break;
    case EquipmentAttribute::SLEEP:
        result = statusValue(equip.weapon.id.type, equip.weapon.id.id, WeaponStatusId::SLEEP);
        break;
    case EquipmentAttribute::ICE_RESISTANCE:
        result += ARMOR_INFO_HEAD[equip.armor[EquipmentType::ARMOR_HEAD].id.id].iceResistance;
        result += ARMOR_INFO_CHEST[equip.armor[EquipmentType::ARMOR_CHEST].id.id].iceResistance;
        result += ARMOR_INFO_ARM[equip.armor[EquipmentType::ARMOR_ARM].id.id].iceResistance;
        result += ARMOR_INFO_WAIST[equip.armor[EquipmentType::ARMOR_WAIST].id.id].iceResistance;
        result += ARMOR_INFO_LEG[equip.armor[EquipmentType::ARMOR_LEG].id.id].iceResistance;
        break;
    }
    return result;
}

u16 EquipManager::elementValue(u16 type, u16 id, u16 elementId) {
    switch (type) {
    case EquipmentType::WEAPON_BLADEMASTER:
        if (BLADEMASTER_WEAPON_INFO[id].elementId == elementId) {
            return BLADEMASTER_WEAPON_INFO[id].elementValue;
        }
        break;
    }
    return 0;
}

u16 EquipManager::statusValue(u16 type, u16 id, u16 statusId) {
    switch (type) {
    case EquipmentType::WEAPON_BLADEMASTER:
        if (BLADEMASTER_WEAPON_INFO[id].statusId == statusId) {
            return BLADEMASTER_WEAPON_INFO[id].statusValue;
        }
        break;
    }
    return 0;
}

u8 EquipManager::armorEligibilityFlags(EquipmentEntry *equipment) {
    switch (equipment->id.type) {
    case EquipmentType::ARMOR_HEAD:
        return ARMOR_INFO_HEAD[equipment->id.id].applicability & ArmorApplicabilityFlag::ALL;
    case EquipmentType::ARMOR_CHEST:
        return ARMOR_INFO_CHEST[equipment->id.id].applicability & ArmorApplicabilityFlag::ALL;
    case EquipmentType::ARMOR_WAIST:
        return ARMOR_INFO_WAIST[equipment->id.id].applicability & ArmorApplicabilityFlag::ALL;
    case EquipmentType::ARMOR_ARM:
        return ARMOR_INFO_ARM[equipment->id.id].applicability & ArmorApplicabilityFlag::ALL;
    case EquipmentType::ARMOR_LEG:
        return ARMOR_INFO_LEG[equipment->id.id].applicability & ArmorApplicabilityFlag::ALL;
    case EquipmentType::WEAPON_BLADEMASTER:
    case EquipmentType::WEAPON_GUNNER:
        return ArmorApplicabilityFlag::ALL;
    default:
        return 0;
    }
}

int EquipManager::sellPrice(EquipmentEntry *equipment) {
    int price = 0;
    switch (equipment->id.type) {
    case EquipmentType::ARMOR_LEG:
    case EquipmentType::ARMOR_HEAD:
    case EquipmentType::ARMOR_CHEST:
    case EquipmentType::ARMOR_ARM:
    case EquipmentType::ARMOR_WAIST:
        price = armorSellPrice(equipment);
        break;
    case EquipmentType::WEAPON_BLADEMASTER:
    case EquipmentType::WEAPON_GUNNER:
        price = weaponSellPrice(equipment);
        break;
    default:
        return 0;
    }
    return price;
}

bool EquipManager::isEquippable(EquipmentEntry *equipment) {
    UserData *userData = &GameSys::objectPtr->userData;
    u8 mask = 0;
    if (userData->sexId == 0) {
        mask |= ArmorApplicabilityFlag::MALE;
    } else {
        mask |= ArmorApplicabilityFlag::FEMALE;
    }
    if (userData->equipment.weapon.id.type == EquipmentType::WEAPON_GUNNER) {
        mask |= ArmorApplicabilityFlag::GUNNER;
    } else {
        mask |= ArmorApplicabilityFlag::BLADEMASTER;
    }
    switch (equipment->id.type) {
    case EquipmentType::ARMOR_HEAD:
        if (mask == (mask & ARMOR_INFO_HEAD[equipment->id.id].applicability)) {
            break;
        }
        return false;
    case EquipmentType::ARMOR_CHEST:
        if (mask == (mask & ARMOR_INFO_CHEST[equipment->id.id].applicability)) {
            break;
        }
        return false;
    case EquipmentType::ARMOR_WAIST:
        if (mask == (mask & ARMOR_INFO_WAIST[equipment->id.id].applicability)) {
            break;
        }
        return false;
    case EquipmentType::ARMOR_ARM:
        if (mask == (mask & ARMOR_INFO_ARM[equipment->id.id].applicability)) {
            break;
        }
        return false;
    case EquipmentType::ARMOR_LEG:
        if (mask == (mask & ARMOR_INFO_LEG[equipment->id.id].applicability)) {
            break;
        }
        return false;
    case EquipmentType::WEAPON_BLADEMASTER:
    case EquipmentType::WEAPON_GUNNER:
        break;
    }
    return true;
}

u16 EquipManager::weaponModelId(EquipmentEntry *equipment) {
    if (equipment->id.type == EquipmentType::WEAPON_BLADEMASTER) {
        return BLADEMASTER_WEAPON_INFO[equipment->id.id].modelId;
    }
    if (equipment->id.type == EquipmentType::WEAPON_GUNNER) {
        return GUNNER_WEAPON_INFO[equipment->id.id].modelId;
    }
    return 0;
}

u8 EquipManager::shopCraftingRecipeVisibility(u16 recipeType, s16 recipeId, u32 materialSource) {
    ShopCraftingRecipe *recipe;
    switch (recipeType) {
    case ShopCraftingRecipeType::WEAPONCRAFT:
        recipe = &SHOP_CRAFTING_RECIPE_WEAPON[recipeId];
        break;
    case ShopCraftingRecipeType::ARMORCRAFT:
        recipe = &SHOP_CRAFTING_RECIPE_ARMOR[recipeId];
        break;
    case ShopCraftingRecipeType::DECORATIONCRAFT:
        recipe = &SHOP_CRAFTING_RECIPE_DECORATION[recipeId];
        break;
    }
    u8 keyOwned = 0;
    u8 keyCount = 0;
    int i;
    int bag;
    int box;
    u8 mask = 0;
    int total;
    for (i = 0; i < 4; ++i) {
        if (recipe->materials[i].itemId == ItemId::NONE || recipe->materials[i].quantity == 0) {
            mask |= 1 << i;
        } else {
            u8 have = ItemManager::objectPtr->getItemBit(recipe->materials[i].itemId) == true;
            switch (materialSource) {
            case ShopCraftingMaterialSource::ITEM_BAG_AND_BOX:
                bag = ItemManager::objectPtr->bagQuantity(recipe->materials[i].itemId);
                box = ItemManager::objectPtr->boxTotalQuantity(recipe->materials[i].itemId);
                break;
            case ShopCraftingMaterialSource::BAG_ONLY:
                bag = ItemManager::objectPtr->bagQuantity(recipe->materials[i].itemId);
                box = 0;
                break;
            case ShopCraftingMaterialSource::BOX_ONLY:
                bag = 0;
                box = ItemManager::objectPtr->boxTotalQuantity(recipe->materials[i].itemId);
                break;
            }
            total = bag + box;
            if (recipe->keyMaterialFlags[i] != 0) {
                ++keyCount;
                if (have > 0) {
                    ++keyOwned;
                }
            }
            if (total >= recipe->materials[i].quantity) {
                mask |= 1 << i;
            }
        }
    }
    if (recipe->unlockType == ShopCraftingRecipeUnlockType::COMPLETED_HR9_ABSOLUTE_ZERO) {
        if (func_eboot_088579A0(GameSys::objectPtr, 14) == false) {
            return ShopCraftingVisibility::HIDDEN;
        }
    }
    if (mask == 0xF) {
        return ShopCraftingVisibility::CRAFTABLE;
    }
    if (keyCount != 0 && keyCount == keyOwned) {
        return ShopCraftingVisibility::NOT_CRAFTABLE;
    }
    if (recipe->unlockType != ShopCraftingRecipeUnlockType::NEED_KEY_ITEMS) {
        return ShopCraftingVisibility::NOT_CRAFTABLE;
    }
    return ShopCraftingVisibility::HIDDEN;
}

bool EquipManager::boxEquipmentIsGunnerWeapon(s16 index) {
    return GameSys::objectPtr->userData.equipmentBox[index].id.type
           == EquipmentType::WEAPON_GUNNER;
}

u8 EquipManager::hasBowgunAttachment(s16 index, int attachment) {
    UserData *userData = &GameSys::objectPtr->userData;
    bool gunner = boxEquipmentIsGunnerWeapon(index);
    if (gunner == false) {
        return 0xFF;
    } else {
        return (userData->equipmentBox[index].upgrades & attachment) != 0;
    }
}

u16 EquipManager::bowgunLevel(s16 index) {
    UserData *userData = &GameSys::objectPtr->userData;
    bool gunner = boxEquipmentIsGunnerWeapon(index);
    if (gunner == false) {
        return 0;
    } else {
        return userData->equipmentBox[index].upgrades & EquipmentUpgrade::LEVEL_MASK;
    }
}

void EquipManager::bowgunLevelUp(s16 index) {
    UserData *userData = &GameSys::objectPtr->userData;
    bool gunner = boxEquipmentIsGunnerWeapon(index);
    if (gunner) {
        u16 upgrades = userData->equipmentBox[index].upgrades;
        u16 level = upgrades & EquipmentUpgrade::LEVEL_MASK;
        if (level >= 4) {
            level = 4;
        } else {
            ++level;
        }
        userData->equipmentBox[index].upgrades = (upgrades & EquipmentUpgrade::ATTACHMENT_MASK) | level;
    }
}

void EquipManager::setSilencer(s16 index, int enable) {
    UserData *userData = &GameSys::objectPtr->userData;
    bool gunner = boxEquipmentIsGunnerWeapon(index);
    if (!gunner) {
        return;
    }
    if (enable == 0) {
        userData->equipmentBox[index].upgrades |= EquipmentUpgrade::SILENCER;
        userData->equipmentBox[index].upgrades &= ~EquipmentUpgrade::POWER_BARREL;
    } else {
        userData->equipmentBox[index].upgrades &= ~EquipmentUpgrade::SILENCER;
    }
}

void EquipManager::setPowerBarrel(s16 index, int enable) {
    UserData *userData = &GameSys::objectPtr->userData;
    bool gunner = boxEquipmentIsGunnerWeapon(index);
    if (!gunner) {
        return;
    }
    if (enable == 0) {
        userData->equipmentBox[index].upgrades |= EquipmentUpgrade::POWER_BARREL;
        userData->equipmentBox[index].upgrades &= ~EquipmentUpgrade::SILENCER;
        userData->equipmentBox[index].upgrades &= ~EquipmentUpgrade::SHIELD;
    } else {
        userData->equipmentBox[index].upgrades &= ~EquipmentUpgrade::POWER_BARREL;
    }
}

void EquipManager::setZoomScope(s16 index, int enable) {
    UserData *userData = &GameSys::objectPtr->userData;
    bool gunner = boxEquipmentIsGunnerWeapon(index);
    if (!gunner) {
        return;
    }
    if (enable == 0) {
        userData->equipmentBox[index].upgrades |= EquipmentUpgrade::ZOOM_SCOPE;
    } else {
        userData->equipmentBox[index].upgrades &= ~EquipmentUpgrade::ZOOM_SCOPE;
    }
}

void EquipManager::setShield(s16 index, int enable) {
    UserData *userData = &GameSys::objectPtr->userData;
    bool gunner = boxEquipmentIsGunnerWeapon(index);
    if (!gunner) {
        return;
    }
    if (enable == 0) {
        userData->equipmentBox[index].upgrades |= EquipmentUpgrade::SHIELD;
        userData->equipmentBox[index].upgrades &= ~EquipmentUpgrade::POWER_BARREL;
    } else {
        userData->equipmentBox[index].upgrades &= ~EquipmentUpgrade::SHIELD;
    }
}

int EquipManager::bowgunLevelUpPrice(u8 type, u16 id, int level) {
    int rate[4] = {2, 3, 8, 15};
    if (type != EquipmentType::WEAPON_GUNNER) {
        return 0;
    }
    if (level < 0 || level > 3) {
        return 0;
    }
    u32 base = price(type, id) / 10;
    return base * rate[level];
}

int EquipManager::usedSlots(EquipmentEntry *equipment) {
    int count = 0;
    for (int i = 0; i < 3; ++i) {
        if (equipment->decorationIds[i] != 0) {
            ++count;
        }
    }
    return count;
}

int EquipManager::emptySlots(EquipmentEntry *equipment) {
    if (equipment->id.type > 6) {
        return 0;
    }
    s8 numSlots = slots(equipment);
    if (numSlots < 1) {
        return 0;
    }
    if (numSlots > 3) {
        numSlots = 3;
    }
    int i;
    int used;
    used = 0;
    for (i = 0; i < numSlots; ++i) {
        if (equipment->decorationIds[i] != 0) {
            s8 size = decorationInfo(equipment->decorationIds[i])->size;
            if (size == 0 || size > 3) {
                size = 1;
            }
            used += size;
        }
    }
    if (used > numSlots) {
        return 0;
    }
    return numSlots - used;
}

DecorationInfo *EquipManager::decorationInfo(u16 itemId) {
    u16 decorationId = ITEM_DEFINITIONS[itemId].decorationId;
    if (decorationId == 0) {
        return NULL;
    }
    return DECORATION_INFO + decorationId;
}

bool EquipManager::isDecorationApplicable(EquipmentEntry *equipment, u16 itemId) {
    DecorationInfo *info = decorationInfo(itemId);
    if (info == NULL) {
        return false;
    } else {
        switch (equipment->id.type) {
        case EquipmentType::WEAPON_BLADEMASTER:
            if ((info->applicability & DecorationApplicabilityFlag::BLADEMASTER) != 0) {
                return true;
            }
            break;
        case EquipmentType::WEAPON_GUNNER:
            if ((info->applicability & DecorationApplicabilityFlag::GUNNER) != 0) {
                return true;
            }
            break;
        default:
            if (equipment->id.type > 4) {
                return false;
            }
            ArmorInfo *armor = armorInfo(equipment->id.type, equipment->id.id);
            u8 armorFlags = armor->applicability & (ArmorApplicabilityFlag::BLADEMASTER | ArmorApplicabilityFlag::GUNNER);
            if ((info->applicability & armorFlags) != 0) {
                return true;
            }
            break;
        }
    }
    return false;
}

void EquipManager::addDecoration(EquipmentEntry *equipment, u16 itemId) {
    for (int i = 0; i < 3; ++i) {
        if (equipment->decorationIds[i] == 0) {
            equipment->decorationIds[i] = itemId;
            return;
        }
    }
}

void EquipManager::compactDecorations(EquipmentEntry *equipment) {
    if (equipment->decorationIds[0] + equipment->decorationIds[1] +
            equipment->decorationIds[2] == 0) {
        return;
    }
    if (equipment->decorationIds[0] == 0) {
        if (equipment->decorationIds[1] != 0) {
            equipment->decorationIds[0] = equipment->decorationIds[1];
            equipment->decorationIds[1] = 0;
        } else if (equipment->decorationIds[2] != 0) {
            equipment->decorationIds[0] = equipment->decorationIds[2];
            equipment->decorationIds[2] = 0;
        }
    }
    if (equipment->decorationIds[1] == 0 && equipment->decorationIds[2] != 0) {
        equipment->decorationIds[1] = equipment->decorationIds[2];
        equipment->decorationIds[2] = 0;
    }
}

void EquipManager::removeDecoration(EquipmentEntry *equipment, int slot) {
    equipment->decorationIds[slot] = 0;
    compactDecorations(equipment);
}

void EquipManager::copyDecorations(EquipmentEntry *equipment, u16 *out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    for (int i = 0; i < 3; ++i) {
        if (equipment->decorationIds[i] != 0) {
            DecorationInfo *info = decorationInfo(equipment->decorationIds[i]);
            for (int j = 0; j < info->size; ++j) {
                *out++ = equipment->decorationIds[i];
            }
        }
    }
}

u16 EquipManager::defense(EquipmentEntry *equipment) {
    if (equipment->id.id == 0) {
        return 0;
    } else {
        ArmorInfo *info = ARMOR_INFO_TABLES[equipment->id.type];
        info += equipment->id.id;
        u8 level = equipment->upgrades;
        u8 maxLevel = maxArmorLevel(equipment);
        if (level > maxLevel) {
            level = maxLevel;
        }
        return info->defense + level * 2;
    }
}

static inline u8 slotsOf(EquipmentEntry *equipment) {
    u8 type = equipment->id.type;
    if (type == EquipmentType::WEAPON_BLADEMASTER || type == EquipmentType::WEAPON_GUNNER) {
        if (type == EquipmentType::WEAPON_BLADEMASTER) {
            return BLADEMASTER_WEAPON_INFO[equipment->id.id].slots;
        }
        return GUNNER_WEAPON_INFO[equipment->id.id].slots;
    }
    ArmorInfo *info = ARMOR_INFO_TABLES[type];
    info += equipment->id.id;
    return info->slots;
}

u8 EquipManager::slots(EquipmentEntry *equipment) {
    return slotsOf(equipment);
}

int EquipManager::armorSellPrice(EquipmentEntry *equipment) {
    if (equipment->id.id == 0) {
        return 0;
    }
    ArmorInfo *info = ARMOR_INFO_TABLES[equipment->id.type];
    info += equipment->id.id;
    u32 price = info->price;
    int level = maxArmorLevel(equipment);
    if (level != 0) {
        EquipmentEntry upgraded;
        copyEquipmentEntry(&upgraded, equipment);
        upgraded.upgrades = 0;
        while (upgraded.upgrades < equipment->upgrades) {
            price += armorUpgradePrice(&upgraded);
            ++upgraded.upgrades;
            if (upgraded.upgrades > (u8)level) {
                break;
            }
        }
    }
    int result = price / 2;
    result += decorationSellValue(equipment);
    return result;
}

int EquipManager::weaponSellPrice(EquipmentEntry *equipment) {
    if (equipment->id.id == 0) {
        return 0;
    }
    u8 type = EquipManager::objectPtr->weaponType(equipment->id.type, equipment->id.id);
    u32 price;
    if (type == PlType::HEAVY_BOWGUN || type == PlType::LIGHT_BOWGUN) {
        price = GUNNER_WEAPON_INFO[equipment->id.id].price;
        if ((equipment->upgrades & EquipmentUpgrade::LEVEL_MASK) != 0) {
            u16 levels = equipment->upgrades & EquipmentUpgrade::LEVEL_MASK;
            for (int i = 0; i < levels; ++i) {
                price += bowgunLevelUpPrice(equipment->id.type, equipment->id.id, i);
            }
        }
    } else {
        switch (equipment->id.type) {
        case EquipmentType::WEAPON_BLADEMASTER:
            price = BLADEMASTER_WEAPON_INFO[equipment->id.id].price;
            break;
        case EquipmentType::WEAPON_GUNNER:
            price = GUNNER_WEAPON_INFO[equipment->id.id].price;
            break;
        default:
            price = 0;
            break;
        }
    }
    price >>= 1;
    price += decorationSellValue(equipment);
    return price;
}

int EquipManager::decorationSellValue(EquipmentEntry *equipment) {
    if (equipment->id.id == 0) {
        return 0;
    }
    int total = 0;
    for (int i = 0; i < 3; ++i) {
        if (equipment->decorationIds[i] != 0) {
            total += ITEM_DEFINITIONS[equipment->decorationIds[i]].sellValue;
        }
    }
    return total;
}

bool EquipManager::equipmentEquals(EquipmentEntry *a, EquipmentEntry *b) {
    if (a->id.type != b->id.type) {
        return false;
    }
    if (a->id.id != b->id.id) {
        return false;
    }
    if (a->upgrades != b->upgrades) {
        return false;
    }
    if (a->decorationIds[0] != b->decorationIds[0]) {
        return false;
    }
    if (a->decorationIds[1] != b->decorationIds[1]) {
        return false;
    }
    return a->decorationIds[2] == b->decorationIds[2];
}

u8 EquipManager::maxArmorLevel(EquipmentEntry *equipment) {
    if (equipment->id.type >= 5) {
        return 0;
    }
    ArmorInfo *info = ARMOR_INFO_TABLES[equipment->id.type];
    info += equipment->id.id;
    u8 maxLevel = 0;
    if (maxLevel < info->armorSpheres[0]) {
        maxLevel = info->armorSpheres[0];
    }
    if (maxLevel < info->armorSpheres[1]) {
        maxLevel = info->armorSpheres[1];
    }
    if (maxLevel < info->armorSpheres[2]) {
        maxLevel = info->armorSpheres[2];
    }
    if (maxLevel < info->armorSpheres[3]) {
        maxLevel = info->armorSpheres[3];
    }
    if (maxLevel < info->armorSpheres[4]) {
        maxLevel = info->armorSpheres[4];
    }
    if (maxLevel < info->armorSpheres[5]) {
        maxLevel = info->armorSpheres[5];
    }
    if (func_eboot_088579A0(GameSys::objectPtr, 14) == true &&
        maxLevel < info->armorSpheres[6]) {
        maxLevel = info->armorSpheres[6];
    }
    if (maxLevel) {
        --maxLevel;
    }
    if (maxLevel > 0x3F) {
        maxLevel = 0x3F;
    }
    return maxLevel;
}

int EquipManager::armorUpgradeTier(EquipmentEntry *equipment) {
    u8 level = equipment->upgrades + 1;
    if (equipment->id.type >= 5) {
        return 0;
    }
    ArmorInfo *info = ARMOR_INFO_TABLES[equipment->id.type];
    info += equipment->id.id;
    u8 maxLevel = maxArmorLevel(equipment);
    if (maxLevel < level) {
        return -1;
    }
    if (maxLevel != 0) {
        if (level < info->armorSpheres[0]) {
            return 0;
        }
        if (level < info->armorSpheres[1]) {
            return 1;
        }
        if (level < info->armorSpheres[2]) {
            return 2;
        }
        if (level < info->armorSpheres[3]) {
            return 3;
        }
        if (level < info->armorSpheres[4]) {
            return 4;
        }
        if (func_eboot_088579A0(GameSys::objectPtr, 14) == false) {
            return 5;
        }
        if (level < info->armorSpheres[5]) {
            return 5;
        }
        return 6;
    }
    return -1;
}

int EquipManager::armorUpgradePrice(EquipmentEntry *equipment) {
    if (equipment->id.type >= 5) {
        return 0;
    }
    ArmorInfo *info = ARMOR_INFO_TABLES[equipment->id.type];
    info += equipment->id.id;
    float price = info->price;
    s8 tier = armorUpgradeTier(equipment);
    if (tier < 0) {
        return 0;
    }
    return (u32)(price * ARMOR_UPGRADE_COST[tier].rate +
                 ARMOR_UPGRADE_COST[tier].base);
}

void EquipManager::setRecipeFlag(u8 recipeType, u16 recipeId, u8 flag) {
    UserData *userData = &GameSys::objectPtr->userData;
    u8 *flags;
    u16 limit;
    switch (recipeType) {
    case ShopCraftingRecipeType::WEAPONCRAFT:
        flags = userData->weaponRecipeFlags;
        limit = 830;
        break;
    case ShopCraftingRecipeType::ARMORCRAFT:
        flags = userData->armorRecipeFlags;
        limit = 2200;
        break;
    case ShopCraftingRecipeType::DECORATIONCRAFT:
        flags = userData->decorationRecipeFlags;
        limit = 200;
        break;
    default:
        return;
    }
    if (recipeId >= limit) {
        return;
    }
    u8 *byte = &flags[recipeId / 2];
    if ((recipeId & 1) != 0) {
        *byte |= flag << 4;
    } else {
        *byte |= flag;
    }
}

bool EquipManager::checkRecipeFlag(u8 recipeType, u16 recipeId, u8 flag) {
    UserData *userData = &GameSys::objectPtr->userData;
    u8 *flags;
    u16 limit;
    switch (recipeType) {
    case ShopCraftingRecipeType::WEAPONCRAFT:
        flags = userData->weaponRecipeFlags;
        limit = 830;
        break;
    case ShopCraftingRecipeType::ARMORCRAFT:
        flags = userData->armorRecipeFlags;
        limit = 2200;
        break;
    case ShopCraftingRecipeType::DECORATIONCRAFT:
        flags = userData->decorationRecipeFlags;
        limit = 200;
        break;
    default:
        return false;
    }
    if (recipeId >= limit) {
        return false;
    }
    u8 *byte = &flags[recipeId / 2];
    if ((recipeId & 1) != 0) {
        int masked = *byte & (flag << 4);
        if (masked != 0) {
            return true;
        }
    } else {
        int masked = *byte & flag;
        if (masked != 0) {
            return true;
        }
    }
    return false;
}

#pragma opt_unroll_loops on
void EquipManager::clearAllRecipeNewFlags() {
    UserData *userData = &GameSys::objectPtr->userData;
    for (int i = 0; i < 415; ++i) {
        userData->weaponRecipeFlags[i] &= (u8)~(ShopCraftingRecipeFlag::NEW | (ShopCraftingRecipeFlag::NEW << 4));
    }
    for (int i = 0; i < 1100; ++i) {
        userData->armorRecipeFlags[i] &= (u8)~(ShopCraftingRecipeFlag::NEW | (ShopCraftingRecipeFlag::NEW << 4));
    }
    for (int i = 0; i < 100; ++i) {
        userData->decorationRecipeFlags[i] &= (u8)~(ShopCraftingRecipeFlag::NEW | (ShopCraftingRecipeFlag::NEW << 4));
    }
}
#pragma opt_unroll_loops off

int EquipManager::dualWeaponAttributeId(EquipmentEntry *equipment) {
    if (equipment->id.type == EquipmentType::WEAPON_BLADEMASTER) {
        int index = 0;
        DualAttributeWeaponData *entry = DUAL_ATTRIBUTE_WEAPON_DATA;
        while (entry->equipmentId != 0xFFFF) {
            if (entry->equipmentId == equipment->id.id) {
                return index;
            }
            ++entry;
            ++index;
        }
        return -1;
    }
    return -1;
}

DualAttributeWeaponData *EquipManager::dualWeaponAttribute(int index) {
    return &DUAL_ATTRIBUTE_WEAPON_DATA[index];
}
