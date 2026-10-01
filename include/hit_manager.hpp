#pragma once

#include "common.h"
#include "singleton.hpp"

struct HitManager : Singleton<HitManager> {
    // TODO

    HitManager();

    float GetGroundHit(ScePspFVector4 *position);
    bool GetWallHitLine(ScePspFVector4 *target, ScePspFVector4 *position, ScePspFVector4 *out, u16 attr);
    bool GetWallHitLineCam(ScePspFVector4 *target, ScePspFVector4 *camera, float distance, ScePspFVector4 *out, u16 attr);
    bool cmGetGroundHitLine(ScePspFVector4 *camera, ScePspFVector4 *target, ScePspFVector4 *out);
    void k_HitEmCamera(ScePspFVector4 *position);
    void k_HitWallCamera(float *distance_out, ScePspFVector4 *position, ScePspFVector4 *previous_position);
};

extern "C" {
    void func_game_sub_09C336D8(HitManager *, void *);
}
