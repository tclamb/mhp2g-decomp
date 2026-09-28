// The culling tests really return bool (no andi before the xori); the shared
// header keeps the int declarations its callers were matched against.
#define func_eboot_08816E20 func_eboot_08816E20_hdr
#define func_eboot_08816EA8 func_eboot_08816EA8_hdr
#include "camera.hpp"
#undef func_eboot_08816E20
#undef func_eboot_08816EA8

#include "game_sys.hpp"
#include "player_manager.hpp"
#include "vfpu.h"
#include "ge.hpp"
#include "pad.hpp"

template<> Camera *Singleton<Camera>::objectPtr;

// Raw float field of a `Camera *camera` local (offsets not yet in camera.hpp).
#define CAM_F(o) (*(float *)((u8 *)camera + (o)))

Camera::Camera() {
    zClipping = true;
    unknown_0xB70 = 1808;
    unknown_0xB74 = 1912;
    unknown_0xC = DEGREES_TO_VFPU(78.5);
    unknown_0x10 = -999;
    unknown_0xB34 = 0;
}

ScePspFVector4 D_eboot_089310B0 = {0, 0, -50, 0};
ScePspFVector4 D_eboot_089310C0 = {0, 1, 0, 0};

inline void *inline_memset(void *dst, int val, u32 size) {
    u8 *p = (u8 *)dst;
    u32 n = size;
    if (p) {
        while (n != 0) {
            *p++ = val;
            --n;
        }
    }
    return dst;
}

void Camera::method_088137C8() {
    ScePspFVector4 p, q, r;
    p = D_eboot_089310B0;
    inline_memset(&q, 0, sizeof(q));
    r = D_eboot_089310C0;

    Ge::objectPtr->norm = 1;
    near_z = 30;
    far_z = 65000;
    unknown_0xC = DEGREES_TO_VFPU(78.5);
    unknown_0x8 = (float)(30.0 / 17.0);
    method_08817024();
    method_08815028(&p, &q, &r);
    unknown_0xDC0 = 0;
    enableCameraControls = 0;
    method_0881395C();
    unknown_0x14 = 3;
    next_demo_id = 0;
    demo_enemy = 0;
}

void Camera::method_088138DC() {
    unknown_0xB10 = 0;
    unknown_0xB3C = 1;
    method_0881395C();
    unknown_0xB10 = 0;
    player = PlayerManager::objectPtr->method_088DF804(GameSys::objectPtr->player_id);
    unknown_0xB00 = 0;
    unknown_0xAE0 = 0;
    unknown_0xAC0 = 0;
    unknown_0xA81 = 0;
    unknown_0xA8A = 0;
    unknown_0xA88 = 0;
    demo_enemy = 0;
    next_demo_id = 0;
    method_08815744();
    method_08813990();
}

void Camera::method_0881395C() {
    near_z = 30;
    far_z = 65000;
    unknown_0xC = DEGREES_TO_VFPU(78.5);
    unknown_0x8 = (float)(30.0 / 17.0);
    unknown_0xB34 = 0;
    method_08816108();
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", method_08813990__6CameraFv);

extern s16 D_eboot_089310D8[8];
extern float D_eboot_089310E8[3];

// One camera shake channel (camera+0xAB0/0xAD0/0xAF0, 0x20 bytes each).
struct CameraShake {
    ScePspFVector4 anchor; // world position the falloff is measured from
    u8 active;             // 0x10
    u8 duration;           // 0x11: index into D_eboot_089310D8 | 0x80 = no falloff
    s16 timer;             // 0x12: frames left
    u8 range;              // 0x14: index into D_eboot_089310E8 (2000/3000/4000)
};

// Vertical camera shake: offset = A * sin(2*pi * (90 * timer / duration) / 360)
// * falloff, A = 10 (duration index < 2) or 12; falloff = 1 - dist(anchor, eye)
// / range, 0 beyond range (1 with flag 0x80); the sign flips every 2 frames
// (timer bit 1). Added to eye.y (0xB44) and target.y (0xB54).
extern "C" void func_eboot_08813B78(Camera *camera, CameraShake *e) {
    if (e->active) {
        float t = 90.0f * ((float)e->timer / (float)D_eboot_089310D8[e->duration & 0x7F]);
        float amp;
        if ((e->duration & 0x7F) < 2) amp = 10.0f;
        else amp = 12.0f;
        amp *= vsin_s(6.2831855f * (t / 360.0f));
        float falloff;
        if (e->duration & 0x80) {
            falloff = 1.0f;
        } else {
            float dist = flvecCalcDistance(&e->anchor, (ScePspFVector4 *)((u8 *)camera + 0xB40));
            float range = D_eboot_089310E8[e->range];
            if (dist >= range) falloff = 0.0f; else falloff = 1.0f - dist / range;
        }
        amp *= falloff;
        if ((e->timer >> 1) & 1) amp = -amp;
        CAM_F(0xB44) += amp;
        CAM_F(0xB54) += amp;
    }
}

extern s16 D_eboot_089310D8[8];

extern "C" void func_eboot_08813CD8(Camera *camera, int index,
                                      ScePspFVector4 *position) {
    u8 *p = (u8 *)camera;
    p[0xAC0] = 1;
    p[0xAC1] = index;
    *(s16 *)(p + 0xAC2) = D_eboot_089310D8[index];
    float *out = (float *)(p + 0xAB0);
    out[0] = position->x;
    out[1] = position->y;
    out[2] = position->z;
    out[3] = position->w;
    p[0xAC4] = 0;
}

extern "C" void func_eboot_08813D24(Camera *camera, int index) {
    u8 *p = (u8 *)camera;
    p[0xAC0] = 1;
    p[0xAC1] = index | 0x80;
    *(s16 *)(p + 0xAC2) = D_eboot_089310D8[index];
}

extern "C" void func_eboot_08813D50(Camera *camera, int index, ObjBase *object) {
    if ((object->inLoadedStage() & 0xFF) == 0) return;
    u8 *p = (u8 *)camera;
    p[0xAC0] = 1;
    p[0xAC1] = index;
    *(s16 *)(p + 0xAC2) = D_eboot_089310D8[index];
    float *out = (float *)(p + 0xAB0);
    float *in = (float *)((u8 *)object + 0x200);
    out[0] = in[0]; out[1] = in[1]; out[2] = in[2]; out[3] = in[3];
    p[0xAC4] = 0;
}

extern "C" void func_eboot_08813DE4(Camera *camera, int index, ObjBase *object,
                                      u8 flag) {
    if ((object->inLoadedStage() & 0xFF) == 0) return;
    u8 *p = (u8 *)camera;
    p[0xAC0] = 1;
    p[0xAC1] = index;
    *(s16 *)(p + 0xAC2) = D_eboot_089310D8[index];
    float *out = (float *)(p + 0xAB0);
    float *in = (float *)((u8 *)object + 0x200);
    out[0] = in[0]; out[1] = in[1]; out[2] = in[2]; out[3] = in[3];
    p[0xAC4] = flag;
}

extern "C" void func_eboot_08813E84(Camera *camera, int index, ObjBase *object) {
    if ((object->inLoadedStage() & 0xFF) == 0) return;
    u8 *p = (u8 *)camera;
    p[0xAE0] = 1;
    p[0xAE1] = index;
    *(s16 *)(p + 0xAE2) = D_eboot_089310D8[index];
    float *out = (float *)(p + 0xAD0);
    float *in = (float *)((u8 *)object + 0x200);
    out[0] = in[0]; out[1] = in[1]; out[2] = in[2]; out[3] = in[3];
    p[0xAE4] = 0;
}

extern "C" int func_eboot_08813F18(Camera *camera) {
    u8 *p = (u8 *)camera;
    if (*(s8 *)(p + 0xDC4) >= 0) return 0;
    if (camera->base_sub_type != 0) return 0;
    return p[0x1DE] != 0;
}

extern "C" void func_eboot_08813F48(Camera *camera, void *source) {
    u16 value = *(u16 *)((u8 *)source + 0x1E6);
    u8 target = *(u8 *)((u8 *)GameSys::objectPtr + 0x28);
    int match = (value == target);
    if (match == 1) camera->unknown_0xA81 = 1;
}

extern "C" void func_eboot_08813F78(Camera *camera, void *source) {
    u8 *p = (u8 *)camera;
    u8 *s = (u8 *)source;
    u16 value = *(u16 *)(s + 0x1E6);
    u8 target = *(u8 *)((u8 *)GameSys::objectPtr + 0x28);
    int match = (value == target);
    if (match == 1) {
        p[0x1D9] = 0;
        *(s16 *)(p + 0x1D0) = *(u32 *)(s + 0x1F4) + 0x2000;
        p[0x14] = 2;
    }
}

// Starts/retargets the zoom camera. flag 1 restarts the animation (state 2,
// operationFrame 15). With an NPC the stack is reset to it (target type 0,
// position = npc+0x200); without one the current entry keeps its type and
// gets the default framing: type 4 -> yCenter 160 / zSpacing 120, else
// 120 / 300. FOV pi/6 rad, 15-frame zoom.
extern "C" void func_eboot_08813FBC(Camera *camera, Npc *npc, u8 flag) {
    ZoomCameraData *z = &camera->subCameras[SubCameraType::PLAYER_EX].data.playerEX.zoom_cam;
    if (flag == 1) {
        z->state.byte = 0;
        z->animationState = 2;
        z->operationFrame = 15;
    } else {
        if (z->animationState == 2) z->state.byte = 0;
        z->animationState = 1;
    }
    if (npc != NULL) {
        z->state.byte = 0;
        z->stackSize = 0;
        z->npcs[z->stackSize] = npc;
        z->fieldOfView = 0.5235988f;
        z->animationTotalFrames = 15;
        z->targetTypes[z->stackSize] = 0;
        copy_q(&z->position, (ScePspFVector4 *)((u8 *)npc + 0x200));
    } else {
        z->npcs[z->stackSize] = npc;
        z->fieldOfView = 0.5235988f;
        z->animationTotalFrames = 15;
        switch (z->targetTypes[z->stackSize]) {
        case 4:
            z->yCenters[z->stackSize] = 160.0f;
            z->zSpacings[z->stackSize] = 120.0f;
            break;
        default:
            z->yCenters[z->stackSize] = 120.0f;
            z->zSpacings[z->stackSize] = 300.0f;
            break;
        }
    }
}

// Pushes a zoom-camera target of type 3 (player-EX sub-camera, zoom_cam at
// 0x7E0): animationState 1, FOV pi/6 rad, 15-frame zoom.
extern "C" void func_eboot_088140E0(Camera *camera, float yCenter, float zSpacing) {
    ZoomCameraData *z = &camera->subCameras[SubCameraType::PLAYER_EX].data.playerEX.zoom_cam;
    z->state.byte = 0;
    z->animationState = 1;
    z->npcs[z->stackSize] = 0;
    z->fieldOfView = 0.5235988f;
    z->targetTypes[z->stackSize] = 3;
    z->yCenters[z->stackSize] = yCenter;
    z->zSpacings[z->stackSize] = zSpacing;
    z->animationTotalFrames = 15;
}

extern float D_eboot_089310D0;

// Ends a zoom-camera step (animationState 3). pop != 0 pops the stack and
// either animates back to the previous entry (15 frames) or clears it;
// otherwise a type-5 entry that is still animating gets its duration from the
// distance left: 0.075 * (|pos - positions[i]| + |tgt - targets[i]|) + 1.
extern "C" void func_eboot_08814148(Camera *camera, int pop) {
    SubCamera *s = &camera->subCameras[SubCameraType::PLAYER_EX];
    ZoomCameraData *z = &s->data.playerEX.zoom_cam;
    z->animationState = 3;
    z->animationTotalFrames = 15;
    if (pop) {
        if (--z->stackSize != 0) {
            z->state.byte = 1;
            z->animationState = 2;
            z->operationFrame = 15;
            z->animationTotalFrames = 15;
        } else {
            z->state.byte = 0;
            z->animationState = 0;
            z->operationFrame = 0;
        }
    } else {
        int i = z->stackSize - 1;
        if (z->targetTypes[i] == 5 && z->operationFrame != 0) {
            float d1 = flvecCalcDistance(&s->current_position, &z->positions[i]);
            float d2 = flvecCalcDistance(&s->current_target, &z->targets[i]);
            s16 f = (s16)(D_eboot_089310D0 * (d1 + d2)) + 1;
            z->operationFrame = f;
            z->animationTotalFrames = f;
        }
    }
}

// True when the zoom camera's animationState (0x810) is 1 or 2.
extern "C" bool func_eboot_08814258(Camera *camera) {
    u8 m = *((u8 *)camera + 0x810);
    return m == 1 || m == 2;
}

// True when the zoom camera's animationState (0x810) is 1 or 3.
extern "C" bool func_eboot_08814280(Camera *camera) {
    u8 m = *((u8 *)camera + 0x810);
    return m == 1 || m == 3;
}

// Pushes a zoom-camera target of type 2 with an explicit position, direction
// and FOV; flag 1 restarts the animation (state 2, operationFrame 15).
extern "C" void func_eboot_088142A8(Camera *camera, ScePspFVector4 *position,
                                      ScePspFVector4 *direction, float *fov, u8 flag) {
    ZoomCameraData *z = &camera->subCameras[SubCameraType::PLAYER_EX].data.playerEX.zoom_cam;
    copy_q(&z->position, position);
    copy_q(&z->direction, direction);
    z->fieldOfView = *fov;
    if (flag == 1) {
        z->state.byte = 0;
        z->animationState = 2;
        z->operationFrame = 15;
    } else {
        z->animationState = 1;
    }
    z->npcs[z->stackSize] = 0;
    z->targetTypes[z->stackSize] = 2;
}

// Forwards to 08814148 (a1 = pop passes through unchanged).
extern "C" void func_eboot_08814318(Camera *camera, int pop) {
    func_eboot_08814148(camera, pop);
}

extern "C" void func_eboot_08814320(Camera *camera, u8 mode, void *subject, u8 flags) {
    camera->zClipping = false;
    camera->next_demo_id = mode;
    camera->demo_enemy = (Enemy *)subject;
    camera->padding_0xA74[6] = flags;
}

extern "C" void func_eboot_08814334(Camera *camera, u8 mode, void *subject) {
    camera->zClipping = false;
    if (camera->next_demo_id == 0) {
        camera->next_demo_id = mode;
        camera->demo_enemy = (Enemy *)subject;
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814354);

extern "C" void func_eboot_08814354(Camera *, int);

extern "C" void func_eboot_088148F4(Camera *camera) {
    func_eboot_08814354(camera, 1);
}

extern "C" int func_eboot_08814A4C(Camera *camera);
extern "C" void func_eboot_08814B6C(Camera *camera);

// Starts the monster-intro demo camera for `enemy`: clears a running demo
// (08814A4C == 1 -> 08814B6C), then maps the monster id (enemy+0x1E8) to a
// demo mode 0xB..0x12 (ids 2/0x24/0x47 pick 0xD or 0xC from enemy+0x280).
// Other ids start nothing.
extern "C" void func_eboot_088148FC(Camera *camera, void *enemy) {
    if ((u8)func_eboot_08814A4C(camera) == 1) func_eboot_08814B6C(camera);
    u8 mode;
    switch (*((u8 *)enemy + 0x1E8)) {
    case 0x7:
    case 0x32:
        mode = 0xB;
        break;
    case 0x2:
    case 0x24:
    case 0x47:
        if (*((u8 *)enemy + 0x280) != 2) mode = 0xC; else mode = 0xD;
        break;
    case 0x37:
        mode = 0xE;
        break;
    case 0x36:
    case 0x3C:
        mode = 0xF;
        break;
    case 0x40:
    case 0x41:
        mode = 0x11;
        break;
    case 0x3B:
        mode = 0x10;
        break;
    case 0x4C:
    case 0x58:
        mode = 0x12;
        break;
    default:
        return;
    }
    func_eboot_08814320(camera, mode, enemy, 0);
}

extern "C" int func_eboot_08814A4C(Camera *camera) {
    int mode = *(u8 *)((u8 *)camera + 0x9BE);
    switch (mode) {
    case 3:
    case 4:
    case 5:
        return 1;
    default:
        return 0;
    }
}

extern "C" void func_eboot_08814A88(Camera *camera) {
    if (camera->base_sub_type != 2) {
        func_eboot_08814320(camera, 2, 0, 0);
    }
}

// Starts demo mode 1 unless the GameSys byte 0x6AF10 is 12.
// (A switch, not an if: the call then keeps `jal; nop`.)
extern "C" void func_eboot_08814AB8(Camera *camera) {
    switch (*((u8 *)GameSys::objectPtr + 0x6AF10)) {
    case 12:
        break;
    default:
        func_eboot_08814320(camera, 1, 0, 0);
        break;
    }
}

extern "C" void func_eboot_08814334(Camera *camera, u8 mode, void *subject);

// Starts the demo camera mode (3/4/5) that belongs to the current GameSys
// u16 at 0x6AF0E (ids 0xC/0x19/0xB0); other ids leave the camera alone.
// Small switches are emitted as a compare chain in reverse case order.
extern "C" void func_eboot_08814B00(Camera *camera) {
    u8 mode;
    switch (*(u16 *)((u8 *)GameSys::objectPtr + 0x6AF0E)) {
    case 0xC: mode = 3; break;
    case 0x19: mode = 4; break;
    case 0xB0: mode = 5; break;
    default: return;
    }
    func_eboot_08814334(camera, mode, 0);
}

extern "C" void func_eboot_08814B5C(Camera *camera, void *subject) {
    func_eboot_08814320(camera, 10, subject, 0);
}

extern "C" void func_eboot_08814B6C(Camera *camera) {
    u8 *p = (u8 *)camera;
    p[0x968] = 0;
    *(s16 *)(p + 0x964) = 0;
    p[0x9BE] = 0;
    p[0x9BF] = 0;
    *(u32 *)(p + 0x9C0) = 0;
    p[0xA0B] = 0;
    camera->zClipping = true;
    camera->next_demo_id = 0;
    p[0xA0A] = 0;
}

// Demo state: the signed byte 0x9BF if non-zero, else 2 while a demo is
// pending (0xA80) or active (0x9BE), else 0.
extern "C" int func_eboot_08814B98(Camera *camera) {
    u8 *p = (u8 *)camera;
    u8 *q = p + 0x970;
    s8 v = *(s8 *)(p + 0x9BF);
    if (v) return v;
    if (camera->next_demo_id || q[0x4E]) return 2;
    return 0;
}

extern "C" u8 func_eboot_08814BD0(Camera *camera) {
    return *(u8 *)((u8 *)camera + 0x9BE);
}

extern "C" u8 func_eboot_08814BD8(Camera *camera) {
    return *(u8 *)((u8 *)camera + 0xA0A);
}

extern "C"
bool func_eboot_08814BE0(Camera *this_) {
    if (this_->subCameras[5].isActive) {
        switch (this_->subCameras[5].data.demo.demo_id) {
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
            return true;
        default:
            break;
        }
    }
    return false;
}

// Camera s16 at 0x964 when the GameSys flag 0x422 is set, else -1.
extern "C" s16 func_eboot_08814C30(Camera *camera) {
    if (*((u8 *)GameSys::objectPtr + 0x422) == 0) return -1;
    return *(s16 *)((u8 *)camera + 0x964);
}

extern "C"
bool func_eboot_08814C50(Camera *this_) {
    if (GameSys::objectPtr->allow_hidden_flag == true) {
        return true;
    }
    DemoCameraData &data = this_->subCameras[5].data.demo;
    if (data.is_quest_clear) {
        return true;
    }
    switch (data.demo_id) {
    case 0x3:
    case 0x4:
    case 0x5:
    case 0xA:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
        return true;
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    default:
        return false;
    }
}

// True for the demo ids (subCameras[5].data.demo.demo_id) in this list;
// the switch compiles to a compare chain in reverse case order.
extern "C" bool func_eboot_08814CC4(Camera *camera) {
    switch (camera->subCameras[5].data.demo.demo_id) {
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1F:
    case 0x21:
    case 0x23:
    case 0x24:
    case 0x27:
    case 0x28:
    case 0x2A:
    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x31:
    case 0x33:
    case 0x34:
    case 0x38:
    case 0x39:
    case 0x3A:
    case 0x3B:
    case 0x3C:
    case 0x3D:
    case 0x3E:
    case 0x3F:
        return true;
    }
    return false;
}

extern "C" int func_eboot_08814E08(Camera *camera, void *source) {
    u8 *p = (u8 *)camera;
    if (p[0x961] != 0) return 0;
    int active = (*(u8 *)((u8 *)source + 0x564) != 0);
    if (active == 1) return 1;
    return p[0x621] != 0;
}

// PCHNGR sub-camera (index 3) query: two state bytes (data+0x69/+0x6A) and,
// when the second is set, (current_fov - data+0x50) * data+0x58; returns
// the signed byte data+0x68. The data fields are not named in camera.hpp yet.
extern "C" s8 func_eboot_08814E40(Camera *camera, u8 *a, u8 *b, float *c) {
    SubCamera *s = &camera->subCameras[SubCameraType::PCHNGR];
    u8 *d = (u8 *)&s->data;
    *a = d[0x69];
    *b = d[0x6A];
    if (d[0x6A] != 0) {
        *c = s->current_fov - *(float *)(d + 0x50);
        *c = *c * *(float *)(d + 0x58);
    }
    return *(s8 *)(d + 0x68);
}

extern "C" void func_eboot_08814E84(Camera *camera, ScePspFVector4 *out) {
    const float *v = (float *)((u8 *)camera + 0xB40);
    out->x = v[0];
    out->y = v[1];
    out->z = v[2];
    out->w = 0;
}

extern "C" void func_eboot_08814EA4(Camera *camera, ScePspFVector4 *out) {
    const float *v = (float *)((u8 *)camera + 0xB50);
    out->x = v[0];
    out->y = v[1];
    out->z = v[2];
    out->w = 0;
}

extern "C" void func_eboot_08814EC4(Camera *camera, ScePspFVector4 *out) {
    copy_q(out, (ScePspFVector4 *)((u8 *)camera + 0xB60));
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08814ED4);

static inline float camera_vdot_t(ScePspFVector4 *a, ScePspFVector4 *b) {
    float result;
#ifdef __MWERKS__
    __asm__ (
        "lv.q C000, %1"
        "lv.q C010, %2"
        "vdot.t S100, C000, C010"
        "sv.s S100, %0"
        : "=m"(result)
        : "m"(*a), "m"(*b)
    );
#else
    result = a->x * b->x + a->y * b->y + a->z * b->z;
#endif
    return result;
}

// xyz normalization (0 stays 0), w kept.
static inline void camera_normalize_t(ScePspFVector4 *out, ScePspFVector4 *v) {
#ifdef __MWERKS__
    __asm__ (
        "lv.q C000, %1"
        "vdot.t S010, C000, C000"
        "vzero.s S011"
        "vcmp.s EZ, S010, S010"
        "nop"
        "vrsq.s S010, S010"
        "vcmovt.s S010, S011, 0"
        "vpfxd 0xFF"
        "vscl.t C000, C000, S010"
        "sv.q C000, %0"
        : "=m"(*out)
        : "m"(*v)
    );
#endif
}

static inline void camera_transpose(ScePspFMatrix4 *m) {
#ifdef __MWERKS__
    __asm__ (
        "lv.q C000, 0x0(%0)"
        "lv.q C010, 0x10(%0)"
        "lv.q C020, 0x20(%0)"
        "lv.q C030, 0x30(%0)"
        "sv.q R000, 0x0(%0)"
        "sv.q R001, 0x10(%0)"
        "sv.q R002, 0x20(%0)"
        "sv.q R003, 0x30(%0)"
        : "+m"(*m)
    );
#endif
}

// dst = affine inverse of src: the 3x3 part transposed and divided by each
// row's squared length, translation = -(that 3x3) * src.w.
static inline void camera_affine_inverse(ScePspFMatrix4 *dst, ScePspFMatrix4 *src) {
#ifdef __MWERKS__
    __asm__ (
        "lv.q C100, 0x0(%1)"
        "lv.q C110, 0x10(%1)"
        "lv.q C120, 0x20(%1)"
        "lv.q C130, 0x30(%1)"
        "vmidt.q E000"
        "vmidt.q E200"
        "vdot.t S300, C100, C100"
        "vsqrt.s S300, S300"
        "vdot.t S301, C110, C110"
        "vsqrt.s S301, S301"
        "vdot.t S302, C120, C120"
        "vsqrt.s S302, S302"
        "vdiv.t C200, R100, C300"
        "vdiv.t C210, R101, C300"
        "vdiv.t C220, R102, C300"
        "vneg.t C310, C130"
        "vmul.t C320, R200, C310"
        "vfad.t S230, C320"
        "vmul.t C320, R201, C310"
        "vfad.t S231, C320"
        "vmul.t C320, R202, C310"
        "vfad.t S232, C320"
        "vdiv.t C000, C200, C300"
        "vdiv.t C010, C210, C300"
        "vdiv.t C020, C220, C300"
        "vdiv.t C030, C230, C300"
        "sv.q C000, 0x0(%0)"
        "sv.q C010, 0x10(%0)"
        "sv.q C020, 0x20(%0)"
        "sv.q C030, 0x30(%0)"
        : "=m"(*dst) : "m"(*src)
    );
#endif
}

extern "C" void func_eboot_088164C0(ScePspFMatrix4 *out, ScePspFVector4 *eye,
                                      ScePspFVector4 *target, ScePspFVector4 *up);

#define CAM_V(o) ((ScePspFVector4 *)((u8 *)this + (o)))
#define CAM_M(o) ((ScePspFMatrix4 *)((u8 *)this + (o)))
#define CAM_THIS_F(o) (*(float *)((u8 *)this + (o)))

// Sets the camera: eye 0xB40, target 0xB50, up 0xB60, eye-target distance
// 0xDB8. Builds two look-at matrices: 0xB80 from the raw vectors (used by the
// culling tests) and 0xBC0 from eye/target scaled by Ge::norm (the view matrix
// of world-to-screen), plus their affine inverses 0xC80 and 0xC40.
void Camera::method_08815028(ScePspFVector4 *eye, ScePspFVector4 *target,
                             ScePspFVector4 *up) {
    ScePspFVector4 dir, seye, starget;
    CAM_V(0xB40)->x = eye->x;
    CAM_V(0xB40)->y = eye->y;
    CAM_V(0xB40)->z = eye->z;
    CAM_V(0xB40)->w = eye->w;
    CAM_V(0xB50)->x = target->x;
    CAM_V(0xB50)->y = target->y;
    CAM_V(0xB50)->z = target->z;
    CAM_V(0xB50)->w = target->w;
    CAM_V(0xB60)->x = up->x;
    CAM_V(0xB60)->y = up->y;
    CAM_V(0xB60)->z = up->z;
    CAM_V(0xB60)->w = up->w;
    vsub_q(&dir, target, eye);
    CAM_THIS_F(0xDB8) = camera_vdot_t(&dir, &dir);
    CAM_THIS_F(0xDB8) = vsqrt_s(CAM_THIS_F(0xDB8));
    func_eboot_088164C0(CAM_M(0xB80), eye, target, up);
    float n = Ge::objectPtr->norm;
    vscl_q(&seye, eye, n);
    vscl_q(&starget, target, n);
    func_eboot_088164C0(CAM_M(0xBC0), &seye, &starget, up);
    camera_affine_inverse(CAM_M(0xC80), CAM_M(0xB80));
    camera_affine_inverse(CAM_M(0xC40), CAM_M(0xBC0));
}

static inline float camera_vcos(float angle) {
    float result;
#ifdef __MWERKS__
    __asm__ (
        "lv.s S000, %1"
        "vcst.s S001, VFPU_2_PI"
        "vmul.s S000, S000, S001"
        "vcos.s S010, S000"
        "sv.s S010, %0"
        : "=m"(result)
        : "m"(angle)
    );
#else
    result = cosf(angle);
#endif
    return result;
}

extern "C" void func_eboot_08815274(Camera *camera, ScePspFMatrix4 *out,
                                       float fov, float aspect, float near_plane,
                                       float far_plane) {
    memset(out, 0, sizeof(*out));
    float scaled_near, far_squared;
    scaled_near = near_plane * Ge::objectPtr->norm;
    far_squared = far_plane * far_plane;
    float half_fov = fov;
    half_fov *= 0.5f;
    float cs = camera_vcos(half_fov);
    float sn = vsin_s(half_fov);
    float cot = cs / sn;
    out->x.x = cot / aspect;
    out->y.y = cot;
    float divisor = far_squared - scaled_near;
    out->z.z = -(far_squared + scaled_near) / divisor;
    out->z.w = -1.0f;
    out->w.z = -(2.0f * far_squared * scaled_near) / divisor;
}

extern "C" void func_eboot_08816C5C(Camera *);

extern "C" void func_eboot_08815384(Camera *camera, float fov, float aspect,
                                       float near_plane, float far_plane) {
    func_eboot_08815274(camera, &camera->projection, fov, aspect, near_plane,
        far_plane);
    // Affine inverse of the 0xC00 matrix (M^T / L^2 on the 3x3 part), not a
    // normalization.
    ScePspFMatrix4 *inverse = (ScePspFMatrix4 *)((u8 *)camera + 0xD00);
    ScePspFMatrix4 *projection = (ScePspFMatrix4 *)((u8 *)camera + 0xC00);
#ifdef __MWERKS__
    __asm__ (
        "lv.q C100, 0x0(%0)"
        "lv.q C110, 0x10(%0)"
        "lv.q C120, 0x20(%0)"
        "lv.q C130, 0x30(%0)"
        "vmidt.q E000"
        "vmidt.q E200"
        "vdot.t S300, C100, C100"
        "vsqrt.s S300, S300"
        "vdot.t S301, C110, C110"
        "vsqrt.s S301, S301"
        "vdot.t S302, C120, C120"
        "vsqrt.s S302, S302"
        "vdiv.t C200, R100, C300"
        "vdiv.t C210, R101, C300"
        "vdiv.t C220, R102, C300"
        "vneg.t C310, C130"
        "vmul.t C320, R200, C310"
        "vfad.t S230, C320"
        "vmul.t C320, R201, C310"
        "vfad.t S231, C320"
        "vmul.t C320, R202, C310"
        "vfad.t S232, C320"
        "vdiv.t C000, C200, C300"
        "vdiv.t C010, C210, C300"
        "vdiv.t C020, C220, C300"
        "vdiv.t C030, C230, C300"
        "sv.q C000, 0x0(%1)"
        "sv.q C010, 0x10(%1)"
        "sv.q C020, 0x20(%1)"
        "sv.q C030, 0x30(%1)"
        : "+m"(*projection), "=m"(*inverse)
    );
#endif
    func_eboot_08816C5C(camera);
}

// World to screen. out = viewport(project(view * (in * Ge::norm, w = 1)))
// minus the offsets 0xB70/0xB74 (1808 = 2048 - 240, 1912 = 2048 - 136):
// x = 240 + 240 * cx / cw, y = 136 - 136 * cy / cw, z = far/2 + far/2 * cz / cw.
// |cw| < 1e-5 is replaced by 1 (no divide by zero, no behind-camera test).
extern "C" void func_eboot_08815434(Camera *camera, ScePspFVector4 *out,
                                      ScePspFVector4 *in) {
    ScePspFVector4 tmp;
    vscl_q(out, in, Ge::objectPtr->norm);
    out->w = 1.0f;
    ScePspFMatrix4 *view = &camera->world;
#ifdef __MWERKS__
    __asm__ (
        "lv.q C100, 0x0(%1)"
        "lv.q C200, 0x0(%2)"
        "lv.q C210, 0x10(%2)"
        "lv.q C220, 0x20(%2)"
        "lv.q C230, 0x30(%2)"
        "vtfm4.q C000, E200, C100"
        "sv.q C000, 0x0(%0)"
        : "=m"(tmp) : "m"(*out), "m"(*view));
#endif
    ScePspFMatrix4 *proj = &camera->projection;
#ifdef __MWERKS__
    __asm__ (
        "lv.q C100, 0x0(%1)"
        "lv.q C200, 0x0(%2)"
        "lv.q C210, 0x10(%2)"
        "lv.q C220, 0x20(%2)"
        "lv.q C230, 0x30(%2)"
        "vtfm4.q C000, E200, C100"
        "sv.q C000, 0x0(%0)"
        : "=m"(*out) : "m"(tmp), "m"(*proj));
#endif
    bool zero = (-1e-5f < out->w && out->w < 1e-5f);
    if (zero == true) out->w = 1.0f;
    out->x = camera->viewport_center.f[0] + (camera->viewport_scale.f[0] * out->x) / out->w;
    out->y = camera->viewport_center.f[1] + (camera->viewport_scale.f[1] * out->y) / out->w;
    out->z = camera->viewport_center.f[2] + (camera->viewport_scale.f[2] * out->z) / out->w;
    out->x -= camera->unknown_0xB70;
    out->y -= camera->unknown_0xB74;
}

// Camera yaw as a 16-bit angle (0x10000 = full turn) from eye 0xB40 towards
// target 0xB50, measured in the XZ plane: atan2(-(dz), dx).
extern "C" u16 func_eboot_08815588(Camera *camera) {
    float x = CAM_F(0xB50) - CAM_F(0xB40);
    float z = CAM_F(0xB58) - CAM_F(0xB48);
    return (int)((65536.0f * atan2f_s(-z, x)) / 6.2831855f + 0.5f);
}

// Overlay for camera fields not yet named in camera.hpp (kept local to avoid
// conflicting edits of the shared header). params (0xAAC) points to the
// current camera parameter block; 0xB1C-0xB30 cache part of it.
struct CameraAreaList {
    int count;
    CameraDataEntry **areas;
};

struct SmallCameraData {
    float unknown_0x0;
    float unknown_0x4;
    float unknown_0x8;
    float unknown_0xC;
    float unknown_0x10;
    float unknown_0x14;
    float unknown_0x18;
};

struct BigCameraData {
    float unknown_0x0[8];
    SmallCameraData unknown_0x20 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0x40 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0x60 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0x80 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0xA0 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0xC0 __attribute__((aligned(0x10)));
    SmallCameraData unknown_0xE0 __attribute__((aligned(0x10)));
};

extern BigCameraData D_eboot_08931120;

struct CameraParamsView {
    float unknown_0x0;
    float unknown_0x4;
    float unknown_0x8;
    float unknown_0xC;
    float unknown_0x10;
    float unknown_0x14;
    float unknown_0x18;
    float unknown_0x1C;
};

struct CameraParams {
    u16 unknown_0x0;
    u16 unknown_0x2;
    u16 unknown_0x4;
    u16 unknown_0x6;
    u16 unknown_0x8;
    u16 unknown_0xA;
    u32 unknown_0xC;
    u32 unknown_0x10;
    u32 unknown_0x14;
    u32 unknown_0x18;
    CameraAreaList *area_lists; // 0x1C: indexed by 0xB16 + 0xB1C * 0xB18
    u32 unknown_0x20;
    u32 unknown_0x24;
    CameraDataEntry *default_area; // 0x28: -> the embedded entry at 0x30
    u8 padding_0x2C[4];
    // 0x30: default CameraDataEntry (bytes 0-5, near/far distance, near/far
    // fov, zones, hokan_info, then the pan/rail union from 0x50).
    u8 unknown_0x30[6];
    float unknown_0x38;
    float unknown_0x3C;
    float unknown_0x40;
    float unknown_0x44;
    float unknown_0x48;
    float unknown_0x4C;
    float unknown_0x50;
    float unknown_0x54;
    float unknown_0x58;
    u16 unknown_0x5C;
    u16 unknown_0x5E;
    u8 padding_0x60[0x10];
    CameraParamsView views[5];
};

struct CameraExt {
    u8 padding_0x0[0xAAC];
    CameraParams *params;
    u8 padding_0xAB0[0xB14 - 0xAB0];
    u8 unknown_0xB14;
    u8 padding_0xB15;
    u16 unknown_0xB16;
    u16 unknown_0xB18;
    u8 padding_0xB1A[0xB1C - 0xB1A];
    u16 unknown_0xB1C;
    u16 unknown_0xB1E;
    u16 unknown_0xB20;
    u16 unknown_0xB22;
    u32 unknown_0xB24;
    u32 unknown_0xB28;
    u32 unknown_0xB2C;
    u32 unknown_0xB30;
};

extern "C" void func_eboot_088159A0(Camera *);

void Camera::method_08815744() {
    CameraExt *o = (CameraExt *)this;
    if (o->params == 0) func_eboot_088159A0(this);
    o->unknown_0xB1C = o->params->unknown_0x4;
    o->unknown_0xB1E = o->params->unknown_0x6;
    o->unknown_0xB20 = o->params->unknown_0x8;
    o->unknown_0xB22 = o->params->unknown_0xA;
    o->unknown_0xB24 = o->params->unknown_0xC;
    o->unknown_0xB28 = o->params->unknown_0x10;
    o->unknown_0xB2C = o->params->unknown_0x14;
    o->unknown_0xB30 = o->params->unknown_0x18;
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_088157D4);

// Fills the camera parameter block with defaults: modes 0x102, 20000
// limits, the default area entry at +0x30 (near 400, far 2400, fovs 1 / 0.75,
// 5*pi/18) and five views copied from D_eboot_08931120.
#define COPY(i, s) \
    p->views[i].unknown_0x4 = D_eboot_08931120.s.unknown_0x4; \
    p->views[i].unknown_0x8 = D_eboot_08931120.s.unknown_0x8; \
    p->views[i].unknown_0x10 = D_eboot_08931120.s.unknown_0x10; \
    p->views[i].unknown_0x18 = D_eboot_08931120.s.unknown_0x18;
extern "C" void func_eboot_088159A0(Camera *camera) {
    CameraExt *o = (CameraExt *)camera;
    CameraParams *p = o->params;
    u8 *q = p->unknown_0x30;
    p->unknown_0x0 = 0x102;
    p->unknown_0x2 = 0;
    p->unknown_0x4 = 1;
    p->unknown_0x6 = 1;
    p->unknown_0x8 = 20000;
    p->unknown_0xA = 20000;
    p->unknown_0xC = 0;
    p->unknown_0x10 = 0;
    p->unknown_0x14 = 20000;
    p->unknown_0x18 = 20000;
    p->area_lists = 0;
    p->unknown_0x20 = 0;
    p->unknown_0x24 = 0;
    p->default_area = (CameraDataEntry *)q;
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
    q[3] = 2;
    q[4] = 0;
    q[5] = 0;
    p->unknown_0x38 = 400;
    p->unknown_0x3C = 2400;
    p->unknown_0x40 = 1;
    p->unknown_0x44 = 0.75f;
    p->unknown_0x48 = 0;
    p->unknown_0x4C = 0;
    COPY(0, unknown_0x20)
    COPY(1, unknown_0x40)
    COPY(2, unknown_0x60)
    COPY(3, unknown_0x80)
    COPY(4, unknown_0xA0)
    p->unknown_0x50 = 5 * PI / 18;
    p->unknown_0x54 = 0;
    p->unknown_0x58 = 0;
    p->unknown_0x5E = 0;
    p->unknown_0x5C = 0;
    o->params = p;
}
#undef COPY

// Latches the pad state the camera reads (0xA70 buttons, 0xA72 rising edge,
// 0xA76 analog angle, 0xA78 analog magnitude); cleared while the GameSys
// check (mode 1) is set or the player byte 0x552 is set.
extern "C" void func_eboot_08815B44(Camera *camera) {
    u8 *p = (u8 *)camera;
    if (func_eboot_0884F9A0(GameSys::objectPtr, 1) == true ||
        *((u8 *)camera->player + 0x552) != 0) {
        *(u16 *)(p + 0xA74) = 0;
        camera->rising_edge = 0;
        camera->buttons = 0;
        *(u16 *)(p + 0xA78) = 0;
        *(u16 *)(p + 0xA76) = 0;
    } else {
        camera->buttons = Pad::BUTTONS;
        camera->rising_edge = Pad::RISING_EDGE;
        *(u16 *)(p + 0xA76) = Pad::ANALOG_ANGLE;
        *(u16 *)(p + 0xA78) = Pad::ANALOG_MAGNITUDE;
    }
}


extern "C" void func_eboot_08815DC4(Camera *camera, int id);

// Per-frame camera select. Ticks the three effect channels (0xAC0 + 0x20*i:
// active byte, s16 timer at +2). Copies the base sub-camera (0xA8D) pose to
// 0x20/0x30 (roll 0xA0, fov 0xA8). An active DEMO (5), PCHNGR (3) or
// PLAYER_EX (4) sub-camera overrides it (id in 0xDC4, else -1): eye 0xB40 and
// target 0xB50 come from that sub-camera, then the effect channel is applied
// (08813B78 with 0xAF0/0xAD0/0xAB0), roll -> 0xB34, fov -> 0xC.
extern "C" void func_eboot_08815BD8(Camera *camera) {
    u8 *p = (u8 *)camera;
    for (int i = 0; i < 3; i++) {
        u8 *e = p + i * 0x20;
        if (e[0xAC0] != 0) {
            if (--*(s16 *)(e + 0xAC2) <= 0) e[0xAC0] = 0;
        }
    }
    SubCamera *base = &camera->subCameras[camera->base_sub_type];
    copy_q((ScePspFVector4 *)(p + 0x20), &base->current_position);
    copy_q((ScePspFVector4 *)(p + 0x30), &base->current_target);
    CAM_F(0xA0) = base->current_roll;
    CAM_F(0xA8) = base->current_fov;
    *(s8 *)(p + 0xDC4) = -1;
    if (camera->subCameras[5].isActive) *(s8 *)(p + 0xDC4) = 5;
    else if (camera->subCameras[3].isActive) *(s8 *)(p + 0xDC4) = 3;
    else if (camera->subCameras[4].isActive) *(s8 *)(p + 0xDC4) = 4;
    if (*(s8 *)(p + 0xDC4) > 0) {
        SubCamera *s = &camera->subCameras[*(s8 *)(p + 0xDC4)];
        copy_q((ScePspFVector4 *)(p + 0xB40), &s->current_position);
        copy_q((ScePspFVector4 *)(p + 0xB50), &s->current_target);
        if (*(s8 *)(p + 0xDC4) == 5) func_eboot_08813B78(camera, (CameraShake *)(p + 0xAF0));
        else if (*(s8 *)(p + 0xDC4) == 3) func_eboot_08813B78(camera, (CameraShake *)(p + 0xAD0));
        else if (*(s8 *)(p + 0xDC4) == 4) func_eboot_08813B78(camera, (CameraShake *)(p + 0xAB0));
        CAM_F(0xB34) = s->current_roll;
        camera->unknown_0xC = s->current_fov;
        camera->unknown_0xA88 = 0;
    } else {
        copy_q((ScePspFVector4 *)(p + 0xB40), (ScePspFVector4 *)(p + 0x20));
        copy_q((ScePspFVector4 *)(p + 0xB50), (ScePspFVector4 *)(p + 0x30));
        func_eboot_08813B78(camera, (CameraShake *)(p + 0xAB0));
        CAM_F(0xB34) = CAM_F(0xA0);
        camera->unknown_0xC = CAM_F(0xA8);
    }
    func_eboot_08815DC4(camera, *(s8 *)(p + 0xDC4));
}

// Sets sub-camera 0's byte 0x92 (sub-camera id/type); 5 and negative ids
// become 0xFF (none).
extern "C" void func_eboot_08815DC4(Camera *camera, int id) {
    SubCamera *s = camera->subCameras;
    if (id == 5) id = -1;
    s->unknown_0x92 = id >= 0 ? id : 0xFF;
}

extern "C" u8 func_eboot_08815EC8(Camera *, CameraDataEntry *, int);

// Picks the camera area (0xA90 `areas`) from the parameter block: no block ->
// NULL, -1; otherwise the default area (params+0x28), then, unless 0xB14 is
// set, the first entry of the area list selected by 0xB16 + 0xB1C * 0xB18 for
// which 08815EC8(camera, area, 1) returns 0 (returns 1 when one is found).
extern "C" int func_eboot_08815DE4(Camera *camera) {
    CameraExt *o = (CameraExt *)camera;
    if (o->params == NULL) {
        camera->areas = NULL;
        return -1;
    }
    camera->areas = o->params->default_area;
    if (o->unknown_0xB14 != 0) return 0;
    if (o->params->area_lists == NULL) return 0;
    CameraAreaList *e = &o->params->area_lists[o->unknown_0xB16 + o->unknown_0xB1C * o->unknown_0xB18];
    int n = e->count;
    if (n == 0) return 0;
    CameraDataEntry **list = e->areas;
    for (int i = n; i > 0; i--, list++) {
        if (func_eboot_08815EC8(camera, *list, 1) == 0) {
            camera->areas = *list;
            return 1;
        }
    }
    return 0;
}

// One trigger zone of a camera area (CameraDataEntry::zones, 0x40 bytes).
struct CameraZone {
    float quad[8];          // XZ corners, see 08816020
    ScePspFVector3 origin;  // 0x20
    u8 flags;               // 0x2C: zones whose flags & mask are skipped
    ScePspFVector3 normal;  // 0x30
    float depth;            // 0x3C: slab thickness along the normal
};

extern "C" u8 func_eboot_088DCC6C(Player *, Player *);
extern "C" u8 func_eboot_08816020(Camera *camera, float *q, ScePspFVector4 *p);

// dot(a, b) of two unaligned 3-vectors (component-wise lv.s loads).
static inline float camera_vdot3(ScePspFVector3 *a, ScePspFVector3 *b) {
    float result;
#ifdef __MWERKS__
    __asm__ (
        "lv.s S000, 0x0(%1)"
        "lv.s S001, 0x4(%1)"
        "lv.s S002, 0x8(%1)"
        "lv.s S010, 0x0(%2)"
        "lv.s S011, 0x4(%2)"
        "lv.s S012, 0x8(%2)"
        "vdot.t S100, C000, C010"
        "sv.s S100, %0"
        : "=m"(result)
        : "m"(*a), "m"(*b)
    );
#else
    result = a->x * b->x + a->y * b->y + a->z * b->z;
#endif
    return result;
}

// Is the player inside the camera area? Areas with attribute 0x80 first need
// 088DCC6C(player, player). Then each zone not masked out by `mask` is tested:
// the player position (player+0x200) must lie in the slab
// 0 <= dot(normal, pos - origin) <= depth and inside the zone's XZ quad.
// Returns 0 when inside some zone, 2 otherwise.
extern "C" u8 func_eboot_08815EC8(Camera *camera, CameraDataEntry *area, int mask) {
    CameraZone *z;
    int n;
    if (area->attributes & 0x80) {
        if (func_eboot_088DCC6C(camera->player, camera->player) == 0) return 2;
    }
    z = (CameraZone *)area->zones;
    for (n = area->zone_count; n > 0; n--, z++) {
        if ((z->flags & (u8)mask) != 0) continue;
        Player *pl = camera->player;
        ScePspFVector3 d;
        d.x = *(float *)((u8 *)pl + 0x200) - z->origin.x;
        d.y = *(float *)((u8 *)pl + 0x204) - z->origin.y;
        d.z = *(float *)((u8 *)pl + 0x208) - z->origin.z;
        float dot = camera_vdot3(&z->normal, &d);
        if (dot < 0.0f) continue;
        if (dot > z->depth) continue;
        if (func_eboot_08816020(camera, z->quad, (ScePspFVector4 *)((u8 *)pl + 0x200)) == 0) return 0;
    }
    return 2;
}

// Point-in-quad test in the XZ plane for a camera zone. q holds two corner
// pairs: A0 = (q[0], q[2]), A1 = (q[1], q[3]), B0 = (q[4], q[6]),
// B1 = (q[5], q[7]). Returns 2 (outside) as soon as p lies on the negative
// side of an edge (2D cross product < 0), else 0 (inside).
extern "C" u8 func_eboot_08816020(Camera *camera, float *q, ScePspFVector4 *p) {
    float dx, dz, ex, ez;
    dx = p->x - q[0];
    dz = p->z - q[2];
    ex = q[0] - q[4];
    ez = q[2] - q[6];
    if (ez * dx - dz * ex < 0.0f) return 2;
    ex = q[5] - q[0];
    ez = q[7] - q[2];
    if (ez * dx - dz * ex < 0.0f) return 2;
    dx = p->x - q[1];
    dz = p->z - q[3];
    ex = q[4] - q[1];
    ez = q[6] - q[3];
    if (ez * dx - dz * ex < 0.0f) return 2;
    ex = q[1] - q[5];
    ez = q[3] - q[7];
    if (ez * dx - dz * ex < 0.0f) return 2;
    return 0;
}

extern "C" void func_eboot_08815384(Camera *, float, float, float, float);

void Camera::method_08816108() {
    func_eboot_08815384(this, unknown_0xC, unknown_0x8, near_z, far_z);
    unknown_0x10 = unknown_0xC;
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816144);

// Right-handed look-at (gluLookAt layout, row-vector form after the transpose):
// f = normalize(eye - target), s = normalize(normalize(up) x f), v = f x s;
// rows 0-2 of the rotation are s, v, f; translation = -(s.eye, v.eye, f.eye).
extern "C" void func_eboot_088164C0(ScePspFMatrix4 *out, ScePspFVector4 *eye,
                                      ScePspFVector4 *target, ScePspFVector4 *up) {
    ScePspFVector4 s, v, f, u;
    vsub_q(&f, eye, target);
    copy_q(&u, up);
    camera_normalize_t(&f, &f);
    camera_normalize_t(&u, &u);
    flvecOuterProduct(&s, &u, &f);
    camera_normalize_t(&s, &s);
    flvecOuterProduct(&v, &f, &s);
    copy_q(&out->x, &s);
    copy_q(&out->y, &v);
    copy_q(&out->z, &f);
    sv_q(&out->w, 0, 0, 0, 0);
    camera_transpose(out);
    sv_q(&out->w, -camera_vdot_t(eye, &s), -camera_vdot_t(eye, &v),
         -camera_vdot_t(eye, &f), 1.0f);
}

extern "C" void func_eboot_08816724(Camera *);

// Places the camera at once: up = (0,1,0), eye/target copied to both the
// requested (0x20/0x30) and the current (0xB40/0xB50) vectors, FOV (radians
// in VFPU units, see 0xC) to 0xA8 and 0xC, then rebuilds the matrices.
extern "C" void func_eboot_08816698(Camera *camera, ScePspFVector4 *eye,
                                      ScePspFVector4 *target, float fov) {
    sv_q((ScePspFVector4 *)((u8 *)camera + 0xB60), 0, 1, 0, 0);
    copy_q((ScePspFVector4 *)((u8 *)camera + 0x20), eye);
    copy_q((ScePspFVector4 *)((u8 *)camera + 0x30), target);
    copy_q((ScePspFVector4 *)((u8 *)camera + 0xB40), eye);
    copy_q((ScePspFVector4 *)((u8 *)camera + 0xB50), target);
    CAM_F(0xA8) = fov;
    camera->unknown_0xC = fov;
    func_eboot_08816724(camera);
}

INCLUDE_ASM("asm/eboot/nonmatchings/camera", func_eboot_08816724);

extern "C" void func_eboot_08816B24(Camera *camera, ScePspFVector4 *position,
                                      ScePspFVector4 *target, float fov) {
    copy_q((ScePspFVector4 *)((u8 *)camera + 0x20), position);
    copy_q((ScePspFVector4 *)((u8 *)camera + 0x30), target);
    *(float *)((u8 *)camera + 0xA8) = fov;
}

// Rotates the up vector (0, 1, 0) by `angle` radians around the axis to - from.
extern "C" void func_eboot_08816B44(ScePspFVector4 *out, ScePspFVector4 *from,
                                      ScePspFVector4 *to, float angle) {
    ScePspFVector4 axis;
    ScePspFVector4 q;
    ScePspFMatrix4 m;
    axis.x = to->x - from->x;
    axis.y = to->y - from->y;
    axis.z = to->z - from->z;
    axis.w = 1.0f;
    sceVfpuQuaternionFromRotate(&q, &axis, angle);
    sceVfpuQuaternionToMatrix(&m, &q);
    axis.x = 0; axis.y = 1; axis.z = 0;
    flvecApplyMat33(out, &axis, &m);
}

// Frustum plane through p0, p1, p2: out.xyz = normalize((p1 - p0) x (p2 - p0)).
extern "C" void func_eboot_08816BE8(Camera *camera, ScePspFVector4 *out,
                                      ScePspFVector4 *p0, ScePspFVector4 *p1,
                                      ScePspFVector4 *p2) {
    ScePspFVector4 a, b, n;
    vsub_q(&a, p1, p0);
    vsub_q(&b, p2, p0);
    flvecOuterProduct(&n, &a, &b);
#ifdef __MWERKS__
    __asm__ (
        "lv.q C000, %1"
        "vdot.t S010, C000, C000"
        "vzero.s S011"
        "vcmp.s EZ, S010, S010"
        "nop"
        "vrsq.s S010, S010"
        "vcmovt.s S010, S011, 0"
        "vpfxd 0xFF"
        "vscl.t C000, C000, S010"
        "sv.q C000, %0"
        : "=m"(*out)
        : "m"(n)
    );
#endif
}

static inline void camera_vzero_q(ScePspFVector4 *v) {
#ifdef __MWERKS__
    __asm__ (
        "vzero.q C000"
        "sv.q C000, %0"
        : "=m"(*v)
    );
#else
    v->x = v->y = v->z = v->w = 0;
#endif
}

// Builds the four side culling planes (view space, through the eye): 0xD50
// left, 0xD60 right, 0xD70 top, 0xD80 bottom, and the negated depth limits
// 0xD90 = -near, 0xD94 = -far. The corner points sit at z = -1.5 far with
// |x| = far * cot(fov/2) and |y| = |x| / aspect. cot, not tan: these planes are
// much wider than the projection frustum (about +-55 x +-39 degrees against
// +-39 x +-25 at the default fov 0.8722 rad and aspect 30/17).
extern "C" void func_eboot_08816C5C(Camera *camera) {
    ScePspFVector4 origin, p1, p2;
    camera_vzero_q(&origin);
    p1.z = p2.z = 1.5f * -camera->far_z;
    float half = 0.5f * camera->unknown_0xC;
    float c = camera_vcos(half);
    float s = vsin_s(half);
    p1.x = -camera->far_z * (c / s);
    p1.y = camera->far_z * (c / s) / camera->unknown_0x8;
    p2.x = p1.x;
    p2.y = -p1.y;
    func_eboot_08816BE8(camera, (ScePspFVector4 *)((u8 *)camera + 0xD50), &origin, &p1, &p2);
    p1.x = camera->far_z * (c / s);
    p1.y = -camera->far_z * (c / s) / camera->unknown_0x8;
    p2.x = p1.x;
    p2.y = -p1.y;
    func_eboot_08816BE8(camera, (ScePspFVector4 *)((u8 *)camera + 0xD60), &origin, &p1, &p2);
    p1.x = camera->far_z * (c / s);
    p1.y = p1.x / camera->unknown_0x8;
    p2.x = -p1.x;
    p2.y = p1.y;
    func_eboot_08816BE8(camera, (ScePspFVector4 *)((u8 *)camera + 0xD70), &origin, &p1, &p2);
    p1.x = -camera->far_z * (c / s);
    p1.y = p1.x / camera->unknown_0x8;
    p2.x = -p1.x;
    p2.y = p1.y;
    func_eboot_08816BE8(camera, (ScePspFVector4 *)((u8 *)camera + 0xD80), &origin, &p1, &p2);
    *(float *)((u8 *)camera + 0xD94) = -camera->far_z;
    *(float *)((u8 *)camera + 0xD90) = -camera->near_z;
}

// Depth-only visibility of a sphere (p, margin): view z from column z of the
// 0xB80 matrix; false (culled) when entirely in front of -near or beyond -far.
// Always true when zClipping (0xAAA) is off.
extern "C" bool func_eboot_08816E20(Camera *camera, ScePspFVector4 *p, float margin) {
    if (!camera->zClipping) return 1;
    float z = CAM_F(0xB88) * p->x + CAM_F(0xB98) * p->y + CAM_F(0xBA8) * p->z + CAM_F(0xBB8);
    float d = z - margin;
    if (d > CAM_F(0xD90)) return 0;
    d = z + margin;
    return !(d < CAM_F(0xD94));
}

extern "C" bool func_eboot_08816EB0(Camera *, ScePspFVector4 *, float, float);

extern "C" bool func_eboot_08816EA8(Camera *camera, ScePspFVector4 *position,
                                     float margin) {
    return func_eboot_08816EB0(camera, position, margin,
        *(float *)((u8 *)camera + 0xD94));
}

// Full sphere-vs-frustum test (p, margin) in view space: depth against -near
// and far_limit (EA8 passes 0xD94 = -far), then x against the left/right
// planes (0xD50/0xD60: x and z terms) and y against the top/bottom planes
// (0xD70/0xD80: y and z terms). true = visible; true when zClipping is off.
extern "C" bool func_eboot_08816EB0(Camera *camera, ScePspFVector4 *p, float margin,
                                      float far_limit) {
    if (!camera->zClipping) return 1;
    float z = CAM_F(0xB88) * p->x + CAM_F(0xB98) * p->y + CAM_F(0xBA8) * p->z + CAM_F(0xBB8);
    float d = z - margin;
    if (d > CAM_F(0xD90)) return 0;
    d = z + margin;
    if (d < far_limit) return 0;
    float x = CAM_F(0xB80) * p->x + CAM_F(0xB90) * p->y + CAM_F(0xBA0) * p->z + CAM_F(0xBB0);
    if (x * CAM_F(0xD50) + z * CAM_F(0xD58) > margin) return 0;
    if (x * CAM_F(0xD60) + z * CAM_F(0xD68) > margin) return 0;
    float y = CAM_F(0xB84) * p->x + CAM_F(0xB94) * p->y + CAM_F(0xBA4) * p->z + CAM_F(0xBB4);
    if (y * CAM_F(0xD74) + z * CAM_F(0xD78) > margin) return 0;
    return !(y * CAM_F(0xD84) + z * CAM_F(0xD88) > margin);
}

void Camera::method_08817024() {
    float half_far = far_z * 0.5f;
    viewport_scale.f[0] = 240;
    viewport_scale.f[1] = -136;
    viewport_scale.f[2] = half_far;
    viewport_center.f[0] = 2048;
    viewport_center.f[1] = 2048;
    viewport_center.f[2] = half_far;
}


float D_eboot_089310D0 = 0.075;
u8 D_eboot_089310D4[3] = {0, 2, 20};
s16 D_eboot_089310D8[8] = {10, 20, 30, 40, 50, 60, 70, 80};
float D_eboot_089310E8[3] = {2000, 3000, 4000};
u8 pad_089310F4[12] = {};
ScePspFVector4 D_eboot_08931100 = {0, 0, 320, 0};
ScePspFVector4 D_eboot_08931110 = {0, 0, -1000, 0};

// (SmallCameraData / BigCameraData are defined above func_eboot_088159A0.)
BigCameraData D_eboot_08931120 = {
    {5 * PI / 18},
    {0, 300, 160, 0, 184, 0, 80},
    {0, 270, 490, 0, 170, 0, 80},
    {0, 140, 490, 0, 170, 0, 80},
    {0, 60, 450, 0, 210, 0, 60},
    {0, 400, 220, 0, 195, 0, 80}
};

SmallCameraData D_eboot_08931220[2] = {
    {0, 280, 500, 0, 170, 0, 80},
    {0, 250, 100, 0, 0, 0, 80},
};

u8 pad_08931258[8] = {};

ScePspFVector4 D_eboot_08931260[8] = {
    {0, 0, 30},
    {0, 0, -1000},
    {0, 100, 30},
    {0, 0, -1000},
    {0, 150, -50},
    {0, 0, -1000},
    {100, 350, 400},
    {100, -550, -1000}
};

struct FishingCameraOffsets {
    ScePspFVector3 position;
    ScePspFVector3 target;
};

FishingCameraOffsets D_eboot_089312E0[39] = {
    {{84, 380, -200}, {10, 21, 127}},
    {{46, 143, 690}, {-21, 154, 176}},
    {{46, 90, 640}, {-21, 104, 176}},
    {{84, 380, -200}, {10, 21, 127}},
    {{300, 320, 40}, {0.0, 50, 160}},
    {{84, 380, -200}, {10, 21, 127}},
    {{125, 461, -263}, {34, 40, 127}},
    {{221, 34, 1050}, {-220, 120, 161}},
    {{-418, -82, -73}, {46, 40, 400}},
    {{84, 380, -200}, {10, 21, 127}},
    {{281, 157, 687}, {-88, -33, 261}},
    {{191, 192, 670}, {-81, -65, 268}},
    {{84, 360, -240}, {10, 55, 222}},
    {{84, 360, -240}, {10, 55, 222}},
    {{-132, 297, -128}, {7, 92, 152}},
    {{90, 297, -174}, {7, 82, 152}},
    {{84, 360, -240}, {10, 55, 222}},
    {{84, 360, -240}, {10, 55, 222}},
    {{525, 410, 365}, {55, 120, 296}},
    {{-294, 90, 1130}, {99, -140, 135}},
    {{-257, 340, 700}, {-16, 120, 299}},
    {{274, 274, 666}, {69, 40, 262}},
    {{84, 360, -240}, {10, 55, 222}},
    {{445, 310, 500}, {-15, 5, 280}},
    {{84, 360, -240}, {10, 55, 222}},
    {{46, 143, 690}, {-21, 154, 176}},
    {{46, 90, 640}, {-21, 104, 176}},
    {{-638, 553, 858}, {844, -621, -210}},
    {{84, 380, -200}, {10, 21, 127}},
    {{125, 461, -263}, {34, 40, 127}},
    {{84, 380, -200}, {10, 21, 127}},
    {{191, 192, 670}, {-81, -65, 268}},
    {{281, 157, 687}, {-88, -33, 261}},
    {{221, 34, 1050}, {-220, 120, 161}},
    {{-418, -82, -73}, {46, 40, 400}},
    {{84, 360, -240}, {10, 55, 222}},
    {{84, 360, -240}, {10, 55, 222}},
    {{-293, 471, -247}, {373, -233, 1020}},
    {{-345, 228, 926}, {380, -80, 110}}
};

float D_eboot_08931688[39] = {
    [0 ... 38] = 11 * PI / 36,
    [21] = 12 * PI / 36,
    [23] = 12 * PI / 36,
};

// maps stageId into D_eboot_089312E0/08931688
u8 D_eboot_08931724[267] = {
    [0 ... 266] = -1,
    [1] = 13,
    [4] = 14,
    [5] = 15,
    [16] = 23,
    [19] = 16,
    [21] = 0,
    [22] = 1,
    [23] = 2,
    [42] = 3,
    [46] = 4,
    [48] = 20,
    [52] = 5,
    [54] = 6,
    [59] = 7,
    [61] = 8,
    [67] = 9,
    [68] = 24,
    [74] = 10,
    [75] = 11,
    [99] = 12,
    [108] = 12,
    [110] = 13,
    [113] = 14,
    [114] = 15,
    [116] = 16,
    [121] = 17,
    [130] = 18,
    [142] = 20,
    [144] = 21,
    [146] = 22,
    [151] = 23,
    [154] = 24,
    [179] = 19,
    [184] = 0,
    [195] = 3,
    [201] = 37,
    [203] = 38,
    [208] = 25,
    [209] = 26,
    [214] = 27,
    [218] = 28,
    [220] = 29,
    [229] = 30,
    [236] = 31,
    [237] = 32,
    [239] = 33,
    [241] = 34,
    [249] = 35,
    [251] = 36,
    [259] = 35,
    [261] = 36,
};

#include "../../assets/eboot/D_eboot_08931830.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089319A0.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089319E8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931A2C.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931A70.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931BF4.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931CC0.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931DEC.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08931F74.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932004.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932308.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932568.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932860.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08932E58.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089333DC.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08933850.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08933D4C.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089340C0.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934660.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934674.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089347D8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089348E8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089349F8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934B08.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934C18.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934D28.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934E38.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08934F44.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089350B8.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089351D0.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08935344.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_089354CC.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_08935640.cameraop.inc.hpp"
#include "../../assets/eboot/D_eboot_0893575C.cameraop.inc.hpp"

typedef u32 CameraScript[];
extern CameraScript
    D_em02_09D27700, D_em02_09D28270, D_em02_09D28538, D_em02_09D28740, D_em02_09D28D90, D_em02_09D2A1F0,
    D_em07_09D21DC8, D_em07_09D24ED8, D_em07_09D25378, D_em07_09D25848,
    D_em14_09D2AB68, D_em14_09D2AED8,
    D_em15_09D2C3D0,
    D_em17_09D2CBD0,
    D_em20_09D34540, D_em20_09D34E20,
    D_em21_09D29450, D_em21_09D295E0,
    D_em33_09D20C90,
    D_em40_09D35790,
    D_em54_09D3C8E8, D_em54_09D3CB40, D_em54_09D3CF68, D_em54_09D3D358, D_em54_09D3D7F0, D_em54_09D3DAA8, D_em54_09D3DC40,
    D_em55_09D25F08, D_em55_09D26948, D_em55_09D26C90,
    D_em58_09D23010,
    D_em59_09D25308, D_em59_09D25710,
    D_em75_09D56958, D_em75_09D56D70, D_em75_09D57088, D_em75_09D57488, D_em75_09D57490,
    D_lobby_task_09B06098, D_lobby_task_09B06258, D_lobby_task_09B06318, D_lobby_task_09B06420, D_lobby_task_09B06528,
    D_lobby_task_09B06680, D_lobby_task_09B06988, D_lobby_task_09B069F0, D_lobby_task_09B06CA8, D_lobby_task_09B06E30,
    D_lobby_task_09B07250, D_lobby_task_09B072C0, D_lobby_task_09B07388, D_lobby_task_09B073F0, D_lobby_task_09B074D0,
    D_lobby_task_09B078C0, D_lobby_task_09B079D8, D_lobby_task_09B07C60, D_lobby_task_09B07D20, D_lobby_task_09B07DE0,
    D_lobby_task_09B07EA0, D_lobby_task_09B07F60, D_lobby_task_09B08020, D_lobby_task_09B080E0, D_lobby_task_09B081A0;

u32 *demo_cam_tbl[107] = {
    0,
    &D_eboot_08931830[0],
    &D_eboot_08931F74[0],
    &D_eboot_089319A0[0],
    &D_eboot_089319E8[0],
    &D_eboot_08931A2C[0],
    &D_eboot_08931CC0[0],
    &D_eboot_08931BF4[0],
    &D_eboot_08931DEC[0],
    &D_eboot_08931A70[0],
    &D_em21_09D29450[0],
    &D_em07_09D24ED8[0],
    &D_em02_09D28270[0],
    &D_em02_09D28538[0],
    &D_em55_09D26948[0],
    &D_em54_09D3DAA8[0],
    &D_em59_09D25710[0],
    &D_em54_09D3DC40[0],
    &D_em75_09D57488[0],
    &D_em07_09D25378[0],
    &D_em02_09D28740[0],
    &D_em55_09D26C90[0],
    &D_em75_09D57490[0],
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    &D_eboot_08932004[0],
    &D_eboot_08932308[0],
    &D_eboot_08932568[0],
    &D_em20_09D34540[0],
    &D_eboot_08932860[0],
    &D_em20_09D34E20[0],
    &D_eboot_08932E58[0],
    &D_eboot_089333DC[0],
    &D_em54_09D3C8E8[0],
    &D_em15_09D2C3D0[0],
    &D_eboot_08933850[0],
    &D_em21_09D295E0[0],
    &D_eboot_08933D4C[0],
    &D_em14_09D2AB68[0],
    &D_em54_09D3CB40[0],
    &D_em54_09D3CF68[0],
    &D_em54_09D3D358[0],
    &D_em17_09D2CBD0[0],
    &D_em54_09D3D7F0[0],
    &D_em33_09D20C90[0],
    &D_em14_09D2AED8[0],
    &D_em59_09D25308[0],
    &D_eboot_089340C0[0],
    &D_em40_09D35790[0],
    &D_em07_09D21DC8[0],
    &D_em07_09D25848[0],
    &D_em55_09D25F08[0],
    &D_em75_09D56958[0],
    &D_em75_09D56D70[0],
    &D_em02_09D27700[0],
    &D_em02_09D28D90[0],
    &D_em02_09D2A1F0[0],
    &D_em75_09D57088[0],
    &D_em58_09D23010[0],
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    (u32 *)0xFFFFFFFF,
    &D_eboot_08934674[0],
    &D_eboot_089347D8[0],
    &D_eboot_089348E8[0],
    &D_eboot_089349F8[0],
    &D_eboot_08934B08[0],
    &D_eboot_08934C18[0],
    &D_eboot_08934D28[0],
    &D_eboot_08934E38[0],
    &D_eboot_08935344[0],
    &D_eboot_089354CC[0],
    &D_eboot_08935640[0],
    &D_eboot_0893575C[0],
    &D_eboot_08934F44[0],
    &D_eboot_089351D0[0],
    &D_eboot_089350B8[0],
    (u32 *)0xFFFFFFFF,
    &D_lobby_task_09B06098[0],
    &D_lobby_task_09B06258[0],
    &D_lobby_task_09B06318[0],
    &D_lobby_task_09B06420[0],
    &D_lobby_task_09B06528[0],
    &D_lobby_task_09B06680[0],
    &D_lobby_task_09B06988[0],
    &D_lobby_task_09B069F0[0],
    &D_lobby_task_09B06CA8[0],
    &D_lobby_task_09B06E30[0],
    &D_lobby_task_09B07250[0],
    &D_lobby_task_09B072C0[0],
    &D_lobby_task_09B07388[0],
    &D_lobby_task_09B073F0[0],
    &D_lobby_task_09B074D0[0],
    (u32 *)0xFFFFFFFF,
    &D_lobby_task_09B078C0[0],
    &D_lobby_task_09B079D8[0],
    &D_lobby_task_09B07C60[0],
    &D_lobby_task_09B07D20[0],
    &D_lobby_task_09B07DE0[0],
    &D_lobby_task_09B07EA0[0],
    &D_lobby_task_09B07F60[0],
    &D_lobby_task_09B08020[0],
    &D_lobby_task_09B080E0[0],
    &D_lobby_task_09B081A0[0],
    &D_eboot_08934660[0],
};

// maps emId to cameraScriptId; death cutscene?
s8 D_eboot_08935A20[90] = {
    [0 ... 89] = 6,
    [0] = -1,
    [3 ... 4] = -1,
    [10] = -1,
    [12] = -1,
    [18] = -1,
    [25] = -1,
    [29] = -1,
    [32] = -1,
    [56] = -1,
    [58] = -1,
    [68 ... 70] = -1,
    [72] = -1,
    [74] = -1,
    [2] = 20,
    [5] = 7,
    [7] = 19,
    [9] = 7,
    [13] = 8,
    [16] = 8,
    [19] = 7,
    [23 ... 24] = 7,
    [27 ... 28] = 8,
    [30 ... 31] = 8,
    [33] = 8,
    [35] = 8,
    [36] = 20,
    [50] = 19,
    [55] = 21,
    [57] = 7,
    [61 ... 63] = 9,
    [66] = 9,
    [68] = 8,
    [71] = 20,
    [73] = 9,
    [76] = 22,
    [77] = 8,
    [79 ... 80] = 8,
    [88] = 23,
};

struct CameraScriptExt {
    u16 cameraScriptIndex;
    u16 stagePacIndexOffset;
    u16 flags;
};

CameraScriptExt D_eboot_08935A7C[10] = {
    {95, 0, 0},
    {79, 0, 0},
    {60, 0, 0},
    {23, 1, 4 | 1},
    {61, 0, 0},
    {62, 0, 0},
    {63, 0, 0},
    {24, 0, 4 | 1},
    {25, 1, 4 | 1},
    {-1},
};
