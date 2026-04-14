#pragma once

#include "singleton.hpp"
struct HitManager : Singleton<HitManager> {
    // TODO

    HitManager();
};

extern "C" {
    void func_game_sub_09C336D8(HitManager *, void *);
}
