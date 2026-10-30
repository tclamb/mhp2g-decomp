#pragma once

#include "player.hpp"
#include "singleton.hpp"

struct PlayerSlotId {
    enum {
        PL_ID_CAT = 1,
        PL_TRAINER_ID = 5,
        COUNT = 6,
    };
private:
    PlayerSlotId();
};

struct PlayerManager : Singleton<PlayerManager> {
    typedef void (PlayerManager::*mem_fn)();

    mem_fn memFn;
    Player *plPtrTbl[PlayerSlotId::COUNT];
    u32 modelLoadState[PlayerSlotId::COUNT];
    s32 slot;
    u32 *plcom_tbl;
    u32 unused_0x44;

    template <typename T>
    static inline T *allocatePlayer();

    inline bool acquirePlayerModel(u32 playerId, NetPlayerData *playerModel, u32 state);

    PlayerManager();
    void reset();
    void player_mv();
    void draw();
    void initPlayers(SceBool localOnly);
    void settlePlayerDynamics();
    void init();
    void pl_move();
    void player_mk();
    void yure_move();
    void pl_sw_set();
    void acquirePlayerList(Player *player, int id);
    void releasePlayerList(Player *player);
    bool acquirePlayerModel(u8 playerId, u8 sexId, u8 type, u16 modelId, u8 skinId, int clothingId, int isArmor);
    void releaseAllPlayers();
    void releasePlayer(u8 playerId);
    void unloadPlayerParts(u8 playerId, int parts);
    void releasePlayerWeapon(u8 playerId);
    int findEmptyPlayerSlot();
    Player *makePlayer(u32 index);
    void makePlayers();
    void deletePlayer(int index);
    void deleteAllPlayers();
    int loadPlayerModel(u32 playerId);
    void requestLoadWeaponMotion(int playerId);
    void requestLoadWeaponMotions();
    void clearWeaponMotion(int playerId);
    void clearWeaponMotions();
    bool loadPlayerModels();
    int loadPlayerSeBank(int playerId);
    int loadPlayerSeBanks();
    void loadCatPac();
    void clearCatPac();
    void bindCat(PlayerCat *cat);
    void deleteCatHook(Player *player);
    void createCat();
    void deleteCat();
    bool loadCatSeBank(int playerId);
    void demoOn();
    void demoOff();
    Player *getPlayerPtr(int id);
    ~PlayerManager();

protected:
    inline void setMemFn(mem_fn f) {
        if (f != 0) {
            memFn = f;
        }
    }
};
