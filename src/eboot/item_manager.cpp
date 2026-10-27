#include "item_manager.hpp"

#include "game_sys.hpp"
#include "player_manager.hpp"
#include "system.hpp"

template<> ItemManager *Singleton<ItemManager>::objectPtr;

#include "item_manager.item_definitions.inc.cpp"
#include "item_manager.mix_definitions.inc.cpp"

#define MIX_DEFINITION_COUNT ((int)(sizeof(MIX_DEFINITIONS) / sizeof(MIX_DEFINITIONS[0])))
#define MIX_RATE_COUNT ((int)(sizeof(MIX_RATES) / sizeof(MIX_RATES[0])))
#define COMBO_BOOK_COUNT ((int)(sizeof(COMBO_BOOK_IDS) / sizeof(COMBO_BOOK_IDS[0])))

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
        for (s16 i = 0; i < ITEM_BAG_CAPACITY; ++i) {
            if (data.itemBag[i].itemId == ItemId::NONE && addend > 0) {
                s16 stackSize = ITEM_DEFINITIONS[itemId].stackSize;
                data.itemBag[i].itemId = itemId;
                if (stackSize < addend) {
                    addend = stackSize;
                }
                data.itemBag[i].quantity = addend;
                setBagItem(itemId);
                result = 0;
                break;
            }
        }
    } else {
        for (s16 i = 0; i < ITEM_BAG_CAPACITY; ++i) {
            if (data.itemBag[i].itemId == itemId) {
                s16 stackSize = ITEM_DEFINITIONS[itemId].stackSize;
                if (addend > 0 && data.itemBag[i].quantity >= stackSize) {
                    data.itemBag[i].quantity = stackSize;
                    setBagItem(itemId);
                    result = 3;
                    break;
                }

                if (addend < 0 && ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
                    result = 1;
                    break;
                }

                data.itemBag[i].quantity += addend;
                if (data.itemBag[i].quantity <= 0) {
                    bagRemove(i);
                    data.itemBag[i].itemId = ItemId::NONE;
                    data.itemBag[i].quantity = 0;
                    result = 4;
                } else {
                    if (data.itemBag[i].quantity > stackSize) {
                        data.itemBag[i].quantity = stackSize;
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
    for (s16 i = 0; i < ITEM_BAG_CAPACITY; ++i) {
        UserData &userData = GameSys::objectPtr->userData;
        if (userData.itemBag[i].itemId == itemId) {
            return userData.itemBag[i].quantity;
        }
    }
    return 0;
}

s16 ItemManager::bagSpace(u16 itemId) {
    UserData &userData = GameSys::objectPtr->userData;
    s16 i;
    s16 emptySlots = 0;
    for (i = 0; i < ITEM_BAG_CAPACITY; ++i) {
        if (userData.itemBag[i].itemId == itemId) {
            int result = ITEM_DEFINITIONS[itemId].stackSize;
            if (result == 0xFF) {
                return 0xFF;
            } else {
                result -= userData.itemBag[i].quantity;
            }
            return result;
        }

        if (userData.itemBag[i].itemId == ItemId::NONE) {
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
s16 ItemManager::bagRemove(s16 inventoryIndex) {
    UserData &userData = GameSys::objectPtr->userData;

    if (inventoryIndex >= ITEM_BAG_CAPACITY) {
        return 2;
    }

    userData.itemBag[inventoryIndex].quantity = 0;
    if (userData.itemBag[inventoryIndex].itemId == ItemId::NONE) {
        return 0;
    }
    userData.itemBag[inventoryIndex].itemId = ItemId::NONE;

    return 1;
}

u16 ItemManager::knownMixCount(u8 mixCountFilter) {
    UserData &userData = GameSys::objectPtr->userData;

    Player *player = PlayerManager::objectPtr->getPlayerPtr(GameSys::objectPtr->player_id);

    if (!mixCountFilter) {
        // test for alchemy
        if (canMix(player, NULL, InventoryType::BAG) != true) {
            mixCountFilter = MixCountFilter::IGNORE_ALCHEMY;
        } else {
            mixCountFilter = MixCountFilter::ANY;
        }
    }

    u16 result = 0;
    MixDefinition *mix = &MIX_DEFINITIONS[0];
    for (u32 i = MIX_DEFINITION_COUNT; i != 0; --i, ++mix) {
        if ((userData.mixMask[mix->id >> 3] & (1 << (mix->id & 0x7))) != 0) {
            if (mixCountFilter == MixCountFilter::IGNORE_ALCHEMY) {
                if ((mix->flags & MixFlag::ALCHEMY) == 0) {
                    ++result;
                }
            } else if (mixCountFilter == MixCountFilter::ONLY_ALCHEMY) {
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
    } else if (isItemBox == true && boxFind(ItemId::ALCHEMY_GUIDE) != ItemId::INVALID_ID) {
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

bool ItemManager::isMixIngredient(s16 itemId, bool isItemBox, int mixFilter) {
    int id = itemId;

    MixDefinition *mix;
    int n;

    MixIndexEntry *idx = &MIX_INDEX[id];
    if (idx->count != 0) {
        mix = &MIX_DEFINITIONS[idx->firstId];
        for (n = idx->count; n > 0; --n, ++mix) {
            switch (mixFilter) {
            case MixFilter::ANY:
                return true;
            case MixFilter::DEFAULT:
                if (canMix(PlayerManager::objectPtr->getPlayerPtr(GameSys::objectPtr->player_id), mix, isItemBox) == true) {
                    return true;
                }
                break;
            case MixFilter::NO_ALCHEMY:
                if ((mix->flags & MixFlag::ALCHEMY) == 0) {
                    return true;
                }
                break;
            }
        }
    }

    for (int otherId = 0; otherId < id; ++otherId) {
        MixIndexEntry *idx = &MIX_INDEX[otherId];
        if (idx->count != 0) {
            mix = &MIX_DEFINITIONS[idx->firstId];
            for (n = idx->count; n > 0; --n, ++mix) {
                if (mix->secondMaterialItemId == itemId) {
                    switch (mixFilter) {
                    case MixFilter::ANY:
                        return true;
                    case MixFilter::DEFAULT:
                        if (canMix(PlayerManager::objectPtr->getPlayerPtr(GameSys::objectPtr->player_id), mix, isItemBox) == true) {
                            return true;
                        }
                        break;
                    case MixFilter::NO_ALCHEMY:
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

MixDefinition *ItemManager::findMix(s16 firstMaterialItemId, s16 secondMaterialItemId, int mixFlags, bool isItemBox, int mixFilter) {
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
            if ((mixFlags & MixFlag::ALCHEMY) != 0) {
                return mix;
            }
            switch (mixFilter) {
            case MixFilter::ANY:
                return mix;
            case MixFilter::DEFAULT:
                if (canMix(PlayerManager::objectPtr->getPlayerPtr(GameSys::objectPtr->player_id), mix, isItemBox) == true) {
                    return mix;
                }
                break;
            case MixFilter::NO_ALCHEMY:
                if ((mix->flags & MixFlag::ALCHEMY) == 0) {
                    return mix;
                }
                break;
            }
        }
    }
    return NULL;
}

bool ItemManager::getMixBit(MixDefinition *mix, int mixFlags, bool isItemBox) {
    if ((mixFlags & MixFlag::ALCHEMY) == 0) {
        if (canMix(PlayerManager::objectPtr->getPlayerPtr(GameSys::objectPtr->player_id), mix, isItemBox) != true) {
            return false;
        }
    }
    if ((GameSys::objectPtr->userData.mixMask[mix->id >> 3] & (1 << (mix->id & 0x7))) == 0) {
        return false;
    }
    return true;
}

bool ItemManager::getMixBit(s16 firstMaterialItemId, s16 secondMaterialItemId) {
    MixDefinition *mix = findMix(firstMaterialItemId, secondMaterialItemId, 0, false, MixFilter::DEFAULT);
    if (mix == NULL) {
        return false;
    }

    if ((GameSys::objectPtr->userData.mixMask[mix->id >> 3] & (1 << (mix->id & 0x7))) == 0) {
        return false;
    }

    return true;
}

s16 ItemManager::combine(u8 *quantityOut, Player *player, s16 firstMaterialItemId, s16 secondMaterialItemId, int inventoryType) {
    MixDefinition *mix = findMix(firstMaterialItemId, secondMaterialItemId, 0, inventoryType == InventoryType::BOX, MixFilter::DEFAULT);
    if (mix == NULL) {
        return ItemId::INVALID_ID;
    }

    s16 resultItemId;
    s16 successThreshold = mixRate(firstMaterialItemId, secondMaterialItemId, inventoryType);
    if (System::objectPtr->next_index(1) % 100 < successThreshold) {
        resultItemId = mix->successItemId;
        if (player->activeSkill(SkillId::BULLET_LIMIT) == true) {
            *quantityOut = MIX_MAX_QUANTITIES[mix->quantityId];
        } else {
            MixQuantity *q = MIX_QUANTITIES[mix->quantityId];
            *quantityOut = q->minimum;
            u8 *p = q->percents;
            int roll = System::objectPtr->next_index(1) % 100;
            while (true) {
                if (roll < *p) {
                    break;
                }
                *quantityOut += 1;
                roll -= *p++;
            }
        }
    } else {
        *quantityOut = 0;
        resultItemId = ItemId::INVALID_ID;
    }

    if (inventoryType == InventoryType::BAG) {
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

    if (itemId == ItemId::NONE || ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
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
                userData->itemBox[i].itemId = ItemId::NONE;
            }

            if (remaining < 1) {
                return 0;
            }
        }
    }
    return remaining;
}

s16 ItemManager::boxRemove(u16 itemId, s16 quantity, u16 itemBoxIndex) {
    UserData *userData = &GameSys::objectPtr->userData;
    if (quantity < 0) {
        return 0;
    }
    if (itemId == ItemId::NONE || ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
        return 0;
    }
    s16 boxSize = GameSys::objectPtr->itemBoxSize();
    s16 remaining = quantity;
    if (itemBoxIndex != ItemId::INVALID_ID) {
        if (userData->itemBox[itemBoxIndex].quantity != 0) {
            if (userData->itemBox[itemBoxIndex].itemId == itemId) {
                if (userData->itemBox[itemBoxIndex].quantity >= quantity) {
                    userData->itemBox[itemBoxIndex].quantity -= quantity;
                    remaining = 0;
                } else {
                    remaining -= userData->itemBox[itemBoxIndex].quantity;
                    userData->itemBox[itemBoxIndex].quantity = 0;
                }

                if (userData->itemBox[itemBoxIndex].quantity < 1) {
                    userData->itemBox[itemBoxIndex].itemId = ItemId::NONE;
                }

                if (remaining >= 1) {
                    remaining = boxRemove(itemId, remaining, ItemId::INVALID_ID);
                }
            } else {
                remaining = boxRemove(itemId, quantity, ItemId::INVALID_ID);
            }
        } else {
            remaining = boxRemove(itemId, quantity, ItemId::INVALID_ID);
        }
    } else {
        for (s16 i = boxSize - 1; i >= 0; --i) {
            if (userData->itemBox[i].itemId != itemId) {
                continue;
            }

            if (userData->itemBox[i].quantity >= remaining) {
                userData->itemBox[i].quantity -= remaining;
                remaining = 0;
            } else {
                remaining -= userData->itemBox[i].quantity;
                userData->itemBox[i].quantity = 0;
            }

            if (userData->itemBox[i].quantity < 1) {
                userData->itemBox[i].itemId = ItemId::NONE;
            }

            if (remaining < 1) {
                return 0;
            }
        }
    }
    return remaining;
}

u16 ItemManager::boxFind(u16 itemId) {
    UserData &userData = GameSys::objectPtr->userData;
    if (itemId != ItemId::INVALID_ID) {
        for (int index = 0; index < GameSys::objectPtr->itemBoxSize(); ++index) {
            if (userData.itemBox[index].itemId == itemId && userData.itemBox[index].quantity > 0) {
                return index;
            }
        }
    }
    return ItemId::INVALID_ID;
}

s8 ItemManager::mixRate(s16 firstMaterialItemId, s16 secondMaterialItemId, int inventoryType) {
    MixDefinition *mix = findMix(firstMaterialItemId, secondMaterialItemId, inventoryType == InventoryType::BOX, false, MixFilter::DEFAULT);
    if (mix == NULL) {
        return -1;
    }
    return mixRate(mix, inventoryType);
}

s8 ItemManager::mixRate(MixDefinition *mix, int inventoryType) {
    bool boxFlag = (inventoryType == InventoryType::BOX) ? true : false;
    Player *player = PlayerManager::objectPtr->getPlayerPtr(GameSys::objectPtr->player_id);
    if (canMix(player, mix, boxFlag) != true) {
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

    u8 skillType = player->Pl_Skill_type_ck(SkillType::MIX_SUCCESS_RATE);
    switch (skillType) {
    case SkillId::COMBINE_SUCCESS_PLUS_15_PERCENT:  rate += 15; break;
    case SkillId::COMBINE_SUCCESS_PLUS_25_PERCENT:  rate += 25; break;
    case SkillId::COMBINE_SUCCESS_PLUS_45_PERCENT:  rate += 45; break;
    case SkillId::COMBINE_SUCCESS_MINUS_5_PERCENT:  rate -= 5;  break;
    case SkillId::COMBINE_SUCCESS_MINUS_10_PERCENT: rate -= 10; break;
    case SkillId::COMBINE_SUCCESS_MINUS_15_PERCENT: rate -= 15; break;
    }

    if (player->Pl_PlateSkill_ck(KitchenSkillId::FELYNE_COMBINE_HI)) {
        rate += 10;
    }

    if (player->Pl_PlateSkill_ck(KitchenSkillId::FELYNE_COMBINE_LO)) {
        rate += 5;
    }

    u32 n;
    if (inventoryType == false) {
        for (n = 0; n < COMBO_BOOK_COUNT; ++n) {
            if (GameSys::objectPtr->bagQuantity(COMBO_BOOK_IDS[n]) == 0) {
                break;
            }
        }
        rate += COMBO_BOOK_BONUSES[n];
    }

    if (inventoryType == true) {
        for (n = 0; n < COMBO_BOOK_COUNT; ++n) {
            bool owned = false;
            if (GameSys::objectPtr->bagQuantity(COMBO_BOOK_IDS[n]) != 0) {
                owned = true;
            }

            if (GameSys::objectPtr->isLobby() == true) {
                if (boxFind(COMBO_BOOK_IDS[n]) != ItemId::INVALID_ID) {
                    owned = true;
                }
            }

            if (owned == false) {
                break;
            }
        }
        rate += COMBO_BOOK_BONUSES[n];
    }

    if (rate > 100) {
        rate = 100;
    }
    return rate;
}

void ItemManager::setMixBit(MixDefinition *mix) {
    GameSys::objectPtr->userData.mixMask[mix->id >> 3] |= 1 << (mix->id & 0x7);
}

u8 ItemManager::maxMixQuantity(MixDefinition *mix) {
    return MIX_MAX_QUANTITIES[mix->quantityId];
}

MixDefinition *ItemManager::nextMix(u8 *mixId, char step, u16 *firstMaterialItemId, u16 *secondMaterialItemId, int mixFlags) {
    int n;
    s16 id = *mixId;
    u16 *materials;

    if (step == 0) {
        materials = MIX_MATERIAL_ITEM_IDS[id];
        MixDefinition *mix = findMix(materials[0], materials[1], mixFlags, false, MixFilter::DEFAULT);
        if (mix != NULL) {
            *firstMaterialItemId = materials[0];
            *secondMaterialItemId = materials[1];
            return mix;
        }
        step = 1;
    }

    for (n = MIX_DEFINITION_COUNT; n != 0; --n) {
        id += step;
        if (id > MIX_DEFINITION_COUNT - 1) {
            id = 0;
        } else if (id < 0) {
            id = MIX_DEFINITION_COUNT - 1;
        }
        materials = MIX_MATERIAL_ITEM_IDS[id];
        MixDefinition *mix = findMix(materials[0], materials[1], mixFlags, false, MixFilter::DEFAULT);
        if (mix != NULL) {
            *mixId = id;
            *firstMaterialItemId = materials[0];
            *secondMaterialItemId = materials[1];
            return mix;
        }
    }
    return NULL;
}

int ItemManager::boxStackQuantity(u16 itemId) {
    UserData &userData = GameSys::objectPtr->userData;
    s16 size = GameSys::objectPtr->itemBoxSize();
    for (int i = 0; i < size; ++i) {
        if (userData.itemBox[i].itemId == itemId) {
            if (ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
                return 999;
            }
            return userData.itemBox[i].quantity;
        }
    }
    return 0;
}

int ItemManager::boxTotalQuantity(u16 itemId) {
    UserData &userData = GameSys::objectPtr->userData;
    s16 size = GameSys::objectPtr->itemBoxSize();
    int total = 0;
    for (int i = 0; i < size; ++i) {
        if (userData.itemBox[i].itemId == itemId) {
            if (ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
                return 999;
            }
            total += userData.itemBox[i].quantity;
        }
    }
    return total;
}

int ItemManager::boxTotalSpace(u16 itemId) {
    UserData &userData = GameSys::objectPtr->userData;
    if (itemId == ItemId::NONE || ITEM_DEFINITIONS[itemId].stackSize == 0xFF) {
        return 0;
    }
    s16 size = GameSys::objectPtr->itemBoxSize();
    int total = 0;
    for (int i = 0; i < size; ++i) {
        if (userData.itemBox[i].itemId == itemId) {
            total += 99 - userData.itemBox[i].quantity;
        } else if (userData.itemBox[i].itemId == ItemId::NONE) {
            total += 99;
        }
    }
    return total;
}

int ItemManager::boxStackSpace(u16 itemId) {
    UserData &userData = GameSys::objectPtr->userData;
    if (itemId == ItemId::NONE) {
        return -1;
    }
    InventoryEntry *entry;
    int n = GameSys::objectPtr->itemBoxSize();
    int max = ITEM_DEFINITIONS[itemId].stackSize;
    int space = max;
    if (max != 0xFF) {
        max = 99;
        space = 99;
    }
    entry = userData.itemBox;
    for (; n > 0; --n, ++entry) {
        if (entry->itemId == ItemId::NONE) {
            return max;
        }

        if (entry->itemId == itemId && max != 0xFF) {
            space -= max - entry->quantity;
            if (space < 1) {
                return max;
            }
        }
    }
    return max - space;
}

ItemDefinition *ItemManager::bagDefinition(int inventoryIndex) {
    return &ITEM_DEFINITIONS[GameSys::objectPtr->method_088567AC(inventoryIndex)->itemId];
}

void ItemManager::setItemBit(u16 itemId) {
    GameSys::objectPtr->userData.setItemBit(itemId);
}

bool ItemManager::getItemBit(u16 itemId) {
    return GameSys::objectPtr->userData.getItemBit(itemId);
}

void sort(InventoryEntry *entries, u32 count) {
    int pivot = 1;
    while (pivot < count) {
        pivot = pivot * 3 + 1;
    }
    pivot /= 3;

    while (pivot > 0) {
        for (u32 i = pivot; i < count; ++i) {
            int j = i - pivot;
            u16 itemId = entries[i].itemId;
            s16 quantity = entries[i].quantity;
            while (j >= 0) {
                InventoryEntry *other = &entries[j];
                if (other->itemId > itemId ||
                    (itemId == other->itemId && other->quantity < quantity)) {
                    entries[j + pivot] = *other;
                    j -= pivot;
                } else {
                    break;
                }
            }
            entries[j + pivot].itemId = itemId;
            entries[j + pivot].quantity = quantity;
        }
        pivot /= 3;
    }
}

int ItemManager::isDecoration(u16 itemId) {
    return ITEM_DEFINITIONS[itemId].decorationId != 0;
}

u8 mixRateColorId(s8 mixRate) {
    u8 colorId;
    for (colorId = 0; colorId < MIX_RATE_COUNT - 1; ++colorId) {
        if (mixRate >= MIX_RATES[colorId]) {
            break;
        }
    }
    return colorId;
}

int ItemManager::search(u16 *itemIds, s32 itemCount, ItemSearchResult *out) {
    int count = 0;
    for (int i = 0; i < itemCount; ++i) {
        s16 bag = ItemManager::objectPtr->bagQuantity(itemIds[i]);
        int box = ItemManager::objectPtr->boxTotalQuantity(itemIds[i]);
        if (bag + box > 0) {
            out[count].itemId = itemIds[i];
            out[count].bagQuantity = bag;
            out[count].boxQuantity = box;
            ++count;
        }
    }
    return count;
}

u8 ItemManager::convertMixId2nd(u16 mixId) {
    return MIX_DEFINITIONS[mixId].mixId2nd;
}
