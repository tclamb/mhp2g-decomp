#pragma opt_unroll_loops on

#include "camera.hpp"
#include "common.h"
#include "enemy_manager.hpp"
#include "hit_manager.hpp"
#include "npc.hpp"
#include "psptypes.h"
#include "system.hpp"
#include "vfpu.h"

void SubCamera::cam_init(u8 type) {
    cam_sub_mode.word = 0;
    cam_sub_impl = 0;
    cam_type = type;
    switch(type) {
    case SubCameraType::STD:
        cam_init_sub_std();
        set_cam_sub(&SubCamera::cam_sub_std);
        break;
    case SubCameraType::GUNNER:
        cam_init_sub_gunner();
        set_cam_sub(&SubCamera::cam_sub_gunner);
        break;
    case SubCameraType::STG:
        cam_init_sub_stg();
        set_cam_sub(&SubCamera::cam_sub_stg);
        break;
    case SubCameraType::PCHNGR:
        cam_init_sub_pchngr();
        set_cam_sub(&SubCamera::cam_sub_pchngr);
        break;
    case SubCameraType::PLAYER_EX:
        cam_init_sub_playerEX();
        set_cam_sub(&SubCamera::cam_sub_playerEX);
        break;
    case SubCameraType::DEMO:
        cam_init_sub_demo();
        set_cam_sub(&SubCamera::cam_sub_demo);
        break;
    };
}

void SubCamera::cam_sub() {
    if (cam_sub_impl) {
        (this->*cam_sub_impl)();
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", cam_init_sub_std__9SubCameraFv);

// cam_sub_std
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", cam_sub_std__9SubCameraFv);

void SubCamera::cam_init_sub_gunner() {
    // empty
}

// cam_sub_gunner
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", cam_sub_gunner__9SubCameraFv);

// cam_init_sub_stg
void SubCamera::cam_init_sub_stg() {
    current_roll = 0;
    current_fov = DEGREES_TO_RADIANS(55);
    cam_sub_stg();
}

// cam_sub_stg
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", cam_sub_stg__9SubCameraFv);

void SubCamera::cam_init_sub_pchngr() {
    current_fov = DEGREES_TO_RADIANS(45);
    current_roll = 0;
    data.pchngr.unknown_0x5C = DEGREES_TO_RADIANS(45);
    data.pchngr.unknown_0x60 = DEGREES_TO_RADIANS(45);
}

// cam_sub_pchngr
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", cam_sub_pchngr__9SubCameraFv);

void SubCamera::cam_init_sub_playerEX() {
    current_fov = DEGREES_TO_RADIANS(55);
    current_roll = 0;
    data.playerEX.fishing_cam.state.word = 0;
    data.playerEX.zoom_cam.state.word = 0;
    data.playerEX.zoom_cam.animationTotalFrames = 15;
    data.playerEX.zoom_cam.animationState = 0;
    data.playerEX.zoom_cam.stackSize = 0;
}

void SubCamera::cam_sub_playerEX() {
    cam_plEX_fishing(data.playerEX.fishing_cam);
    if (!isActive) {
        cam_plEX_zoom(data.playerEX.zoom_cam);
    }
}

void SubCamera::cam_init_sub_demo() {
    current_roll = 0;
    current_fov = DEGREES_TO_RADIANS(55);
    data.demo.demo_id = 0;
    data.demo.demo_state = 0;
    data.demo.enemy = 0;
    data.demo.is_quest_clear = false;
    data.demo.enable_stage_collision = false;
    timer = 0;
}

extern u32 *demo_cam_tbl[107];

void SubCamera::cam_sub_demo() {
    int result;

    DemoCameraData &d = data.demo;
    Camera *c = Camera::objectPtr;
    isActive = false;

    switch (cam_sub_mode.byte) {
    case 0:
        if (c->next_demo_id != 0) {
            d.demo_id = c->next_demo_id;
            d.enemy = c->demo_enemy;
            d.enable_stage_collision = false;
            cam_sub_state.word = 0;
            if (demo_cam_tbl[d.demo_id] == (void *)0xFFFFFFFF) {
                cam_sub_mode.byte = 2;
                result = ex_ev_camera();
            } else {
                cam_sub_mode.byte = 1;
                result = point_camera();
            }
            isActive = true;
            c->next_demo_id = 0;
        }
        break;
    case 1:
        result = point_camera();
        break;
    case 2:
        result = ex_ev_camera();
        break;
    }

    if (cam_sub_mode.byte != 0) {
        if (result <= 0) {
            isActive = true;
            if (result == 0) {
                d.demo_state = 1;
            } else {
                d.demo_state = -1;
            }
        } else {
            cam_sub_mode.byte = 0;
            d.demo_id = 0;
            d.demo_state = 0;
            d.enemy = 0;
            c->zClipping = true;
            d.is_quest_clear = false;
        }
    }
}

// CamRailMove
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888A034);

// cam_rail_move_sub
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888A08C);

// cam_rail_move_0
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888A1A0);

// cam_rail_move
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888A23C);

// cam_plEX_fishing
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", cam_plEX_fishing__9SubCameraFR17FishingCameraData);

// fish_cam_sub
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888A440);

void SubCamera::cam_plEX_zoom(ZoomCameraData &d) {
    Camera *c = Camera::objectPtr;
    Player *pl = c->player;
    u8 type = c->base_sub_type;
    isActive = 0;

    SubCamera *s = &c->subCameras[type];
    copy_q(&d.positions[0], &s->current_position);
    copy_q(&d.targets[0], &s->current_target);
    d.fieldsOfView[0] = s->current_fov;

    switch (d.state.byte) {
    case 0: // push
        if (d.animationState != 0 /* off */) {
            if (d.animationState == 1 /* zooming in */) {
                d.operationFrame = 0;
            }
            d.state.byte += 1;
            if (d.stackSize != 0) {
                copy_q(&d.positions[d.stackSize], &current_position);
                copy_q(&d.targets[d.stackSize], &current_target);
                d.fieldsOfView[d.stackSize] = current_fov;
            }
            d.stackSize++;
            break;
        }
        return;
    case 1: // hold
        if (d.animationState == 3 /* zooming out */) {
            ++d.state.byte;
        } else if (d.operationFrame < d.animationTotalFrames && ++d.operationFrame >= d.animationTotalFrames) {
            d.animationState = 2;
        }
        break;
    case 2: // pop
        if (d.operationFrame >= 1) {
            d.operationFrame--;
        }
        if (d.animationState == 1 /* zooming in */) {
            d.state.byte = 1;
        } else if (d.operationFrame == 0) {
            if (--d.stackSize != 0) {
                d.state.byte = 1;
                d.animationState = 2;
                d.operationFrame = 15;
                d.animationTotalFrames = 15;
            } else {
                d.state.byte = 0;
                d.animationState = 0;
                d.operationFrame = 0;
                return;
            }
        }
        break;
    }

    int i = d.stackSize - 1;
    float t = zoom_cam_rate(d.operationFrame, d.animationTotalFrames, d.state.byte);

    ScePspFVector4 tmp;
    switch (d.targetTypes[i]) {
    case 0: // dialog
        if (d.npcs[i]->pl_type == 4 /* NPC_POOGIE */) {
            cpInterVector2(&d.direction, &pl->position, &d.npcs[i]->position, 0.5f, 0.5f);
            d.direction.y += 64.0f;
        } else {
            cpInterVector2(&d.direction, &pl->position, &d.position, 0.5f, 0.5f);
            d.direction.y += 150.0f;
        }
        copy_q(&current_position, &d.positions[i]);
        break;
    case 1: // item box
    case 4: // book edit hair
    case 5: // book edit clothing
        if (Camera::objectPtr->enableCameraControls == true) {
            float maxY, minY, minX, maxX;
            switch (d.targetTypes[i]) {
            case 1:
            case 5:
                maxY = 160.0f;
                minY =  40.0f;
                minX = 120.0f;
                maxX = 375.0f;
                break;
            case 4:
                maxY = 175.0f;
                minX = minY = 120.0f;
                maxX = 375.0f;
                break;
            }
            if (Pad::BUTTONS & Ctrl::UP) {
                d.yCenters[i] += 2.0f;
                if (d.yCenters[i] > maxY) {
                    d.yCenters[i] = maxY;
                }
            }
            if (Pad::BUTTONS & Ctrl::DOWN) {
                d.yCenters[i] -= 2.0f;
                if (d.yCenters[i] < minY) {
                    d.yCenters[i] = minY;
                }
            }
            if (Pad::BUTTONS & Ctrl::LEFT) {
                d.zSpacings[i] -= 8.0f;
                if (d.zSpacings[i] < minX) {
                    d.zSpacings[i] = minX;
                }
            }
            if (Pad::BUTTONS & Ctrl::RIGHT) {
                d.zSpacings[i] += 8.0f;
                if (d.zSpacings[i] > maxX) {
                    d.zSpacings[i] = maxX;
                }
            }
        }
        // fallthrough
    case 3: // book edit submenu
        copy_q(&d.direction, &pl->position);
        d.direction.y += d.yCenters[i];
        SubVector(&tmp, &d.positions[i], &d.direction);
        flvecNormalize(&tmp);
        vscl_t(&tmp, &tmp, d.zSpacings[i]);
        vadd_q(&tmp, &d.direction, &tmp);
        cpInterVector(&current_position, &tmp, &d.positions[i], t);
        break;
    case 2: // kitchen table
        cpInterVector(&current_position, &d.position, &d.positions[i], t);
        break;
    }

    cpInterVector(&current_target, &d.direction, &d.targets[i], t);
    current_fov = d.fieldOfView * t + d.fieldsOfView[i] * (1.0f - t);
    current_roll = s->current_roll;
    isActive = 1;
}

float SubCamera::zoom_cam_rate(s16 timer, s16 total_timer, u8 state) {
    float t = 1.0f / total_timer * timer;
    switch (state) {
    case 1:
        t = 1.0f - sceVfpuScalarPow(1.0f - t, 2.25f);
        break;
    case 2:
        t = sceVfpuScalarPow(t, 2.25f);
        break;
    }
    return t;
}

// point_camera
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", point_camera__9SubCameraFv);

int SubCamera::point_cam_sub() {
    DemoCameraData &d = data.demo;
    int result;
    ScePspUnion32 *pc;
    bool running = true;
    pc = d.pc;
    d.error = 0;
    while (running) {
        CameraCommand *cmd = reinterpret_cast<CameraCommand *>(pc);
        ScePspUnion32 *old_pc = pc;
        pc += old_pc->c[1];
        switch (old_pc->c[0]) {
        case 0:
            d.move_type = old_pc->c[2];
            break;
        case 1:
            d.pos_offset_type = old_pc->c[2];
            d.pos_offset_bone_id = old_pc->c[3];
            break;
        case 2:
            cmd_set_pos(&d.pos_start, cmd);
            break;
        case 3:
            cmd_set_pos(&d.pos_end, cmd);
            break;
        case 4:
            d.tar_offset_type = old_pc->c[2];
            d.tar_offset_bone_id = old_pc->c[3];
            break;
        case 5:
            cmd_set_tar(&d.tar_start, cmd);
            break;
        case 6:
            cmd_set_tar(&d.tar_end, cmd);
            break;
        case 7:
            d.follow_target = old_pc->c[2];
            break;
        case 8:
            d.pitch_yaw_start.pitch = old_pc->s[1];
            break;
        case 9:
            d.pitch_yaw_end.pitch = old_pc->s[1];
            break;
        case 10:
            d.pitch_yaw_start.yaw = old_pc->s[1];
            break;
        case 11:
            d.pitch_yaw_end.yaw = old_pc->s[1];
            break;
        case 12:
            d.offset_start = 0.0625f * cmd->type4.arg0;
            break;
        case 13:
            d.offset_end = 0.0625f * cmd->type4.arg0;
            break;
        case 14:
            d.roll_start = old_pc->s[1];
            break;
        case 15:
            d.roll_end = old_pc->s[1];
            break;
        case 16:
            d.fov_start = old_pc->s[1];
            break;
        case 17:
            d.fov_end = old_pc->s[1];
            break;
        case 18:
            timer = timer_total = old_pc->s[1];
            break;
        case 19:
            d.truck_shake_phase = old_pc->s[1];
            break;
        case 20:
            d.truck_shake_rate = old_pc->s[1];
            break;
        case 21:
            d.truck_shake_magnitude_start = old_pc->s[1];
            break;
        case 22:
            d.truck_shake_magnitude_end = old_pc->s[1];
            break;
        case 23:
            d.jib_shake_phase = old_pc->s[1];
            break;
        case 24:
            d.jib_shake_rate = old_pc->s[1];
            break;
        case 25:
            d.jib_shake_magnitude_start = old_pc->s[1];
            break;
        case 26:
            d.jib_shake_magnitude_end = old_pc->s[1];
            break;
        case 27:
            d.roll_shake_phase = old_pc->s[1];
            break;
        case 28:
            d.roll_shake_rate = old_pc->s[1];
            break;
        case 29:
            d.roll_shake_magnitude_start = old_pc->s[1];
            break;
        case 30:
            d.roll_shake_magnitude_end = old_pc->s[1];
            break;
        case 31:
            d.fov_shake_phase = old_pc->s[1];
            break;
        case 32:
            d.fov_shake_rate = old_pc->s[1];
            break;
        case 33:
            d.fov_shake_magnitude_start = old_pc->s[1];
            break;
        case 34:
            d.fov_shake_magnitude_end = old_pc->s[1];
            break;
        case 35: {
            s8 pct = old_pc->c[3];
            s8 i = old_pc->c[2];
            d.shake_rngs[i] = pct;
            break;
        }
        case 36:
            cmd_copy(old_pc->c[2]);
            break;
        case 37:
            d.interpolation_type = (old_pc->s[1] >> 14) & 0x3;
            d.interpolation_exponent = 0.0009765625f * (old_pc->s[1] & 0x3FFF);
            break;
        case 38:
            timer = timer_total = old_pc->s[1];
            d.loop_pc = pc;
            break;
        case 39:
            if (old_pc->c[2] != 0) {
                cmd_cam_move(cmd);
            }
            timer--;
            if (timer_total <= 0 || timer >= 0) {
                pc = d.loop_pc;
                running = false;
                result = 0;
            } else {
                copy_q(&d.pos_start, &current_position);
                copy_q(&d.tar_start, &current_target);
                d.pitch_yaw_start = d.pitch_yaw_end;
                d.roll_start = d.roll_end;
                d.fov_start = d.fov_end;
                d.truck_shake_rate = 0;
                d.jib_shake_rate = 0;
                d.roll_shake_rate = 0;
                d.fov_shake_rate = 0;
                d.truck_shake_magnitude_start = d.truck_shake_magnitude_end;
                d.jib_shake_magnitude_start = d.jib_shake_magnitude_end;
                d.roll_shake_magnitude_start = d.roll_shake_magnitude_end;
                d.fov_shake_magnitude_start = d.fov_shake_magnitude_end;
                d.interpolation_type = 0;
                d.interpolation_exponent = 1.0f;
                timer = -1;
                timer_total = -1;
            }
            break;
        case 40:
            cmd_cam_move(cmd);
            break;
        case 43:
            d.enable_stage_collision = true;
            break;
        case 45:
            d.jump_pc = pc;
            break;
        case 44:
            if (d.jump_pc != 0) {
                pc = d.jump_pc;
            }
            break;
        case 41:
        default:
            return 1;
            break;
        case 42:
            running = false;
            result = -1;
            break;
        }
        if (d.error != 0) {
            return 1;
        }
        d.pc = pc;
    }
    return result;
}

// CamRailPoint
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888B304);

// GetRailTarget
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888B3B0);

// GetRailCamPos
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888B500);

// GetNearSection
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888B588);

// GetNearPoint
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888B698);

// get_near_point_sub
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888B844);

// GetOrthogonalPoint
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888B944);

// ZoomRateCalc
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888BF98);

// ZoomBaseAngleRail
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888C01C);

// RollAngleRail
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888C048);

ScePspFMatrix4 SplineRValue[16];

#define subdiag scratch
#define diag (scratch + 16)
#define superdiag (scratch + 32)
#define b1 (scratch + 48)
#define b2 (scratch + 64)
#define b3 (scratch + 80)
#define x1 (scratch + 96)
#define x2 (scratch + 112)
#define x3 (scratch + 128)
void SubCamera::Spline(ScePspFVector4 *p, int n) {
    float scratch[144];

    subdiag[0] = 0;
    superdiag[0] = p[0].w;
    diag[0] = superdiag[0] + superdiag[0];

    b1[0] = (p[1].x - p[0].x) * 3;
    b2[0] = (p[1].y - p[0].y) * 3;
    b3[0] = (p[1].z - p[0].z) * 3;

    for (int i = 1; i < n - 1; i++) {
        float left = p[i].w / p[i - 1].w;
        float right = p[i - 1].w / p[i].w;
        subdiag[i] = p[i].w;
        diag[i] = (p[i - 1].w + p[i].w) * 2;
        superdiag[i] = p[i - 1].w;
        b1[i] = (left * (p[i].x - p[i - 1].x) + right * (p[i + 1].x - p[i].x)) * 3;
        b2[i] = (left * (p[i].y - p[i - 1].y) + right * (p[i + 1].y - p[i].y)) * 3;
        b3[i] = (left * (p[i].z - p[i - 1].z) + right * (p[i + 1].z - p[i].z)) * 3;
    }

    subdiag[n - 1] = p[n - 2].w;
    diag[n - 1] = p[n - 2].w + p[n - 2].w;
    superdiag[n - 1] = 0;
    b1[n - 1] = (p[n - 1].x - p[n - 2].x) * 3;
    b2[n - 1] = (p[n - 1].y - p[n - 2].y) * 3;
    b3[n - 1] = (p[n - 1].z - p[n - 2].z) * 3;

    tri_diag(x1, subdiag, diag, superdiag, b1, n);
    tri_diag(x2, subdiag, diag, superdiag, b2, n);
    tri_diag(x3, subdiag, diag, superdiag, b3, n);

    ScePspFMatrix4 *m = SplineRValue;
    for (int i = 0; i < n - 1; ++i, ++m) {
        float d = 1 / p[i].w;
        float d2 = d * d;
        float d3 = 1 / sceVfpuScalarPow(p[i].w, 3);

        m->x.x = (p[i].x - p[i + 1].x) * 2 * d3 + (x1[i] + x1[i + 1]) * d2;
        m->y.x = (p[i + 1].x - p[i].x) * 3 * d2 - (x1[i] * 2 + x1[i + 1]) * d;
        m->z.x = x1[i];
        m->w.x = p[i].x;

        m->x.y = (p[i].y - p[i + 1].y) * 2 * d3 + (x2[i] + x2[i + 1]) * d2;
        m->y.y = (p[i + 1].y - p[i].y) * 3 * d2 - (x2[i] * 2 + x2[i + 1]) * d;
        m->z.y = x2[i];
        m->w.y = p[i].y;

        m->x.z = (p[i].z - p[i + 1].z) * 2 * d3 + (x3[i] + x3[i + 1]) * d2;
        m->y.z = (p[i + 1].z - p[i].z) * 3 * d2 - (x3[i] * 2 + x3[i + 1]) * d;
        m->z.z = x3[i];
        m->w.z = p[i].z;
    }
}
#undef subdiag
#undef diag
#undef superdiag
#undef b1
#undef b2
#undef b3
#undef x1
#undef x2
#undef x3

void SubCamera::tri_diag(float *out, float *subdiag, float *diag, float *superdiag, float *in, int equations) {
    float scratch[64];

    float divisor = diag[0];
    if (divisor != 0.0f) {
        divisor = 1.0f / divisor;
    }
    out[0] = in[0] * divisor;

    for (int idx = 1; idx < equations; idx++) {
        scratch[idx - 1] = divisor * superdiag[idx - 1];
        divisor = diag[idx] - (subdiag[idx] * scratch[idx - 1]);
        if (divisor != 0.0f) {
            divisor = 1.0f / divisor;
        }
        out[idx] = divisor * (in[idx] - (subdiag[idx] * out[idx + - 1]));
    }

    int idx = equations - 2;
    while (idx >= 0) {
        out[idx] -= (scratch[idx] * out[idx + 1]);
        idx--;
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", ex_ev_camera__9SubCameraFv);

// ex_ev_cam_sub
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888C914);

// std_cam_sw_set_sub
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888CD08);

// GetPanTarget
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888CDAC);

INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888CEC0);

INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888CFB8);

INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888D070);

// Manual_cam_chk
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888D304);

// Fishing_cam_chk
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888D40C);

// pl_falldown_status
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888D438);

// PachiTypeCheck
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888D58C);

// pachingr_mat
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888D65C);

// 10th roots of unity
float D_eboot_089AA1D8[10] = {
    0.95105648f, /* cos(18°) */
    0.309017f, /* sin(18°)*/
    0,
    1.0f,
    -0.95105648f,
    0.309017f,
    -0.58778518,
    -0.809017,
    0.58778518, /* sin(36°) */
    -0.809017, /* cos(36°) */
};

float D_eboot_089AA200[6] = {
    0,
    1.0f / 1,
    1.0f / 2,
    1.0f / 3,
    1.0f / 4,
    1.0f / 5,
};

// DKAS
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888D764);

// Cardano
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888DA18);

// vInnerProductXYZ
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888DECC);

// vInnerProductXZ
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888DEFC);

ScePspFMatrix4 *SubCamera::get_em_local() {
    DemoCameraData &d = data.demo;
    Enemy *e = d.enemy;
    if (e) {
        for (int i = 0; i < 20; ++i) {
            if (EnemyManager::objectPtr->by_index(i) == e) {
                return &e->transform;
            }
        }
    }
    d.error = true;
    return 0;
}

void SubCamera::get_angle(CameraAngle *out) {
    DemoCameraData &d = data.demo;
    if (timer_total > 0) {
        out->pitch = d.pitch_yaw_start.pitch - d.pitch_yaw_end.pitch;
        out->pitch = d.pitch_yaw_end.pitch + out->pitch * timer / timer_total;
        out->yaw = d.pitch_yaw_start.yaw - d.pitch_yaw_end.yaw;
        out->yaw = d.pitch_yaw_end.yaw + out->yaw * timer / timer_total;
    } else {
        *out = d.pitch_yaw_start;
    }
}

// Camera_hokan_start
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888E028);

// Camera_hokan_chk
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888E054);

// Camera_hokan_sub
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888E0E0);

// Cam_senkai_chk
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888E198);

// cmGetGroundHit
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888E2BC);

// SenkaiChousei
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888E338);

void SubCamera::cmd_set_pos(ScePspFVector4 *out, CameraCommand *pc) {
    DemoCameraData &d = data.demo;
    Player *pl = Camera::objectPtr->player;
    float divisor = 1 / 4096.0f;

    out->x = divisor * pc->type444.arg0;
    out->y = divisor * pc->type444.arg1;
    out->z = divisor * pc->type444.arg2;

    Joint *j;
    ScePspFMatrix4 *m;
    switch (d.pos_offset_type) {
    case 2:
        j = pl->hierarchy.roots[0];
        nlCalcPoint(out, out, &j[d.pos_offset_bone_id].globalPose);
        break;
    case 0:
        nlCalcPoint(out, out, &pl->transform);
        break;
    case 1:
        vadd_q(out, out, &pl->position);
        break;
    case 3:
        m = get_em_local();
        if (m != 0) {
            nlCalcPoint(out, out, m);
        }
        break;
    case 4:
        vadd_q(out, out, &d.tar_start);
        break;
    case 5:
    default:
        break;
    }
}

void SubCamera::cmd_set_tar(ScePspFVector4 *out, CameraCommand *pc) {
    DemoCameraData &d = data.demo;
    Player *pl = Camera::objectPtr->player;
    float divisor = 1 / 4096.0f;

    out->x = divisor * pc->type444.arg0;
    out->y = divisor * pc->type444.arg1;
    out->z = divisor * pc->type444.arg2;

    Joint *j;
    ScePspFMatrix4 *m;
    switch (d.tar_offset_type) {
    case 2:
        j = pl->hierarchy.roots[0];
        nlCalcPoint(out, out, &j[d.tar_offset_bone_id].globalPose);
        break;
    case 0:
        nlCalcPoint(out, out, &pl->transform);
        break;
    case 1:
        vadd_q(out, out, &pl->position);
        break;
    case 3:
        m = get_em_local();
        if (m != 0) {
            nlCalcPoint(out, out, m);
        }
        break;
    case 4:
    default:
        break;
    }
}

void SubCamera::cmd_copy(int flags) {
    DemoCameraData &d = data.demo;
    if (flags & 1) {
        copy_q(&d.pos_end, &d.pos_start);
    }
    if (flags & 2) {
        copy_q(&d.tar_end, &d.tar_start);
    }
    if (flags & 4) {
        d.roll_end = d.roll_start;
    }
    if (flags & 8) {
        d.fov_end = d.fov_start;
    }
    if (flags & 16) {
        d.truck_shake_magnitude_end = d.truck_shake_magnitude_start;
    }
    if (flags & 32) {
        d.jib_shake_magnitude_end = d.jib_shake_magnitude_start;
    }
    if (flags & 64) {
        d.roll_shake_magnitude_end = d.roll_shake_magnitude_start;
    }
    if (flags & 128) {
        d.fov_shake_magnitude_end = d.fov_shake_magnitude_start;
    }
}

inline float SubCamera::interpolant() {
    DemoCameraData &d = data.demo;
    float t;
    if (timer_total >= 1) {
        t = (float)timer / timer_total;
        switch (d.interpolation_type) {
        case 1:
            t = sceVfpuScalarPow(t, d.interpolation_exponent);
            break;
        case 2:
            t = 1.0f - sceVfpuScalarPow(1.0f - t, d.interpolation_exponent);
            break;
        case 3:
            if (t < 0.5f) {
                t = 0.5f * sceVfpuScalarPow(2.0f * t, d.interpolation_exponent);
            } else {
                t = 1.0f - 0.5f * sceVfpuScalarPow(2.0f * (1.0f - t), d.interpolation_exponent);
            }
            break;
        default:
            break;
        }
    } else {
        t = 1.0f;
    }
    return t;
}

inline float dummy(const float x) {
    return x;
}

ScePspFVector4 D_eboot_089AA220 = {0, 0, 500.0f, 0};

void SubCamera::cmd_cam_move(CameraCommand *pc) {
    Player *pl = Camera::objectPtr->player;
    DemoCameraData &d = data.demo;

    ScePspFVector4 offset;
    CameraAngle angle;
    ScePspFMatrix4 rotation;
    ScePspFMatrix4 *m;

    const float divisor = 3.1415927f / 32768;
    float t = interpolant();
    float u = 1.0f - t;

    offset = D_eboot_089AA220;
    switch (d.move_type) {
    case 0:
        get_angle(&angle);
        flmatInit(&rotation);
        flmatRotXYZ33(&rotation, divisor * angle.pitch , divisor * angle.yaw, 0);
        if (d.follow_target == 1) {
            cpInterVector(&current_position, &d.pos_start, &d.pos_end, t);
            flvecApplyMat33_2(&offset, &rotation);
            switch (d.pos_offset_type) {
            case 2: {
                Joint *j = pl->hierarchy.roots[0];
                flvecApplyMat33_2(&offset, &j[d.tar_offset_bone_id].globalPose);
                break;
            }
            case 0:
                flvecApplyMat33_2(&offset, &pl->transform);
                break;
            case 3:
                m = get_em_local();
                if (m != 0) {
                    flvecApplyMat33_2(&offset, m);
                }
                break;
            case 1:
            case 4:
            case 5:
            default:
                break;
            }
            vadd_q(&current_target, &current_position, &offset);
        } else {
            cpInterVector(&current_target, &d.tar_start, &d.tar_end, t);
            offset.z = d.offset_start * t + d.offset_end * u;
            flvecApplyMat33_2(&offset, &rotation);
            switch (d.tar_offset_type) {
            case 2: {
                Joint *j = pl->hierarchy.roots[0];
                m =  &j[d.pos_offset_bone_id].globalPose;
                flvecApplyMat33_2(&offset, m);
                break;
            }
            case 0:
                m = &pl->transform;
                flvecApplyMat33_2(&offset, m);
                break;
            case 3:
                m = get_em_local();
                if (m != 0) {
                    flvecApplyMat33_2(&offset, m);
                }
                break;
            case 1:
            case 4:
            default:
                break;
            }
            vadd_q(&current_position, &current_target, &offset);
        }
        break;
    case 1:
        cpInterVector2(&current_target, &d.tar_start, &d.tar_end, t, u);
        cpInterVector2(&current_position, &d.pos_start, &d.pos_end, t, u);
        break;
    }

    if (d.enable_stage_collision) {
        ScePspFVector4 hit;
        if (HitManager::objectPtr->GetWallHitLineCam(&current_target, &current_position, &hit, 0x8009, 28.0f)) {
            copy_q(&current_position, &hit);
        }
        hit.y = HitManager::objectPtr->GetGroundHit(&current_position);
        if (hit.y + 30.0f > current_position.y) {
            current_position.y = hit.y + 30.0f;
        }
    }

    float roll_start = (d.roll_start * divisor);
    roll_start *= t;
    float roll_end = (d.roll_end * divisor);
    roll_end *= u;
    current_roll = roll_start  + roll_end;
    float new_var = 1.0f;
    float fov_start = (d.fov_start * divisor);
    fov_start *= t;
    float fov_end = (d.fov_end * divisor);
    fov_end *= u;
    current_fov = fov_start + fov_end;

    if (d.roll_shake_rate != 0) {
        float shake_start = d.roll_shake_magnitude_start * t;
        float shake_end = d.roll_shake_magnitude_end * u;
        float shake = shake_start + shake_end;
        float dummy_divisor = dummy(divisor);
        shake *= Acos((d.roll_shake_phase * 3.1415927f) / 32768);
        d.roll_shake_phase += d.roll_shake_rate;
        if (d.shake_rng.roll != 0) {
            shake += shake * d.shake_rng.roll * ((u8)System::objectPtr->next_index(1) - 0x80) * (1 / 32640.0f);
        }
        current_roll += dummy_divisor * shake;
    }

    if (d.fov_shake_rate != 0) {
        float shake_start = d.fov_shake_magnitude_start * t;
        float shake_end =   d.fov_shake_magnitude_end * u;
        float shake = shake_start + shake_end;
        float dummy_divisor = dummy(divisor);
        shake *= Acos((d.fov_shake_phase * 3.1415927f) / 32768);
        d.fov_shake_phase += d.fov_shake_rate;
        if (d.shake_rng.fov != 0) {
            float percent = shake * d.shake_rng.fov * ((u8)System::objectPtr->next_index(1) - 0x80) * (1 / 32640.0f);
            shake += percent;
        }
        current_fov += dummy_divisor * shake;
    }

    ScePspFVector4 shift;
    shift.y = 0;
    shift.x = 0;
    u8 has_shift = 0;

    if (d.truck_shake_rate != 0) {
        float shake_start = d.truck_shake_magnitude_start * t;
        float shake_end =   d.truck_shake_magnitude_end * u;
        float shake = shake_start + shake_end;
        shake *= Acos((d.truck_shake_phase * 3.1415927f) / 32768);
        d.truck_shake_phase += d.truck_shake_rate;
        shift.x = shake;
        has_shift |= 1;
        if (d.shake_rng.truck != 0) {
            // unused
            shake += shake * d.shake_rng.truck * ((u8)System::objectPtr->next_index(1) - 0x80) * (1 / 32640.0f);
        }
    }

    if (d.jib_shake_rate != 0) {
        float shake_start = d.jib_shake_magnitude_start * t;
        float shake_end =   d.jib_shake_magnitude_end * u;
        float shake = shake_start + shake_end;
        shake *= Acos((d.jib_shake_phase * 3.1415927f) / 32768);
        d.jib_shake_phase += d.jib_shake_rate;
        shift.y = shake;
        has_shift |= 1;
        if (d.shake_rng.jib != 0) {
            // unused
            shake += shake * d.shake_rng.jib * ((u8)System::objectPtr->next_index(1) - 0x80) * (1 / 32640.0f);
        }
    }

    if (has_shift & 1) {
        ScePspFMatrix4 matrix;
        ScePspFVector4 left;
        ScePspFVector4 up;
        ScePspFVector4 forward;
        ScePspFVector4 quaternion;
        SubVector(&forward, &current_target, &current_position);
        up.z = 0;
        up.y = new_var;
        up.x = 0;
        flvecOuterProduct(&left, &up, &forward);
        flvecNormalize(&left);
        flvecOuterProduct(&up, &forward, &left);
        flvecNormalize(&up);
        ScaleVector(&left, &left, shift.x);
        ScaleVector(&up, &up,  shift.y);
        AddVector(&shift,&left,&up);
        flQuatSetRot2(&forward, current_roll, &quaternion);
        flQuatCnv(&quaternion, &matrix);
        flvecApplyMat33(&offset, &shift, &matrix);
        AddVector(&current_position, &current_position, &offset);
        AddVector(&current_target, &current_target, &offset);
    }
}

// Pl_OoS_Adj
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888F3A4);

// posa_sphere_make
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888F7D0);

// sdc_flag_set
INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888F8D8);

INCLUDE_ASM("asm/eboot/nonmatchings/sub_camera", func_eboot_0888F9E8);
