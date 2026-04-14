#pragma once

#include "player.hpp"
#include "singleton.hpp"

struct PlayerManager : Singleton<PlayerManager> {
    // TODO

    Player *method_088DF804(int playerId);

    PlayerManager();
};
