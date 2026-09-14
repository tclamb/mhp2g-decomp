#pragma once

#include "psptypes.h"
#include "singleton.hpp"
struct HitManager : Singleton<HitManager> {
    // TODO

    HitManager();

    float GetGroundHit(ScePspFVector4 *position);
    bool GetWallHitLineCam(ScePspFVector4 *target, ScePspFVector4 *camera, ScePspFVector4 *out, u16 attr, float dist);
};

extern "C" {
    void func_game_sub_09C336D8(HitManager *, void *);
}
