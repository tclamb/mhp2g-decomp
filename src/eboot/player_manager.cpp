#include "common.h"

#include "data_manager.hpp"
#include "joint.hpp"
#include "game_sys.hpp"
#include "obj_manager.hpp"
#include "pac.hpp"
#include "player_manager.hpp"
#include "shell_manager.hpp"
#include "sound.hpp"
#include "system.hpp"
#include "vfpu.h"

static PlayerManager instance;
template<> PlayerManager *Singleton<PlayerManager>::objectPtr;

template <typename T>
inline T *PlayerManager::allocatePlayer() {
    return cache::pullObjWork<T>(ObjManager::objectPtr);
}

u16 WEAPON_MOTION_FILE_IDS[PlType::COUNT] = {
    FileId::W00_TBL_BIN, FileId::W01_TBL_BIN, FileId::W02_TBL_BIN, FileId::W03_TBL_BIN,
    FileId::W04_TBL_BIN, FileId::W05_TBL_BIN, FileId::W06_TBL_BIN, FileId::W07_TBL_BIN,
    FileId::W08_TBL_BIN, FileId::W09_TBL_BIN, FileId::W10_TBL_BIN,
};

inline bool PlayerManager::acquirePlayerModel(u32 playerId, NetPlayerData *playerModel, u32 state) {
    return acquirePlayerModel(playerId, playerModel->definition.sexId, state, GameSys::objectPtr->playerBodyModelId(playerId, state), GameSys::objectPtr->playerSkinId(playerId), GameSys::objectPtr->playerClothingId(playerId), GameSys::objectPtr->playerHasArmorModel(playerId, state));
}

#pragma opt_unroll_loops on
PlayerManager::PlayerManager() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        plPtrTbl[i] = NULL;
        modelLoadState[i] = 0;
    }

    plcom_tbl = NULL;
    unused_0x44 = 0;
    setMemFn(&PlayerManager::init);
}
#pragma opt_unroll_loops off

#pragma opt_unroll_loops on
void PlayerManager::reset() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        plPtrTbl[i] = NULL;
        modelLoadState[i] = 0;
    }

    slot = 0;
    setMemFn(&PlayerManager::init);
}
#pragma opt_unroll_loops off

void PlayerManager::player_mv() {
    if (memFn != 0) {
        (this->*memFn)();
    }
}

void PlayerManager::draw() {
    ObjManager::objectPtr->registerObjDraw(ObjListId::PLAYER);
}

void PlayerManager::initPlayers(SceBool localOnly) {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        Player *player = getPlayerPtr(i);
        if (player == NULL) {
            continue;
        }

        if (localOnly == false ||
            (localOnly == true &&
             player->stageTransitionType != StageTransitionType::NONE)) {
            player->pl_id = i;
        } else {
            continue;
        }

        player->stageId = GameSys::objectPtr->stage_id;
        player->init_sub();
    }
}

void PlayerManager::settlePlayerDynamics() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        Player *player = getPlayerPtr(i);
        if (player != NULL) {
            player->settleDynamics();
        }
    }
}

void PlayerManager::init() {
    initPlayers(false);

    if (Evdemo::objectPtr->is_requested == 0) {
        pl_move();
    }

    setMemFn(&PlayerManager::pl_move);
}

void PlayerManager::pl_move() {
    pl_sw_set();
    ObjManager::objectPtr->call_vtable_0x14(ObjListId::PLAYER);

    if (GameSys::objectPtr->isLobby() == false
        && GameSys::objectPtr->isArcade() == false
        && GameSys::objectPtr->isCont() == false
        && GameSys::objectPtr->isEdit() == false) {
        ShellManager::objectPtr->shell00_set();
    }

    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        Player *player = plPtrTbl[i];
        if (player != NULL) {
            if ((bool)(player->drawFlags & DrawFlag::ACTIVE) == false) {
                deletePlayer(i);
            }
        }
    }
}

void PlayerManager::player_mk() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        Player *player = plPtrTbl[i];
        if (player != NULL) {
            if ((bool)(player->drawFlags & DrawFlag::ACTIVE) != false) {
                player->player_modify();
            }
        }
    }
}

void PlayerManager::yure_move() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        Player *player = plPtrTbl[i];
        if (player != NULL
            && (bool)(player->drawFlags & DrawFlag::ACTIVE) != false
            && (bool)(player->drawFlags & DrawFlag::DRAW_QUEUED) != false
            && (bool)(player->objFlags & ObjFlag::HAIR_YURE) != false) {
            player->yure_move_hair();
        }
    }
}

void PlayerManager::pl_sw_set() {
    u32 index;
    if (System::objectPtr->Online_ck() == false) {
        index = 0;
    } else {
        index = GameSys::objectPtr->player_id;
    }

    Player *player = plPtrTbl[index];
    if (player != NULL) {
        player->sw_set_sub();
    }
}

void PlayerManager::acquirePlayerList(Player *player, int id) {
    ASSERT(plPtrTbl[id] == NULL);
    plPtrTbl[id] = player;
    player->index = id;
}

void PlayerManager::releasePlayerList(Player *player) {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        if (plPtrTbl[i] == player) {
            plPtrTbl[i] = NULL;
        }
    }
}

bool PlayerManager::acquirePlayerModel(u8 playerId, u8 sexId, u8 type, u16 modelId, u8 skinId, int clothingId, int isArmor) {
    return ObjManager::objectPtr->acquirePlayerModel(playerId, sexId, type, modelId, skinId, clothingId, isArmor);
}

void PlayerManager::releaseAllPlayers() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        if (plPtrTbl[i] != NULL) {
            releasePlayer(i);
        }
    }
}

void PlayerManager::releasePlayer(u8 playerId) {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        ObjManager::objectPtr->releasePlayerModel(playerId, (u8)i);
    }

    ObjManager::objectPtr->releasePlayerWeapon(playerId);
    ObjManager::objectPtr->releasePlayerSkin(playerId);
    modelLoadState[playerId] = 0;
}

void PlayerManager::unloadPlayerParts(u8 playerId, int parts) {
    ASSERT(parts < BodyModelType::COUNT);
    ObjManager::objectPtr->releasePlayerModel(playerId, parts);
}

void PlayerManager::releasePlayerWeapon(u8 playerId) {
    ObjManager::objectPtr->releasePlayerWeapon(playerId);
}

int PlayerManager::findEmptyPlayerSlot() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        if (plPtrTbl[i] == NULL) {
            return i;
        }
    }
    return -1;
}

Player *PlayerManager::makePlayer(u32 index) {
    if (index == (u32)-1) {
        int slot = findEmptyPlayerSlot();
        index = slot;
        if (slot == -1) {
            return NULL;
        }
    }

    Player *result;
    if (GameSys::objectPtr->isLobby() != false) {
        result = allocatePlayer<PlayerLobby>();
    } else if (GameSys::objectPtr->isArcade() == true ||
               GameSys::objectPtr->isCont() == true ||
               GameSys::objectPtr->isEdit() == true) {
        result = allocatePlayer<PlayerCont>();
    } else {
        result = allocatePlayer<PlayerQuest>();
    }

    if (result != NULL) {
        ObjManager::objectPtr->addObj(ObjListId::PLAYER, result);
        ObjManager::objectPtr->bindPlayerModels(result, index);
        ObjManager::objectPtr->bindPlayerWeapon(result, index);

        DataSlotId dataSlot = (DataSlotId)(index + PLAYER_MOTION_SLOT);
        DataManager::DataSlot *entry = DataManager::objectPtr->entries;
        entry += dataSlot;

        u32 *motion;
        if ((entry->status & DataManager::ENABLE) != 0) {
            motion = (u32 *)entry->buffer;
        } else {
            motion = NULL;
        }
        if (motion != NULL) {
            result->hierarchy.motion_table = motion;
        }

        result->pl_id = index;
        result->vtable_0x10();
        result->vtable_0x48();
        result->applyHairColor();
        result->refreshEquipmentColorSeries();
        acquirePlayerList(result, index);
    }

    return result;
}

void PlayerManager::makePlayers() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        if ((bool)GameSys::objectPtr->playerModels[(u8)i].active != false) {
            makePlayer(i);
        }
    }
}

void PlayerManager::deletePlayer(int index) {
    Player *player = plPtrTbl[index];
    releasePlayerList(player);
    plPtrTbl[index] = NULL;

    ObjManager::objectPtr->clearModel(player);
    ObjManager::objectPtr->clearWeapon(player);
    ObjManager::objectPtr->removeObj(player);
    cache::pushObjWork(player, ObjManager::objectPtr);
}

void PlayerManager::deleteAllPlayers() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        if (plPtrTbl[i] != NULL) {
            deletePlayer(i);
        }
    }
}

int PlayerManager::loadPlayerModel(u32 playerId) {
    NetPlayerData *model = &GameSys::objectPtr->playerModels[playerId];

    switch (modelLoadState[playerId]) {
    case 0:
        if (model->active == false) {
            return 1;
        }
        // fallthrough
    case 1:
    case 2:
    case 3:
    case 4:
    case 5: {
        if (acquirePlayerModel(playerId, model, modelLoadState[playerId]) == true) {
            ++modelLoadState[playerId];
        }
        break;
    }
    case 6: {
        u16 weaponModelId = GameSys::objectPtr->GetWeaponModelID(playerId);
        if (ObjManager::objectPtr->acquirePlayerWeapon(playerId, weaponModelId) == true) {
            ++modelLoadState[playerId];
        }
        break;
    }
    case 7: {
        u8 skinId = GameSys::objectPtr->playerSkinId(playerId);
        if (ObjManager::objectPtr->acquirePlayerSkin(playerId, model->definition.sexId, skinId) == true) {
            modelLoadState[playerId] = 0;
            return 1;
        }
        break;
    }
    default:
        return 1;
    }

    return 0;
}

void PlayerManager::requestLoadWeaponMotion(int playerId) {
    ASSERT(!GameSys::objectPtr->isLobby());

    NetPlayerData *playerModel = &GameSys::objectPtr->playerModels[playerId];
    if (playerModel->active == false) {
        return;
    }

    u16 id = GameSys::objectPtr->GetWeaponModelID(playerId);
    u8 type = EquipManager::objectPtr->weaponModelType(id);
    DataManager::objectPtr->load(playerId + PLAYER_MOTION_SLOT, WEAPON_MOTION_FILE_IDS[type], 0);
}

void PlayerManager::requestLoadWeaponMotions() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        requestLoadWeaponMotion(i);
    }
}

void PlayerManager::clearWeaponMotion(int playerId) {
    DataManager::objectPtr->free(playerId + PLAYER_MOTION_SLOT);
}

void PlayerManager::clearWeaponMotions() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        if (plPtrTbl[i] != NULL) {
            clearWeaponMotion(i);
        }
    }
}

bool PlayerManager::loadPlayerModels() {
    if (slot < GameSys::PLAYER_MODEL_COUNT) {
        if (loadPlayerModel(slot) != 0) {
            ++slot;
        }
        return false;
    }

    slot = 0;
    return true;
}

#define WE_ID(n) (FileId::WE##n##_PAC - FileId::WE000_PAC)

int PlayerManager::loadPlayerSeBank(int playerId) {
    NetPlayerData *model = &GameSys::objectPtr->playerModels[playerId];

    switch (modelLoadState[playerId]) {
    case 0: {
        u16 phd[2];
        u16 bd[2];

        if (model->active == 0) {
            return 1;
        }
        if (GameSys::objectPtr->isLobby() != false) {
            modelLoadState[playerId] = 0;
            return 1;
        }

        u32 weaponModelId = GameSys::objectPtr->GetWeaponModelID(playerId);
        phd[0] = PLAYER_WEAPON_SE_TBL[weaponModelId].phd;
        bd[0] = PLAYER_WEAPON_SE_TBL[weaponModelId].bd;

        if (weaponModelId == WE_ID(111) || weaponModelId == WE_ID(170)) {
            if (model->definition.sexId == SexId::MALE) {
                phd[1] = FileId::SND_VO_N01_PHD;
                bd[1] = FileId::SND_VO_N01_BD;
            } else {
                phd[1] = FileId::SND_VO_N00_PHD;
                bd[1] = FileId::SND_VO_N00_BD;
            }
        } else if (weaponModelId == WE_ID(389)) {
            phd[1] = FileId::SND_VO_A00_PHD;
            bd[1] = FileId::SND_VO_A00_BD;
        } else if (weaponModelId == WE_ID(390)) {
            phd[1] = FileId::SND_VO_A01_PHD;
            bd[1] = FileId::SND_VO_A01_BD;
        } else if (weaponModelId == WE_ID(535)) {
            phd[1] = FileId::SND_VO_A02_PHD;
            bd[1] = FileId::SND_VO_A02_BD;
        } else if (weaponModelId == WE_ID(536)) {
            phd[1] = FileId::SND_VO_A03_PHD;
            bd[1] = FileId::SND_VO_A03_BD;
        } else {
            phd[1] = PLAYER_VOICE_SE_TBL[model->definition.sexId][model->definition.voiceId].phd;
            bd[1] = PLAYER_VOICE_SE_TBL[model->definition.sexId][model->definition.voiceId].bd;
        }

        Sound::objectPtr->loadPlayerSeBank(playerId + BankId::PLAYER_OFFSET, phd, bd, FileId::SND_WEAPON_TSB, 2);
        ++modelLoadState[playerId];
        break;
    }
    case 1:
        if (Sound::objectPtr->load_busy_ck() == 0) {
            modelLoadState[playerId] = 0;
        } else {
            return 0;
        }
        // fallthrough
    default:
        return 1;
    }

    return 0;
}

#undef WE_ID

int PlayerManager::loadPlayerSeBanks() {
    if (slot < GameSys::PLAYER_MODEL_COUNT) {
        if (loadPlayerSeBank(slot) != 0) {
            ++slot;
        }
        return 0;
    }

    slot = 0;
    return 1;
}

void PlayerManager::loadCatPac() {
    DataManager::objectPtr->load(CAT_PAC_SLOT, FileId::CAT_00_PAC, 0);
}

void PlayerManager::clearCatPac() {
    DataManager::objectPtr->free(CAT_PAC_SLOT);
}

void PlayerManager::bindCat(PlayerCat *player) {
    DataSlotId slot = CAT_PAC_SLOT;

    u32 meshCount;
    pmo_header *pmo;
    pmo_header *pmo2;
    pmo_mesh_data *meshData;
    pac_header *pac2;
    u32 meshSize;
    void *pmoBuffer;
    u32 pmoSize;
    u32 pacSize;
    JointFileBlock *jointBlock;
    u32 jointCount;
    void *jointBuffer;
    tmh_header *tmhHeader;
    void *tmhBuffer;
    u8 *pacAdrs;
    u8 *dataAdrs;

    pacAdrs = DataManager::objectPtr->getAdrs(slot);
    ASSERT(pacAdrs != NULL);

    pac_header *pac = (pac_header *)pacAdrs;
    dataAdrs = pac->data(1);
    ASSERT(dataAdrs != NULL);
    pmo = (pmo_header *)dataAdrs;
    meshCount = pmo->mesh_count;
    pmoSize = pmo->material_count() * sizeof(pmo_material_data);
    pmoSize += meshCount * sizeof(pmo_mesh_lighting);
    pmoBuffer = ObjManager::objectPtr->cache.alloc(pmoSize, 0x10);

    pac2 = (pac_header *)DataManager::objectPtr->getAdrs(slot);
    pacSize = pac2->size(1);
    pmo2 = (pmo_header *)pac2->data(1);
    meshData = (pmo_mesh_data *)ObjManager::objectPtr->getPlayerFixVram(PlayerSlotId::PL_ID_CAT, 0);
    meshSize = pmo2->mesh_data_size();
    sceKernelDcacheWritebackInvalidateAll();
    sceDmacMemcpy(meshData, pmo2->mesh_data(), meshSize);
    player->bind_pmo(pmoBuffer, pmo, meshData);

    dataAdrs = pac->data(0);
    ASSERT(dataAdrs != NULL);
    jointBlock = (JointFileBlock *)dataAdrs;
    jointCount = jointBlock->count;
    if ((jointBlock->flags & JointFileBlock::JOINT_FILE_LAST) != 0) {
        --jointCount;
    }
    jointBuffer = ObjManager::objectPtr->cache.alloc(jointCount * sizeof(Joint), 0x10);
    Hierarchy::flGetHierarchy3(&player->hierarchy, jointBuffer, jointBlock, player->model_pmo.scale.x, player->model_pmo.scale.y, player->model_pmo.scale.z);

    dataAdrs = pac->data(2);
    ASSERT(dataAdrs != NULL);
    tmhHeader = (tmh_header *)dataAdrs;
    tmhBuffer = ObjManager::objectPtr->cache.alloc(tmhHeader->picture_count * sizeof(tmh_fragment),
                                                   0x10);
    player->bind_tmh(tmhBuffer, tmhHeader);

    dataAdrs = pac->data(3);
    ASSERT(dataAdrs != NULL);
    player->hierarchy.motion_table = (u32 *)dataAdrs;
}

// TODO figure out shape for weak scope
#define JOINT_PLAYER_MANAGER_IMPL
#include "joint.hpp"
#undef JOINT_PLAYER_MANAGER_IMPL

void PlayerManager::deleteCatHook(Player *player) {
    // empty
}

void PlayerManager::createCat() {
    ASSERT(PlayerManager::objectPtr->getPlayerPtr(PlayerSlotId::PL_ID_CAT) == NULL);
    PlayerCat *pl = allocatePlayer<PlayerCat>();
    ASSERT(pl != NULL);

    ObjManager::objectPtr->addObj(ObjListId::PLAYER, pl);
    bindCat(pl);
    pl->pl_id = PlayerSlotId::PL_ID_CAT;
    pl->vtable_0x10();
    acquirePlayerList(pl, PlayerSlotId::PL_ID_CAT);
}

void PlayerManager::deleteCat() {
    Player *player = plPtrTbl[PlayerSlotId::PL_ID_CAT];
    releasePlayerList(player);
    plPtrTbl[PlayerSlotId::PL_ID_CAT] = NULL;
    deleteCatHook(player);

    ObjManager::objectPtr->removeObj(player);
    cache::pushObjWork(player, ObjManager::objectPtr);
}

bool PlayerManager::loadCatSeBank(int playerId) {
    int catWeaponId = (s16)((PlayerCat *)plPtrTbl[PlayerSlotId::PL_ID_CAT])->catWeaponId();
    u16 phd[2];
    u16 bd[2];

    switch (modelLoadState[playerId]) {
    case 0:
        phd[0] = FileId::SND_NPCAILU_PHD;
        bd[0] = FileId::SND_NPCAILU_BD;
        phd[1] = CAT_WEAPON_SE_TBL[catWeaponId].phd;
        bd[1] = CAT_WEAPON_SE_TBL[catWeaponId].bd;
        Sound::objectPtr->loadPlayerSeBank(playerId + BankId::PLAYER_OFFSET, phd, bd, FileId::SND_NPCAILU_TSB, 2);
        ++modelLoadState[playerId];
        break;
    case 1:
        if (Sound::objectPtr->load_busy_ck() == 0) {
            modelLoadState[playerId] = 0;
        } else {
            return false;
        }
        // fallthrough
    default:
        return true;
    }

    return false;
}

ScePspRGBA8888 RAINBOW_COLORS[8] = {
    0xFFED7EE3,
    0xFFFF9FC1,
    0xFFF1CC9A,
    0xFF85E579,
    0xFF66F3BD,
    0xFF63ECF5,
    0xFF5C8DF0,
    0xFFA178E8,
};

u32 EquipmentColor::rainbow() {
    int index = (GameSys::objectPtr->frame_counter >> 4) & 7;
    int nextIndex = (index + 1) & 7;
    ScePspUnion32 *current = (ScePspUnion32 *)&RAINBOW_COLORS[index];
    ScePspUnion32 *next = (ScePspUnion32 *)&RAINBOW_COLORS[nextIndex];
    float t = (float)(GameSys::objectPtr->frame_counter & 0xF) * 0.0625f;
    float u = 1.0f - t;
    ScePspUnion32 result;

    float channel = current->uc[0] * u + next->uc[0] * t;
    if (channel > 255.0f) {
        channel = 255.0f;
    }
    result.uc[0] = (u8)channel;

    channel = current->uc[1] * u + next->uc[1] * t;
    if (channel > 255.0f) {
        channel = 255.0f;
    }
    result.uc[1] = (u8)channel;

    channel = current->uc[2] * u + next->uc[2] * t;
    if (channel > 255.0f) {
        channel = 255.0f;
    }
    result.uc[2] = (u8)channel;
    result.uc[3] = 0xFF;

    return result.ui;
}

void PlayerManager::demoOn() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        Player *player = getPlayerPtr(i);
        if (player != NULL) {
            player->demo_on_timer = 10;
        }
    }
}

void PlayerManager::demoOff() {
    for (int i = 0; i < PlayerSlotId::COUNT; ++i) {
        Player *player = getPlayerPtr(i);
        if (player != NULL) {
            player->demo_on_timer = 0;
        }
    }

    PlayerQuest *player = (PlayerQuest *)getPlayerPtr(GameSys::objectPtr->player_id);
    if (player->act_ck(PlActionType::EGG, 0) == true) {
        player->Pl_act_set(PlActionType::EGG, 0, 0);
    } else {
        player->pl_to_normal(0, 0, 0);
    }
    player->plFlags &= ~PlFlag::TOOLBELT_OPEN;
}

Player *PlayerManager::getPlayerPtr(int id) {
    if (id < 0 || id >= PlayerSlotId::COUNT) {
        return NULL;
    }
    return plPtrTbl[id];
}

PlayerManager::~PlayerManager() {
    // empty
}

#include "player_manager.skin.cpp"
#include "player_manager.yure.cpp"
#include "player_manager.joint_map.cpp"
#include "player_manager.material_masks.cpp"
#include "player_manager.skill_thresholds.cpp"

u8 HUNTING_HORN_SUBTYPE_NOTE_IDS[20][3] = {
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::BLUE, HuntingHornNoteId::RED},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::YELLOW, HuntingHornNoteId::RED},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::GREEN, HuntingHornNoteId::RED},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::BLUE, HuntingHornNoteId::YELLOW},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::GREEN, HuntingHornNoteId::YELLOW},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::GREEN, HuntingHornNoteId::BLUE},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::BLUE, HuntingHornNoteId::RED},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::YELLOW, HuntingHornNoteId::RED},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::BLUE, HuntingHornNoteId::YELLOW},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::GREEN, HuntingHornNoteId::YELLOW},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::GREEN, HuntingHornNoteId::BLUE},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::GREEN, HuntingHornNoteId::RED},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::GREEN, HuntingHornNoteId::SKY_BLUE},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::YELLOW, HuntingHornNoteId::SKY_BLUE},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::SKY_BLUE, HuntingHornNoteId::BLUE},
    {HuntingHornNoteId::WHITE, HuntingHornNoteId::SKY_BLUE, HuntingHornNoteId::RED},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::GREEN, HuntingHornNoteId::SKY_BLUE},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::YELLOW, HuntingHornNoteId::SKY_BLUE},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::SKY_BLUE, HuntingHornNoteId::BLUE},
    {HuntingHornNoteId::PURPLE, HuntingHornNoteId::SKY_BLUE, HuntingHornNoteId::RED},
};

ScePspRGBA8888 S_SERIES_COLORS[12] = {
    0xFFFFFFFF,
    0xFF262827,
    0xFF15148D,
    0xFF7459FA,
    0xFF1F57FF,
    0xFF1093E6,
    0xFF2A4F29,
    0xFF96AF38,
    0xFF352525,
    0xFF74592A,
    0xFF4F2653,
    0xFFFFFFFF,
};
