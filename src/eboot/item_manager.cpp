#include "item_manager.hpp"

#include "game_sys.hpp"
#include "player_manager.hpp"
#include "system.hpp"

#define ASSERT(...)

template<> ItemManager *Singleton<ItemManager>::objectPtr;

#include "item_manager.item_definitions.inc.cpp"
#include "item_manager.mix_definitions.inc.cpp"

void ItemManager::setBagItem(u16 itemId) {
    setItemBit(itemId);
}

u16 ItemManager::bagAdd(u16 itemId, s16 addend) {
    int result;
    UserData &data = GameSys::objectPtr->userData;
    if (ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
        addend = 0xFF;
    }
    if (bagQuantity(itemId) == 0) {
        result = 5;
        for (s16 i = 0; i < 24; ++i) {
            if (data.inventory[i].itemId == 0 && addend > 0) {
                s16 stackSize = ITEM_DEFINITIONS[itemId].stackSize;
                data.inventory[i].itemId = itemId;
                if (stackSize < addend) {
                    addend = stackSize;
                }
                data.inventory[i].quantity = addend;
                setBagItem(itemId);
                result = 0;
                break;
            }
        }
    } else {
        for (s16 i = 0; i < 24; ++i) {
            if (data.inventory[i].itemId == itemId) {
                s16 stackSize = ITEM_DEFINITIONS[itemId].stackSize;
                if (addend > 0 && data.inventory[i].quantity >= stackSize) {
                    data.inventory[i].quantity = stackSize;
                    setBagItem(itemId);
                    result = 3;
                    break;
                }
                if (addend < 0 && ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
                    result = 1;
                    break;
                }
                data.inventory[i].quantity += addend;
                if (data.inventory[i].quantity <= 0) {
                    bagRemove(i);
                    data.inventory[i].itemId = 0;
                    data.inventory[i].quantity = 0;
                    result = 4;
                } else {
                    if (data.inventory[i].quantity > stackSize) {
                        data.inventory[i].quantity = stackSize;
                        result = 2;
                    } else {
                        result = 1;
                    }
                    setBagItem(itemId);
                }
                break;
            }
        }
    }
    return result;
}

s16 ItemManager::bagQuantity(u16 itemId) {
    for (s16 i = 0; i < 24; ++i) {
        UserData &userData = GameSys::objectPtr->userData;
        if (userData.inventory[i].itemId == itemId) {
            return userData.inventory[i].quantity;
        }
    }
    return 0;
}

s16 ItemManager::bagSpace(u16 itemId) {
    UserData &userData = GameSys::objectPtr->userData;
    s16 i;
    s16 emptySlots = 0;
    for (i = 0; i < 24; ++i) {
        if (userData.inventory[i].itemId == itemId) {
            int result = ITEM_DEFINITIONS[itemId].stackSize;
            if (result == 0xFF) {
                return 0xFF;
            } else {
                result -= userData.inventory[i].quantity;
            }
            return result;
        }
        if (userData.inventory[i].itemId == 0) {
            ++emptySlots;
        }
    }
    if (emptySlots) {
        return ITEM_DEFINITIONS[itemId].stackSize;
    } else {
        return -1;
    }
}

// result 0 = already empty, 1 = success, 2 = invalid index
s16 ItemManager::bagRemove(s16 index) {
    UserData &userData = GameSys::objectPtr->userData;

    ASSERT(index >= 0);
    if (index >= 24) {
        return 2;
    }

    userData.inventory[index].quantity = 0;
    if (userData.inventory[index].itemId == 0) {
        return 0;
    }
    userData.inventory[index].itemId = 0;

    return 1;
}

u16 ItemManager::knownMixCount(u8 filter) {
    UserData &userData = GameSys::objectPtr->userData;

    Player *player = PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id);

    if (!filter) {
        // test for alchemy
        if (canMix(player, NULL, false) != true) {
            filter = CombinationCountFilter::IGNORE_ALCHEMY;
        } else {
            filter = CombinationCountFilter::ANY;
        }
    }

    u16 result = 0;
    MixDefinition *mix = &MIX_DEFINITIONS[0];
    for (u32 i = 0xAC; i != 0; --i, ++mix) {
        if ((userData.mixMask[mix->id >> 3] & (1 << (mix->id & 0x7))) != 0) {
            if (filter == CombinationCountFilter::IGNORE_ALCHEMY) {
                if ((mix->flags & MixFlag::ALCHEMY) == 0) {
                    ++result;
                }
            } else if (filter == CombinationCountFilter::ONLY_ALCHEMY) {
                if ((mix->flags & MixFlag::ALCHEMY) == 1) {
                    ++result;
                }
            } else {
                ++result;
            }
        }
    }
    return result;
}

bool ItemManager::canMix(Player *player, MixDefinition *mix, bool isItemBox) {
    if (mix != NULL && (mix->flags & MixFlag::ALCHEMY) == 0) {
        return true;
    } else if (player->activeSkill(SkillId::ALCHEMY) == true) {
        return true;
    } else if (GameSys::objectPtr->bagQuantity(ItemId::ALCHEMY_GUIDE) != 0) {
        return true;
    } else if (isItemBox == true && boxFind(ItemId::ALCHEMY_GUIDE) != 0xFFFF) {
        return true;
    }
    return false;
}

s32 ItemManager::transportItemId(u16 itemId) {
    switch (itemId) {
    case ItemId::WYVERN_EGG:
    case ItemId::HERBIVORE_EGG:
    case ItemId::CANNON_S:
    case ItemId::QUARTZ_ORE:
    case ItemId::SOOTSTONE_ORE:
    case ItemId::POWDERSTONE:
    case ItemId::CARNIVORE_EGG:
    case ItemId::GIADROME_JEWEL:
    case ItemId::KUT_KU_JEWEL:
    case ItemId::DAIMYO_JEWEL:
    case ItemId::CONGALALA_JEWEL:
    case ItemId::RATHALOS_JEWEL:
    case ItemId::GRAVIOS_JEWEL:
    case ItemId::ELDR_DRGN_TEARS:
    case ItemId::LAO_SHAN_MELON:
    case ItemId::ELDR_DRGN_FOSSIL:
    case ItemId::GYPCEROS_CRYSTAL:
    case ItemId::RATHIAN_JEWEL:
    case ItemId::TEOSTRA_METEOR:
    case ItemId::FROZEN_MEATBALL:
    case ItemId::BUCKSHOT_ACORN:
    case ItemId::CEPHADROME_MELON:
    case ItemId::GOLDEN_COCOON:
    case ItemId::AMBER_EGG:
    case ItemId::GRAVISCUS_BULB:
    case ItemId::HARD_DRAGONROCK:
    case ItemId::DRAGONROCK_PLUS:
    case ItemId::DRAGONROCK:
    case ItemId::CENTURY_WALNUT:
    case ItemId::THSAND_YEAR_FOREST:
    case ItemId::GARUGA_JEWEL:
        return itemId;
    }
    return -1;
}

bool ItemManager::isMixIngredient(s16 _itemId, bool isItemBox, int filter) {
    int itemId = _itemId;

    MixDefinition *mix;
    int n;

    MixIndexEntry *idx = &MIX_INDEX[itemId];
    if (idx->count != 0) {
        mix = &MIX_DEFINITIONS[idx->firstId];
        for (n = idx->count; n > 0; --n, ++mix) {
            switch (filter) {
            case 1:
                return true;
            case 0:
                if (canMix(PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id), mix, isItemBox) == true) {
                    return true;
                }
                break;
            case -1:
                if ((mix->flags & MixFlag::ALCHEMY) == 0) {
                    return true;
                }
                break;
            }
        }
    }

    for (int otherId = 0; otherId < itemId; ++otherId) {
        MixIndexEntry *idx = &MIX_INDEX[otherId];
        if (idx->count != 0) {
            mix = &MIX_DEFINITIONS[idx->firstId];
            for (n = idx->count; n > 0; --n, ++mix) {
                if (mix->secondMaterialItemId == _itemId) {
                    switch (filter) {
                    case 1:
                        return true;
                    case 0:
                        if (canMix(PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id), mix, isItemBox) == true) {
                            return true;
                        }
                        break;
                    case -1:
                        if ((mix->flags & MixFlag::ALCHEMY) == 0) {
                            return true;
                        }
                        break;
                    }
                }
            }
        }
    }
    return false;
}

MixDefinition *ItemManager::findMix(s16 firstMaterialItemId, s16 secondMaterialItemId, int flags, bool isItemBox, int filter) {
    if (firstMaterialItemId > secondMaterialItemId) {
        s16 tmp = secondMaterialItemId;
        secondMaterialItemId = firstMaterialItemId;
        firstMaterialItemId = tmp;
    }
    MixIndexEntry &entry = MIX_INDEX[firstMaterialItemId];
    if (entry.count == 0) {
        return NULL;
    }
    MixDefinition *mix = MIX_DEFINITIONS + entry.firstId;
    for (int i = entry.count; i != 0; --i, mix++) {
        if (mix->secondMaterialItemId == secondMaterialItemId) {
            if ((flags & MixFlag::ALCHEMY) != 0) {
                return mix;
            }
            switch (filter) {
            case 1:
                return mix;
            case 0:
                if (canMix(PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id), mix, isItemBox) == true) {
                    return mix;
                }
                break;
            case -1:
                if ((mix->flags & MixFlag::ALCHEMY) == 0) {
                    return mix;
                }
                break;
            }
        }
    }
    return NULL;
}

bool ItemManager::getMixBit(MixDefinition *mix, int flags, bool isItemBox) {
    if ((flags & MixFlag::ALCHEMY) == 0) {
        if (canMix(PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id), mix, isItemBox) != true) {
            return false;
        }
    }
    if ((GameSys::objectPtr->userData.mixMask[mix->id >> 3] & (1 << (mix->id & 0x7))) == 0) {
        return false;
    }
    return true;
}

bool ItemManager::getMixBit(s16 firstMaterialItemId, s16 secondMaterialItemId) {
    MixDefinition *mix = findMix(firstMaterialItemId, secondMaterialItemId, 0, false, 0);
    if (mix == NULL) {
        return false;
    }
    if ((GameSys::objectPtr->userData.mixMask[mix->id >> 3] & (1 << (mix->id & 0x7))) == 0) {
        return false;
    }
    return true;
}

s16 ItemManager::combine(u8 *quantityOut, Player *player, s16 firstMaterialItemId, s16 secondMaterialItemId, int isItemBox) {
    MixDefinition *mix = findMix(firstMaterialItemId, secondMaterialItemId, 0, isItemBox == true, 0);
    if (mix == NULL) {
        return ItemId::INVALID_ID;
    }

    s16 resultItemId;
    s16 successThreshold = mixRate(firstMaterialItemId, secondMaterialItemId, isItemBox);
    if (System::objectPtr->next_index(1) % 100 < successThreshold) {
        resultItemId = mix->successItemId;
        if (player->activeSkill(SkillId::BULLET_LIMIT) == true) {
            *quantityOut = MIX_MAX_QUANTITIES[mix->quantityId];
        } else {
            MixQuantity *q = MIX_QUANTITIES[mix->quantityId];
            *quantityOut = q->minimum;
            u8 *p = q->percents;
            int roll = System::objectPtr->next_index(1) % 100;
            loop: if (roll >= *p) {
                *quantityOut += 1; roll -= *p++;
                goto loop;
            }
        }
    } else {
        *quantityOut = 0;
        resultItemId = ItemId::INVALID_ID;
    }

    if (!isItemBox) {
        player->bagAdd(firstMaterialItemId, -1, false);
        player->bagAdd(secondMaterialItemId, -1, false);
    }
    return resultItemId;
}

s16 ItemManager::boxRemove(u16 itemId, s16 quantity) {
    UserData *userData = &GameSys::objectPtr->userData;

    if (quantity < 0) {
        return 0;
    }

    if (itemId == 0 || ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
        return 0;
    }

    s16 boxSize = GameSys::objectPtr->itemBoxSize();
    s16 remaining = quantity;
    for (s16 i = 0; i < boxSize; ++i) {
        if (userData->itemBox[i].itemId == itemId) {
            if (userData->itemBox[i].quantity >= remaining) {
                userData->itemBox[i].quantity -= remaining;
                remaining = 0;
            } else {
                remaining -= userData->itemBox[i].quantity;
                userData->itemBox[i].quantity = 0;
            }
            if (userData->itemBox[i].quantity < 1) {
                userData->itemBox[i].itemId = 0;
            }
            if (remaining < 1) {
                return 0;
            }
        }
    }
    return remaining;
}

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", boxRemove__11ItemManagerFUssUs);

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", boxFind__11ItemManagerFUs);

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", mixRate__11ItemManagerFssi);

extern "C" u8 func_eboot_088D9410(Player *player, int skillType);
extern "C" int func_eboot_088D939C(Player *player, int skillType);

extern "C" s8 mixRate__11ItemManagerFP13MixDefinitionb(ItemManager *this_, MixDefinition *mix, int isItemBox) {
    bool box = isItemBox == true ? true : false;
    Player *player = PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id);
    if (this_->canMix(player, mix, box) != true) {
        return -1;
    }
    if ((mix->flags & MixFlag::BOMB) != 0) {
        if (player->activeSkill(SkillId::BOMBER) == true) {
            return 100;
        }
    }
    if ((mix->flags & MixFlag::TRAP) != 0) {
        if (player->activeSkill(SkillId::TRAP_MASTER) == true) {
            return 100;
        }
    }

    u8 rate = MIX_RATES[mix->rateId];
    switch (func_eboot_088D9410(player, 0x3B)) {
    case SkillId::COMBINE_SUCCESS_PLUS_15_PERCENT:
        rate += 15;
        break;
    case SkillId::COMBINE_SUCCESS_PLUS_25_PERCENT:
        rate += 25;
        break;
    case SkillId::COMBINE_SUCCESS_PLUS_45_PERCENT:
        rate += 45;
        break;
    case SkillId::COMBINE_SUCCESS_MINUS_5_PERCENT:
        rate -= 5;
        break;
    case SkillId::COMBINE_SUCCESS_MINUS_10_PERCENT:
        rate -= 10;
        break;
    case SkillId::COMBINE_SUCCESS_MINUS_15_PERCENT:
        rate -= 15;
        break;
    }
    if (func_eboot_088D939C(player, 0xF)) {
        rate += 10;
    }
    if (func_eboot_088D939C(player, 0x10)) {
        rate += 5;
    }

    u32 i;
    if (!isItemBox) {
        for (i = 0; i < 5; i++) {
            if (GameSys::objectPtr->bagQuantity(COMBO_BOOK_IDS[i]) == 0) {
                break;
            }
        }
        rate += COMBO_BOOK_BONUSES[i];
    }
    if (isItemBox == true) {
        for (i = 0; i < 5; i++) {
            bool found = false;
            if (GameSys::objectPtr->bagQuantity(COMBO_BOOK_IDS[i]) != 0) {
                found = true;
            }
            if ((bool)(GameSys::objectPtr->flags_0x6AF14 & 1) == true) {
                if (this_->boxFind(COMBO_BOOK_IDS[i]) != 0xFFFF) {
                    found = true;
                }
            }
            if (found == false) {
                break;
            }
        }
        rate += COMBO_BOOK_BONUSES[i];
    }
    if (rate > 100) {
        rate = 100;
    }
    return rate;
}

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", setMixBit__11ItemManagerFP13MixDefinition);

u8 ItemManager::maxMixQuantity(MixDefinition *mix) {
    return MIX_MAX_QUANTITIES[mix->quantityId];
}

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", nextMix__11ItemManagerFPUccPUsPUsb);

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", boxStackQuantity__11ItemManagerFUs);

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", boxTotalQuantity__11ItemManagerFUs);

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", boxTotalSpace__11ItemManagerFUs);

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", boxStackSpace__11ItemManagerFUs);

ItemDefinition *ItemManager::bagDefinition(int index) {
    return &ITEM_DEFINITIONS[GameSys::objectPtr->method_088567AC(index)->itemId];
}

void ItemManager::setItemBit(u16 itemId) {
    GameSys::objectPtr->userData.setItemBit(itemId);
}

bool ItemManager::getItemBit(u16 itemId) {
    return GameSys::objectPtr->userData.getItemBit(itemId);
}

void sort(InventoryEntry *entries, u32 count) {
    int h = 1;
    while (h < count) {
        h = h * 3 + 1;
    }
    for (h /= 3; h > 0; h /= 3) {
        for (u32 i = h; i < count; i++) {
            u16 itemId = entries[i].itemId;
            s16 quantity = entries[i].quantity;
            int j;
            for (j = i - h; j >= 0; j -= h) {
                if (entries[j].itemId > itemId || (entries[j].itemId == itemId && entries[j].quantity < quantity)) {
                    entries[j + h] = entries[j];
                } else {
                    break;
                }
            }
            entries[j + h].itemId = itemId;
            entries[j + h].quantity = quantity;
        }
    }
}

int ItemManager::isDecoration(u16 itemId) {
    return ITEM_DEFINITIONS[itemId].decorationId != 0;
}

u8 mixRateColorId(s8 mixRate) {
    u8 i;
    for (i = 0; i < 6; i++) {
        if (mixRate >= MIX_RATES[i]) {
            break;
        }
    }
    return i;
}

INCLUDE_ASM("asm/eboot/nonmatchings/item_manager", search__11ItemManagerFPUsiP16ItemSearchResult);

s16 ItemManager::convertMixId2nd(u16 mixId) {
    return MIX_DEFINITIONS[mixId].oldCombinationId;
}
