#pragma once

#include "player.hpp"
#include "singleton.hpp"

struct PlayerManager : Singleton<PlayerManager> {
    // TODO

    Player *getPlayerPtr(int playerId);

    PlayerManager();
};

extern "C" {
    void func_eboot_088DF734(PlayerManager *);
}
