// Player HUD aim markers (player_089B6378, the player subclass with weapon animations).
// Both functions project a point 1000 units ahead of an aim direction to the screen with the
// camera's world-to-screen (func_eboot_08815434: viewport 240/136 + offsets, so the result is in
// 480x272 PSP pixels) and, when the point is in front of the camera (w > 0), draw a 2D marker there
// through the Cockpit in render slot 9 (Cockpit layer 9, SystemFont layer 0), colour 0xF0FF0000.
// Callers: 0x09A6274C (joint version) and 0x09A626A0 (angle version), both in player_089B6378.
#include "common.h"
#include "cockpit.hpp"
#include "camera.hpp"
#include "system_font.hpp"
#include "vfpu.h"

extern "C" ScePspFMatrix4 *func_eboot_08865F60(void *obj, int joint);
extern "C" void func_eboot_08815434(Camera *cam, ScePspFVector4 *screen, ScePspFVector4 *world);
extern "C" void func_eboot_08832A68(Cockpit *c, int x, int y, u32 color);

// Aim along joint 14 of the player model: the joint's -Z axis, 1000 units out from the joint.
extern "C" void func_game_task_09A69D30(void *this_) {
    ScePspFVector4 p;
    ScePspFVector4 dir;
    ScePspFVector4 screen;
    p.x = 0.0f;
    p.y = 0.0f;
    p.z = -1000.0f;
    flvecApplyMat33(&dir, &p, func_eboot_08865F60(this_, 14));
    ScePspFMatrix4 *m = func_eboot_08865F60(this_, 14);
    p.x = m->w.x;
    p.y = m->w.y;
    p.z = m->w.z;
    p.x += dir.x;
    p.y += dir.y;
    p.z += dir.z;
    func_eboot_08815434(Camera::objectPtr, &screen, &p);
    if (screen.w > 0.0f) {
        Cockpit *c = Cockpit::objectPtr;
        c->renderGroup = 9;
        SystemFont::objectPtr->setLayer(0);
        c->method_08826600();
        func_eboot_08832A68(Cockpit::objectPtr, (int)screen.x, (int)screen.y, 0xF0FF0000);
    }
}

struct AimOwner {
    u8 pad_0x0[0x1F4];
    int pitch;                 // +0x1F4, 65536 units per turn
    u8 pad_0x1F8[0x200 - 0x1F8];
    ScePspFVector4 position;   // +0x200
    u8 pad_0x210[0x680 - 0x210];
    s16 yaw;                   // +0x680, 65536 units per turn
};

// Aim from angles: rotation (-yaw, pitch, 0) applied to (0, 0, 1000), from 100 units above the
// player's position.
extern "C" void func_game_task_09A69E50(AimOwner *this_) {
    ScePspFMatrix4 rot;
    ScePspFVector4 v;
    ScePspFVector4 screen;
    v.y = 0.0f;
    v.x = 0.0f;
    v.z = 1000.0f;
    vmidt_q(&rot);
    eulerRotation(&rot, &rot, 9.58738e-05f * (float)(-this_->yaw), 9.58738e-05f * (float)this_->pitch, 0.0f);
    flvecApplyMat33(&v, &v, &rot);
    v.x += this_->position.x;
    v.y += 100.0f + this_->position.y;
    v.z += this_->position.z;
    func_eboot_08815434(Camera::objectPtr, &screen, &v);
    if (screen.w > 0.0f) {
        Cockpit *c = Cockpit::objectPtr;
        c->renderGroup = 9;
        SystemFont::objectPtr->setLayer(0);
        c->method_08826600();
        func_eboot_08832A68(Cockpit::objectPtr, (int)screen.x, (int)screen.y, 0xF0FF0000);
    }
}
