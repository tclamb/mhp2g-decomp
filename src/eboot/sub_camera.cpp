#include "cockpit.hpp"
#include "pac.hpp"
#include "pad.hpp"
#include "camera.hpp"
#include "common.h"
#include "enemy_manager.hpp"
#include "hit_manager.hpp"
#include "npc.hpp"
#include "psptypes.h"
#include "system.hpp"
#include "vfpu.h"

#pragma opt_unroll_loops on

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

extern CameraFollowDefinition cam_cnf_chs;

void SubCamera::cam_init_sub_std() {
    Camera *c = Camera::objectPtr;
    Player *pl = c->player;
    StdCameraData *d = &data.std;

    ScePspFMatrix4 mat;
    ScePspFVector4 pos_offset;
    ScePspFVector4 direction;
    ScePspIVector4 *pangles;
    ScePspVector4 angles;
    ScePspFVector4 tar_offset;

    active_cam_type = 0xFF;

    if (c->area_cnf->move_type != 0 /* FOLLOW */) {
        d->cnf_chs = &cam_cnf_chs;
    } else {
        d->cnf_chs = &c->area_cnf->height;
    }

    if (c->height_id == 0) {
        d->cnf_chs_entry = &d->cnf_chs->entries[4];
    } else {
        d->cnf_chs_entry = &d->cnf_chs->entries[c->height_id - 1];
    }

    d->fov = d->goal_fov = d->cnf_chs->fov;
    d->roll = d->cnf_chs->roll;
    pangles = &angles.iv;
    d->rotation = pl->rotation.y + 0x8000;

    pangles->x = 0;
    pangles->y = d->rotation;
    pangles->z = 0;
    pangles->w = 0;
    cpRotMatrix(&mat, pangles);
    pos_offset.x = pos_offset.w = 0.0f;
    pos_offset.y = d->cnf_chs_entry->y_offset;
    pos_offset.z = d->cnf_chs_entry->z_offset;
    pos_offset.w = 0;
    flvecApplyMat33(&direction, &pos_offset, &mat);

    d->target.x = pl->position.x;
    d->target.y = pl->position.y + d->cnf_chs_entry->height;
    d->target.z = pl->position.z;

    d->position.x = d->target.x + direction.x;
    d->position.y = pl->position.y + direction.y;
    d->position.z = d->target.z + direction.z;

    d->goal_rotation = d->rotation;

    copy_q(&d->goal_position, &d->position);
    copy_q(&d->goal_target, &d->target);

    d->ground_y = HitManager::objectPtr->GetGroundHit(&d->target);
    d->ground_hit = -1;
    d->is_ground_adj = false;

    copy_q(&d->previous_position, &d->position);

    d->unk_0x88 = 0;
    d->wall_distance = 0.0f;
    d->ground_y_adj = 0.0f;
    d->unk_0x78 = 0;
    d->unk_0x8A = 0;
    d->is_falldown = false;
    d->falldown_timer = 0;
    d->unk_0x94 = 30;
    d->is_kabegiwa = false;
    d->kabegiwa_timer = 0;
    d->is_fast_rotate = false;
    d->is_shoulder_cam = false;
    d->inertia_timer = 0;
}

extern UnalignedCameraFollowHeightEntry cam_cnf_chs_kabegiwa;
extern UnalignedCameraFollowHeightEntry cam_cnf_chs_falldown;

void SubCamera::cam_sub_std() {
    ScePspVector4 ang;
    ScePspFVector4 tmp;
    ScePspFVector4 direction;
    ScePspFMatrix4 mat;
    ScePspFVector4 pl_com;
    ScePspFVector4 target;
    ScePspFVector4 hit2;
    ScePspFVector4 view_hit;
    ScePspFVector4 move_hit;
    ScePspFVector4 adj_pos;
    ScePspFVector4 hit;
    ScePspFVector4 goal_dir;
    ScePspFVector4 dir;
    ScePspFVector4 dir2;
    ScePspFVector4 dir_sq;

    Camera *c = Camera::objectPtr;
    if (c->current_cam_sub != 0 /* STD */) {
        isActive = false;
        cam_sub_mode.byte = 0;
        cam_sub_state.byte = 0;
        return;
    }

    StdCameraData *d = &data.std;
    if ((bool)(GameSys::objectPtr->flags_0x6AF14 & 1 /* LOBBY_TASK */) == true) {
        copy_q(&current_position, &d->position);
        copy_q(&current_target, &d->target);
        current_roll = d->roll;
        current_fov = d->fov;
        return;
    }

    Player *pl = c->player;
    CameraAreaCnf *area = c->area_cnf;

    isActive = true;
    c->is_std_cam = true;
    c->unused_0xA89 = 0xFF;

    sdc_flag_set();
    std_cam_sw_set_sub();

    copy_q(&d->previous_position, &d->position);

    d->goal_fov = d->cnf_chs->fov;

    if (area->move_type != 0 /* FOLLOW */) {
        d->cnf_chs = &cam_cnf_chs;
    } else {
        d->cnf_chs = &area->height;
    }

    u16 height_id;
    if (pl_falldown_status() == true) {
        d->cnf_chs_entry = reinterpret_cast<CameraFollowHeightEntry *>(&cam_cnf_chs_falldown);
        height_id = 7;
    } else if (d->kabegiwa_timer == 60) {
        d->cnf_chs_entry = reinterpret_cast<CameraFollowHeightEntry *>(&cam_cnf_chs_kabegiwa);
        height_id = 6;
    } else {
        height_id = c->height_id;
        if (GameSys::objectPtr->options.cameraSetting == 0 /* NORMAL */) {
            if (d->rising_edge & Ctrl::DOWN && c->height_id > 0) {
                --c->height_id;
            }
            if (d->rising_edge & Ctrl::UP && c->height_id < 4) {
                ++c->height_id;
            }
        } else {
            if (d->rising_edge & Ctrl::UP && c->height_id > 0) {
                --c->height_id;
            }
            if (d->rising_edge & Ctrl::DOWN && c->height_id < 4) {
                ++c->height_id;
            }
        }

        if ((height_id & 0xFFFF) != c->height_id) {
            d->inertia_timer = 8;
            height_id = (u8)c->height_id;
        }
        if (c->height_id == 0) {
            d->cnf_chs_entry = &d->cnf_chs->entries[4];
            d->goal_fov = DEGREES_TO_RADIANS(55);
        } else {
            d->cnf_chs_entry = &d->cnf_chs->entries[c->height_id - 1];
            d->goal_fov = DEGREES_TO_RADIANS(50);
        }
    }

    if (d->kabegiwa_timer != 0) {
        d->goal_fov = DEGREES_TO_RADIANS_F(70);
    }

    bool is_shoulder_cam = false;
    if (c->wyvern_find_player_flag) {
        d->is_view_blocked = 0;
        d->goal_rotation = pl->em_dir + 0x8000;
    } else if (active_cam_type != 0xFF) {
        SubCamera *sc = &c->subCameras[active_cam_type];
        active_cam_type = 0xFF;
        float dz = sc->current_position.z; dz -= sc->current_target.z;
        float dx = sc->current_position.x; dx -= sc->current_target.x;
        d->rotation = d->goal_rotation = AarcTan2(dx, dz);
        d->inertia_timer = 1;
    } else if (c->last_cam_sub != 0 /* STD */ && c->last_cam_sub != 1 /* GUNNER */) {
        float dz = c->last_position.z; dz -= c->last_target.z;
        float dx = c->last_position.x; dx -= c->last_target.x;
        d->rotation = d->goal_rotation = AarcTan2(c->last_position.x - c->last_target.x, c->last_position.z - c->last_target.z);
    } else if (d->buttons & (Ctrl::LEFT | Ctrl::RIGHT)) {
        s16 delta;
        if (GameSys::objectPtr->options.cameraSetting != 2 /* REVERSE_2 */) {
            delta = 1150;
        } else {
            delta = -1150;
        }
        if (d->buttons & Ctrl::LEFT) {
            d->goal_rotation += delta;
        }
        if (d->buttons & Ctrl::RIGHT) {
            d->goal_rotation -= delta;
        }
        d->inertia_timer = 8;
    } else if (Cam_senkai_chk()) {
        d->goal_rotation = pl->rotation.y + 0x8000;
        d->is_fast_rotate = true;
        d->inertia_timer = 20;
        is_shoulder_cam = d->is_shoulder_cam;
    } else if (pl_falldown_status() == false && d->gun_targeting_state >= 0 && GameSys::objectPtr->options.cameraType != 1 /* TYPE_2*/) {
        int delta = (u16)((pl->rotation.y ^ 0x8000) - d->goal_rotation);
        if (d->gun_targeting_state == 0) {
            d->goal_rotation += SenkaiChousei(delta, 4096.0,26624.0, 1.0f / 4608);
        } else {
            d->goal_rotation += SenkaiChousei(delta, 0, 26624.0, 1.0f / 4608);
        }
        is_shoulder_cam = true;
    } else if (d->is_view_blocked) {
        d->goal_rotation += (s16)((pl->rotation.y ^ 0x8000) - d->goal_rotation) >> 3;
        d->is_view_blocked = 0;
    }
    d->is_shoulder_cam = is_shoulder_cam;

    d->rotation += (s16)(d->goal_rotation - d->rotation) >> 2;
    if (d->is_fast_rotate) {
        int goal = d->goal_rotation;
        u16 error = goal - d->rotation;
        if (error < 0x1000U || 0xF000U < error) {
            d->is_fast_rotate = false;
        }
    }

    if (c->last_cam_sub == true && !d->is_shoulder_cam) {
        Camera_hokan_start(20);
        if (GameSys::objectPtr->options.cameraType != 0 /* TYPE_1 */) {
            SubVector(&tmp, &c->subCameras[1 /* GUNNER */].current_position, &pl->position);
            CameraFollowHeightEntry *e = &d->cnf_chs->entries[1];
            float min_score = 1.0e8f;
            int i, min_index = 0;
            for (i = 2; i < 5; ++i, ++e) {
                float delta = e->y_offset - tmp.y;
                delta *= delta;
                if (min_score > delta) {
                    min_index = i;
                    min_score = delta;
                }
            }
            height_id = c->height_id = min_index;
            d->cnf_chs_entry = &d->cnf_chs->entries[min_index];
        }
    }

    tmp.x = 0.0f;
    float goal_y;
    if (pl->em_ride_state != 3 /* JUMP_OFF */ && pl->em_ride_state != 0 /* OFF */) {
        tmp.y = 400.0f;
        tmp.z = 400.0f;
        d->goal_target.y = pl->position.y + 100.0f;
    } else if (pl->em_ride_state != 0
            || pl->act_ck(0 /* NORMAL */, 17 /* HANG_1 */)
            || pl->act_ck(0 /* NORMAL */, 27 /* HANG_2 */)
            || pl->act_ck(0 /* NORMAL */, 30 /* FINISH_CLIMB_FROM_HANG */)
            || pl->act_ck(0 /* NORMAL */, 58 /* CLIMB_FROM_HANG */)) {
        tmp.y = 20.0f;
        tmp.z = d->cnf_chs_entry->z_offset;
        d->goal_target.y = pl->position.y + 30.0f;
    } else {
        CameraFollowHeightEntry *e = d->cnf_chs_entry;
        tmp.y = e->y_offset;
        tmp.z = e->z_offset;
        float height = e->height;
        d->goal_target.y = pl->position.y + height;
    }
    d->goal_target.x = pl->position.x;
    d->goal_target.z = pl->position.z;

    ScePspIVector4 *pangles = &ang.iv;
    pangles->x = pangles->z = 0;
    pangles->y = d->rotation;
    cpRotMatrix(&mat, pangles);
    flvecApplyMat33(&direction, &tmp, &mat);
    d->goal_position.x = d->goal_target.x + direction.x;
    d->goal_position.y = pl->position.y + direction.y;
    d->goal_position.z = d->goal_target.z + direction.z;

    if (cam_sub_mode.byte == 0) {
        ++cam_sub_mode.byte;
        copy_q(&d->position, &d->goal_position);
        copy_q(&d->target, &d->goal_target);
        d->fov = d->goal_fov;
    }
    d->target.x = d->goal_target.x;
    d->target.z = d->goal_target.z;
    d->target.y += (d->goal_target.y - d->target.y) * 0.5f;

    if ((bool)data.std.sdc_flag == true) {
        pl_com.x = pl->position.x;
        pl_com.y = pl->position.y + 60.0f;
        pl_com.z = pl->position.z;
        if (HitManager::objectPtr->GetWallHitLineCam(&pl_com, &d->target, 33.0f, &tmp, 4) != false) {
            copy_q(&d->target, &tmp);
        }
    }

    d->fov += (d->goal_fov - d->fov) * 0.05f;

    float min_height = 100.0f;
    float ground_y = HitManager::objectPtr->GetGroundHit(&d->goal_target);
    if (-(ground_y - d->ground_y) < 210.0f) {
        d->ground_hit = 0;
        d->ground_y = ground_y;
        if (d->is_ground_adj) {
            d->is_ground_adj = false;
            min_height = 256.0f;
        }
    } else {
        switch (d->ground_hit) {
        case 0:
            d->ground_hit = 1;
            // fallthrough
        case 1:
            min_height = 256.0f;
            break;
        default:
            d->ground_hit = 0;
            d->ground_y = ground_y;
            break;
        }
        d->is_ground_adj = false;
    }

    ground_y = cmGetGroundHit(&d->goal_position, pl);
    if (ground_y - d->goal_position.y < min_height) {
        ground_y += d->cnf_chs_entry->min_height;
        if (ground_y > d->goal_position.y) {
            d->goal_position.y = ground_y;
            d->is_ground_adj = true;
        } else {
            d->ground_hit = -1;
        }
    }

    if (pl_falldown_status() == true) {
        d->is_kabegiwa = false;
        d->position.y += (d->goal_position.y - d->position.y) * 0.25f;
        d->position.x = d->goal_position.x;
        d->position.z = d->goal_position.z;
        d->inertia_timer = 8;

        target.x = d->target.x;
        target.y = d->position.y;
        target.z = d->target.z;

        if (HitManager::objectPtr->GetWallHitLineCam(&target, &d->position, 28.0f, &hit2, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */)) {
            copy_q(&d->position, &hit2);
            d->goal_rotation += 1024;
        }
    } else if (!d->is_kabegiwa) {
        bool blocked_move = HitManager::objectPtr->GetWallHitLineCam(&d->position, &d->goal_position, 28.0f, &move_hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */);
        bool blocked_view = HitManager::objectPtr->GetWallHitLineCam(&d->goal_target, &d->goal_position, 28.0f, &view_hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */);
        d->is_view_blocked = blocked_view;
        if (blocked_move && d->is_view_blocked) {
                if (pl_approaching_wall_chk() == true) {
                    copy_q(&d->position, &view_hit);
                    d->is_view_blocked = false;
                    d->is_kabegiwa = true;
                } else {
                    copy_q(&d->position, &move_hit);
                }
        } else {
            d->position.y += (d->goal_position.y - d->position.y) * 0.125f;
            d->position.x = d->goal_position.x;
            d->position.z = d->goal_position.z;
        }
    } else {
        copy_q(&move_hit, &d->goal_target);
        if (d->goal_position.y < move_hit.y) {
            move_hit.y = (move_hit.y + d->goal_position.y) * 0.5f;
        }
        if (HitManager::objectPtr->GetWallHitLineCam(&move_hit, &d->goal_position, 28.0f, &view_hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */) != false) {
            if (d->inertia_timer != 0) {
                copy_q(&d->position, &view_hit);
                copy_q(&d->previous_position, &view_hit);
            } else {
                float goal_distance = d->cnf_chs_entry->z_offset;
                move_hit.x = d->position.x - d->goal_target.x;
                move_hit.z = d->position.z - d->goal_target.z;
                float distance_sq = move_hit.x * move_hit.x + move_hit.z * move_hit.z;
                if (distance_sq > goal_distance * goal_distance) {
                    float distance = vsqrt_s(distance_sq);
                    float multiplier = goal_distance / distance;
                    adj_pos.x = d->goal_target.x + move_hit.x * multiplier;
                    adj_pos.y = d->position.y;
                    adj_pos.z = d->goal_target.z + move_hit.z * multiplier;
                    if (HitManager::objectPtr->GetWallHitLineCam(&d->target, &adj_pos, 28.0f, &view_hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */) != false) {
                        copy_q(&d->position, &view_hit);
                    } else {
                        d->position.x += (adj_pos.x - d->position.x) * (2.0f / 3);
                        d->position.y += (adj_pos.y - d->position.y) * (2.0f / 3);
                        d->position.z += (adj_pos.z - d->position.z) * (2.0f / 3);
                    }
                } else {
                    copy_q(&d->position, &view_hit);
                }
            }
        } else {
            d->position.y += (d->goal_position.y - d->position.y) * 0.125f;
            d->position.x = d->goal_position.x;
            d->position.z = d->goal_position.z;
            d->is_kabegiwa = false;
        }
    }

    if (d->is_kabegiwa == false) {
        HitManager::objectPtr->k_HitEmCamera(&d->position);
        HitManager::objectPtr->k_HitWallCamera(&d->wall_distance, &d->position, &d->previous_position);
    }

    if (d->inertia_timer != 0) {
        --d->inertia_timer;
    }

    if (HitManager::objectPtr->GetWallHitLineCam(&d->position, &d->goal_position, 28.0f, &hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */)) {
        SubVector(&hit, &d->position, &d->target);
        float distance_sq = hit.x * hit.x + hit.z * hit.z;
        float goal_distance = 150.0f + d->cnf_chs_entry->z_offset;
        if (distance_sq > goal_distance * goal_distance) {
            copy_q(&d->position, &d->goal_position);
            copy_q(&d->previous_position, &d->goal_target);
            HitManager::objectPtr->k_HitEmCamera(&d->position);
            HitManager::objectPtr->k_HitWallCamera(&d->wall_distance, &d->position, &d->previous_position);
        }
    }

    if (d->is_kabegiwa && height_id >= 2 && height_id < 5) {
        goal_dir.x = d->goal_target.x - d->goal_position.x;
        goal_dir.z = d->goal_target.z - d->goal_position.z;
        float goal_dist_sq = goal_dir.x * goal_dir.x + goal_dir.z * goal_dir.z;
        dir.x = d->target.x - d->position.x;
        dir.z = d->target.z - d->position.z;
        float dist_sq = dir.x * dir.x + dir.z * dir.z;
        if (goal_dist_sq > dist_sq) {
            float dist = vsqrt_s(dist_sq);
            if (dist > 0.0f) {
                float goal_dist = vsqrt_s(goal_dist_sq);
                d->target.x = d->position.x + dir.x * (goal_dist / dist);
                d->target.z = d->position.z + dir.z * (goal_dist / dist);
            }
        }
    }

    float min_y = cmGetGroundHit(&d->position, pl);
    min_y += d->cnf_chs_entry->min_height;
    if (min_y > d->position.y) {
        d->position.y = min_y;
    }

    if (d->position.y < d->target.y) {
        SubVector(&dir2, &d->target, &d->position);
        dir_sq.x = dir2.x * dir2.x;
        dir_sq.y = dir2.y * dir2.y;
        dir_sq.z = dir2.z * dir2.z;
        float dist_sq = dir_sq.x + dir_sq.y + dir_sq.z;
        if (dist_sq > 0.0f) {
            float cos_pitch = (dir_sq.x + dir_sq.z) / dist_sq;
            if (0.75f > cos_pitch) {
                float delta = vsqrt_s(dir_sq.x + dir_sq.z);
                delta *= 0.57735f;
                d->target.y = d->position.y + delta;
            }
        }
    }

    Camera_hokan_chk(area);
    kabegiwa_cam_chk();

    copy_q(&current_position, &d->position);
    copy_q(&current_target, &d->target);
    current_position.y += func_eboot_0888CEC0();

    switch (cam_sub_state.byte) {
    case 0:
        current_roll = d->roll;
        current_fov = d->fov;
        break;
    case 1:
        copy_q(&last_position, &c->last_position);
        copy_q(&last_target, &c->last_target);
        last_roll = c->last_roll;
        last_fov = c->last_fov;
        ++cam_sub_state.byte;
        // fallthrough
    case 2:
        float t = Camera_hokan_sub();
        float u = 1.0f - t;
        cpInterVector2(&current_position, &last_position, &current_position, t, u);
        cpInterVector2(&current_target, &last_target, &current_target, t, u);
        current_roll = last_roll * t + d->roll * u;
        current_fov = last_fov * t + d->fov * u;
        break;
    }

    Pl_OoS_Adj();
}

void SubCamera::cam_init_sub_gunner() {
    // empty
}

extern ScePspFVector4 D_eboot_08931100;
extern ScePspFVector4 D_eboot_08931110;

void SubCamera::cam_sub_gunner() {
    Camera *c = Camera::objectPtr;
    SubCamera *std = &c->subCameras[0];
    StdCameraData *sd = &std->data.std;
    Player *pl = c->player;
    GunnerCameraData *d = &data.gunner;

    ScePspFVector4 target;
    ScePspFMatrix4 mat;
    ScePspFVector4 direction;
    ScePspFVector4 tar_offset;
    ScePspFVector4 pos_offset;
    ScePspVector4 angles;
    ScePspFVector4 hit;
    ScePspFVector4 tmp;
    ScePspFVector4 position;
    ScePspFVector4 target_xz;
    ScePspFVector4 direction_xz;
    ScePspFVector4 delta;
    s16 std_rotation;
    u16 rot_offset;
    float y_offset, t, u;
    float min_y;
    float num, den, adj;

    if (std->isActive == false) {
        isActive = false;
        cam_sub_mode.word = 0;
        return;
    }

    switch (cam_sub_mode.byte) {
    case 0:
        if (sd->is_shoulder_cam == false) {
            isActive = false;
            cam_sub_mode.word = 0;
            return;
        }
        copy_q(&d->start_position, &std->current_position);
        d->aim_angle = (pl->bowgun_pitch + 100) * 0x22 - 0xB00;
        d->goal_rotation_offset = 0x800;
        d->rotation_offset = 0;
        ++cam_sub_mode.byte;
        cam_sub_state.byte = 1;
        timer = 9;
        hokan_divisor = 1.0f / timer;
        break;
    case 1:
        d->aim_angle += (s16)(((pl->bowgun_pitch + 100) * 0x22 - 0xB00) - d->aim_angle) >> 2;
        if (sd->is_shoulder_cam == false) {
            isActive = false;
            cam_sub_mode.word = 0;
            return;
        }
        break;
    }

    isActive = true;
    c->current_cam_sub = 1 /* GUNNER */;
    c->is_std_cam = false;

    std_rotation = sd->rotation;
    rot_offset = (pl->rotation.y ^ 0x8000) - (std_rotation + d->rotation_offset);
    if (rot_offset > 0x100U && 0x7800U > rot_offset) {
        d->goal_rotation_offset = -0x800;
    } else if (0x8800U < rot_offset && rot_offset < 0xFF00U) {
        d->goal_rotation_offset = 0x800;
    }

    if (d->goal_rotation_offset < 0) {
        if (-0x800 < d->rotation_offset) {
            d->rotation_offset += ((s16)(-0x800 - d->rotation_offset) * 6) >> 6;
        }
    } else {
        if (d->rotation_offset < 0x800) {
            d->rotation_offset += ((s16)(0x800 - d->rotation_offset) * 6) >> 6;
        }
    }

    switch (cam_sub_state.byte) {
    case 0:
        copy_q(&tar_offset, &D_eboot_08931100);
        copy_q(&pos_offset, &D_eboot_08931110);
        y_offset = 170.0f;
        break;
    case 1:
        ++cam_sub_state.byte;
        // fallthrough
    case 2:
        t = Camera_hokan_sub();
        tar_offset.y = tar_offset.x = 0;
        tar_offset.z = sd->cnf_chs_entry->z_offset;
        u = 1.0f - t;
        cpInterVector2(&tar_offset, &tar_offset, &D_eboot_08931100, t, u);
        pos_offset.x = pos_offset.y = pos_offset.z = 0;
        cpInterVector2(&pos_offset, &pos_offset, &D_eboot_08931110, t, u);
        y_offset = 170.0f * u + sd->cnf_chs_entry->height * t;
        break;
    }

    d->target.x = target.x = pl->position.x;
    d->target.y = target.y = pl->position.y + y_offset;
    d->target.z = target.z = pl->position.z;

    angles.iv.x = d->aim_angle;
    angles.iv.y = (sd->rotation + d->rotation_offset);
    angles.iv.z = 0;

    cpRotMatrix(&mat, &angles.iv);
    flvecApplyMat33(&direction, &tar_offset, &mat);
    AddVector(&d->position, &d->target, &direction);

    if (HitManager::objectPtr->GetWallHitLineCam(&target, &d->position, 28.0f, &hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */)) {
        copy_q(&d->position, &hit);
    }

    min_y = cmGetGroundHit(&d->position, pl);
    min_y += sd->cnf_chs_entry->min_height;
    if (min_y > d->position.y) {
        d->position.y = min_y;
    }

    angles.iv.y = sd->rotation;
    cpRotMatrix(&mat, &angles.iv);
    flvecApplyMat33(&direction, &pos_offset, &mat);

    target_xz.x = d->target.x;
    target_xz.z = d->target.z;
    direction_xz.x = direction.x;
    direction_xz.z = direction.z;
    target_xz.y = 0;
    direction_xz.y = 0;

    copy_q(&position, &d->position);

    AddVector(&tmp, &d->target, &direction);

    num = (target_xz.x - tmp.x) * direction_xz.x + (target_xz.y - tmp.y) * direction_xz.y + (target_xz.z - tmp.z) * direction_xz.z;
    delta.x = tmp.x - position.x;
    delta.y = tmp.y - position.y;
    delta.z = tmp.z - position.z;
    den = delta.x * direction_xz.x + delta.y * direction_xz.y + delta.z * direction_xz.z;
    if (den != 0.0f) {
        adj = num / den;
        tmp.x += delta.x * adj;
        tmp.y += delta.y * adj;
        tmp.z += delta.z * adj;
    }
    copy_q(&d->target, &tmp);

    current_roll = std->current_roll;
    current_fov = std->current_fov;
    copy_q(&current_position, &d->position);
    copy_q(&current_target, &d->target);
}

void SubCamera::cam_init_sub_stg() {
    current_roll = 0;
    current_fov = DEGREES_TO_RADIANS(55);
    cam_sub_stg();
}

inline int calc_pan(StgCameraData *d, ScePspFVector4 &current_direction) {
    return AarcTan2(current_direction.x, current_direction.z);
}

inline void calc_hokan(SubCamera *sc, StgCameraData *d) {
    float t, u;
    t = sc->Camera_hokan_sub();
    u = 1.0f - t;
    cpInterVector2(&sc->current_position, &d->last_position, &d->position, t, u);
    cpInterVector2(&sc->current_target, &d->last_target, &d->target, t, u);
    sc->current_roll = d->last_roll * t + d->roll * u;
    sc->current_fov = d->last_fov * t + d->fov * u;
}

inline void calc_target(StgCameraData *d, ScePspFVector4 &current_direction, ScePspFVector4 &tmp) {
    tmp.x = tmp.y = 0;
    tmp.z = flvecCalcLength(&current_direction);
    flvecRotX(&tmp, d->pitch * (float)(PI / 32768));
    flvecRotY(&tmp, d->rotation * (float)(PI / 32768));
    vadd_q(&d->target, &d->position, &tmp);
}

inline float calc_fov(SubCamera *sc, StgCameraData *d, CameraAreaCnf *area, float base_fov) {
    return base_fov * sc->ZoomRateCalc(area, flvecCalcDistance(&d->target, &d->position));
}

ScePspFMatrix4 SplineRValue[16];

void SubCamera::cam_sub_stg() {
    StgCameraData *d = &data.stg;
    CameraAreaCnf *area;
    Camera *c = Camera::objectPtr;
    Player *pl = c->player;

    s32 base_pitch;
    s32 base_rotation;
    float base_fov;
    s16 fov_limit;
    s16 fov_step;
    float fov_tmp;
    s16 error;
    float distance;
    CameraRailDefinition *rail;
    float t, u, divisor;

    isActive = false;

    if (c->current_cam_sub != 2 /* STG */) {
        cam_sub_mode.word = 0;
        return;
    }

    area = c->area_cnf;
    if (area->move_type == 0 /* FOLLOW */) {
        cam_sub_mode.word = 0;
        c->current_cam_sub = 0 /* STD */;
        return;
    }

    ScePspFVector4 tmp;

    switch (cam_sub_mode.byte) {
    case 0:
        ++cam_sub_mode.byte;
        isActive = true;
        cam_sub_state.byte = 0;
        switch (area->move_type) {
        case 1 /* FIXED */:
            d->position.x = area->pan.camera_position.x;
            d->position.y = area->pan.camera_position.y;
            d->position.z = area->pan.camera_position.z;
            d->position.w = 1.0f;
            GetPanTarget(&d->target, area);
            d->fov = c->current_fov;
            break;
        case 3 /* OFFSET */:
            d->position.x = pl->position.x + area->pan.camera_position.x;
            d->position.y = pl->position.y + area->pan.camera_position.y;
            d->position.z = pl->position.z + area->pan.camera_position.z;
            d->position.w = 1.0f;
            GetPanTarget(&d->target, area);
            d->fov = c->current_fov;
            break;
        case 2 /* RAIL */:
            CamRailMove(&pl->position, false);
            CamRailPoint(&tmp, SplineRValue + c->rail_point.spline, c->rail_point.coord);
            GetRailTarget(&d->target, area, &tmp);
            GetRailCamPos(&d->position, area);
            d->fov = c->current_fov;
            break;
        }
        vsub_q(&current_direction, &d->target, &d->position);
        d->rotation = calc_pan(d, current_direction);
        d->pitch = AarcTan2(-current_direction.y, CalcDistanceXZ(&d->target, &d->position));
        d->pitch_adj = d->rotation_adj = 0;
        break;
    case 1:
        isActive = true;
        switch (area->move_type) {
        case 1 /* FIXED */:
            d->position.x = area->pan.camera_position.x;
            d->position.y = area->pan.camera_position.y;
            d->position.z = area->pan.camera_position.z;
            GetPanTarget(&d->target, area);
            base_fov = area->pan.fov;
            d->roll = area->pan.roll;
            break;
        case 3 /* OFFSET */:
            d->position.x = pl->position.x + area->pan.camera_position.x;
            d->position.y = pl->position.y + area->pan.camera_position.y;
            d->position.z = pl->position.z + area->pan.camera_position.z;
            GetPanTarget(&d->target, area);
            base_fov = area->pan.fov;
            d->roll = area->pan.roll;
            break;
        case 2 /* RAIL */:
            rail = &area->rail;
            if (!c->is_changing_area) {
                CamRailMove(&pl->position, true);
            } else {
                CamRailMove(&pl->position, false);
            }
            CamRailPoint(&tmp, SplineRValue + c->rail_point.spline, c->rail_point.coord);
            base_fov = ZoomBaseAngleRail(rail, c->rail_point.spline, c->rail_point.t);
            d->roll = RollAngleRail(rail, c->rail_point.spline, c->rail_point.t);
            GetRailTarget(&d->target, area, &tmp);
            GetRailCamPos(&d->position, area);
            break;
        }
        vsub_q(&current_direction, &d->target, &d->position);
        if (c->is_changing_area != false) {
            d->rotation = calc_pan(d, current_direction);
            d->pitch = AarcTan2(-current_direction.y, CalcDistanceXZ(&d->target, &d->position));
            d->pitch_adj = d->rotation_adj = 0;
        } else {
            struct {
                s16 pitch;
                s16 yaw;
                s16 pitch_adj;
                s16 yaw_adj;
            } base;
            base.yaw = calc_pan(d, current_direction);
            base.pitch = AarcTan2(-current_direction.y, CalcDistanceXZ(&d->target, &d->position));
            fov_limit = d->fov * (float)(32768 / (float)PI / 20);
            fov_step = d->fov * (float)(32768 / (float)PI / 200);
            error = base.pitch - d->pitch;
            if (fov_limit < error) {
                d->pitch = base.pitch - fov_limit;
                d->pitch_adj = fov_step;
            } else if (error < -fov_limit) {
                d->pitch = base.pitch + fov_limit;
                d->pitch_adj = -fov_step / 2;
            }
            distance = d->fov * (float)(32768 / (float)PI / 16);
            fov_limit = distance * (float)(480.0f / 272);
            error = base.yaw - d->rotation;
            if (fov_limit < error) {
                d->rotation_adj = d->rotation;
                d->rotation = base.yaw - fov_limit;
                d->rotation_adj = d->rotation - d->rotation_adj;
                d->rotation_adj -= d->rotation_adj >> 2;
            } else if (error < -fov_limit) {
                d->rotation_adj = d->rotation;
                d->rotation = base.yaw + fov_limit;
                d->rotation_adj = d->rotation - d->rotation_adj;
                d->rotation_adj -= d->rotation_adj >> 2;
            }
        }
        calc_target(d, current_direction, tmp);
        d->fov = calc_fov(this, d, area, base_fov);
        break;
    }

    Camera_hokan_chk(area);

    switch (cam_sub_state.byte) {
    case 0:
        flvecCopy(&current_position, &d->position);
        flvecCopy(&current_target, &d->target);
        current_roll = d->roll;
        current_fov = d->fov;
        break;
    case 1:
        flvecCopy(&d->last_position, &c->last_position);
        flvecCopy(&d->last_target, &c->last_target);
        d->last_roll = c->last_roll;
        d->last_fov = c->last_fov;
        ++cam_sub_state.byte;
        // fallthrough
    case 2:
        calc_hokan(this, d);
        break;
    }
}

void SubCamera::cam_init_sub_pchngr() {
    current_fov = DEGREES_TO_RADIANS(45);
    current_roll = 0;
    data.pchngr.bowgun_fov = DEGREES_TO_RADIANS(45);
    data.pchngr.binoculars_fov = DEGREES_TO_RADIANS(45);
}

struct PachiOffsets {
    ScePspFVector4 pos_offset;
    ScePspFVector4 tar_offset;
};

extern PachiOffsets D_eboot_08931260[4];

void SubCamera::cam_sub_pchngr() {
    Camera *c = Camera::objectPtr;
    PchngrCameraData *d = &data.pchngr;
    Player *pl = c->player;
    isActive = false;
    if ((bool)(pl->attributes & 0x1000 /* SCOPE_CAM */) == false) {
        cam_sub_mode.word = 0;
        return;
    }
    d->pachi_type = PachiTypeCheck();
    switch (cam_sub_mode.byte) {
    case 0:
        d->is_variable = false;
        switch (d->pachi_type) {
        case 0 /* BOWGUN */:
            if (pl->pch_lock_chk() == true) {
                return;
            }
            d->crosshair = 1;
            if (pl->pl_scope_chk() == true) {
                d->is_variable = true;
                d->min_fov = DEGREES_TO_RADIANS_F(20);
                d->max_fov = DEGREES_TO_RADIANS(55);
                d->inv_fov_range = 1.0f / (DEGREES_TO_RADIANS(55) - DEGREES_TO_RADIANS(20));
            }
            break;
        case 1 /* BINOCULARS */:
            d->crosshair = 0;
            d->is_variable = true;
            d->min_fov = DEGREES_TO_RADIANS(30);
            d->max_fov = DEGREES_TO_RADIANS(55);
            d->inv_fov_range = 1.0f / (DEGREES_TO_RADIANS(55) - DEGREES_TO_RADIANS(30));
            break;
        case 2 /* BALLISTA */:
        case 3 /* NET_LAUNCHER */:
            d->crosshair = 0x11;
            current_fov = DEGREES_TO_RADIANS(45);
            break;
        }
        ++cam_sub_mode.byte;
    case 1:
        if (d->is_variable) {
            if (Cockpit::objectPtr->Cockpit_menu_chk() == false) {
                if ((c->buttons & Ctrl::RIGHT) != 0 || (c->buttons & Ctrl::LEFT) != 0) {
                    float *fov;
                    if (d->pachi_type == 0 /* BOWGUN */) {
                        fov = &d->bowgun_fov;
                    } else {
                        fov = &d->binoculars_fov;
                    }
                    if ((c->buttons & Ctrl::LEFT) != 0) {
                        *fov -= DEGREES_TO_RADIANS(1.28);
                        if (*fov < d->min_fov) {
                            *fov = d->min_fov;
                        }
                    }
                    if ((c->buttons & Ctrl::RIGHT) != 0) {
                        *fov += DEGREES_TO_RADIANS(1.28);
                        if (*fov > d->max_fov) {
                            *fov = d->max_fov;
                        }
                    }
                }
            }
        }
        switch (d->pachi_type) {
        case 0 /* BOWGUN */:
            if (((GamePlayer *)pl)->pch_lock_chk() == false) {
                copy_q(&d->player_position, &pl->position);
                ScePspFMatrix4 *wmat = pl->get_joint_wmat(14 /* LEFT_HAND */);
                flmatCopy(&d->mat, wmat);
                d->mat.w.y += 20.0f;
            } else {
                copy_q(&d->player_position, &pl->position);
                ScePspFMatrix4 *aim = ((GamePlayer *)pl)->pch_aim_mat();
                ScePspFMatrix4 *wmat = pl->get_joint_wmat(0 /* BASE */);
                vmmul_q(&d->mat, aim, wmat);
                d->mat.w.y += 20.0f;
            }
            current_fov = d->bowgun_fov;
            break;
        case 1 /* BINOCULARS */:
            pachinger_mat(&d->mat, pl->pchngr_pitch, (s16)(pl->rotation.y + 0x8000), &pl->position);
            current_fov = d->binoculars_fov;
            break;
        case 2 /* BALLISTA */:
        case 3 /* NET_LAUNCHER */:
            pachinger_mat(&d->mat, pl->pchngr_pitch, (s16)(pl->rotation.y + 0x8000), &pl->position);
            break;
        }
        break;
    }
    ScePspFVector4 offset;
    ScePspFVector4 eye_position;
    flmatGetTrans(&eye_position, &d->mat);
    offset.x = D_eboot_08931260[d->pachi_type].pos_offset.x;
    offset.z = D_eboot_08931260[d->pachi_type].pos_offset.z;
    offset.y = 0.0f;
    flvecApplyMat33_2(&offset, &d->mat);
    vadd_q(&current_position, &eye_position, &offset);
    current_position.y += D_eboot_08931260[d->pachi_type].pos_offset.y;
    flvecApplyMat33(&offset, &D_eboot_08931260[d->pachi_type].tar_offset, &d->mat);
    vadd_q(&current_target, &eye_position, &offset);
    current_target.y += D_eboot_08931260[d->pachi_type].pos_offset.y;
    isActive = true;
}

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
    cam_plEX_fishing(&data.playerEX.fishing_cam);
    if (!isActive) {
        cam_plEX_zoom(&data.playerEX.zoom_cam);
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

    DemoCameraData *d = &data.demo;
    Camera *c = Camera::objectPtr;
    isActive = false;

    switch (cam_sub_mode.byte) {
    case 0:
        if (c->next_demo_id != 0) {
            d->demo_id = c->next_demo_id;
            d->enemy = c->demo_enemy;
            d->enable_stage_collision = false;
            cam_sub_state.word = 0;
            if (demo_cam_tbl[d->demo_id] == (void *)0xFFFFFFFF) {
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
                d->demo_state = 1;
            } else {
                d->demo_state = -1;
            }
        } else {
            cam_sub_mode.byte = 0;
            d->demo_id = 0;
            d->demo_state = 0;
            d->enemy = 0;
            c->zClipping = true;
            d->is_quest_clear = false;
        }
    }
}

int SubCamera::CamRailMove(ScePspFVector4 *position, bool compound) {
    CameraAreaCnf *area_cnf = Camera::objectPtr->area_cnf;
    if (area_cnf == NULL) {
        return 0;
    } else if (compound == false) {
        return cam_rail_move_0(&Camera::objectPtr->rail_point, &area_cnf->rail, position);
    } else {
        return cam_rail_move(&Camera::objectPtr->rail_point, &area_cnf->rail, position);
    }
}

void SubCamera::cam_rail_move_sub(CameraRailPoint *point, CameraRailDefinition *rail, int i, float t) {
    float result;
    float x = 0.5f;
    if (point->spline < i) {
        do {
            float w = rail->target_points[point->spline].w;
            w -= point->coord;
            if (w <= x) {
                x -= w;
                ++point->spline;
                point->coord = 0.0f;
            } else {
                result = point->coord + x;
                point->coord = result;
                return;
            }
        } while (point->spline < i);

        if (t - point->coord <= x) {
            point->coord = t;
        } else {
            point->coord = point->coord + x;
        }
    } else {
        do {
            if (point->coord <= x) {
                x -= point->coord;
                --point->spline;
                point->coord = rail->target_points[i].w;
            } else {
                result = point->coord - x;
                point->coord = result;
                return;
            }
        } while (point->spline > i);

        if (point->coord - t <= x) {
            point->coord = t;
        } else {
            point->coord = point->coord - x;
        }
    }
}

int SubCamera::cam_rail_move_0(CameraRailPoint *point, CameraRailDefinition *rail, ScePspFVector4 *position) {
    point->spline = GetNearSection(rail, position);
    if (GetNearPoint(point, rail, position)) {
        point->t = point->coord / rail->target_points[point->spline].w;
        return true;
    } else {
        point->t = 0.0f;
        point->coord = 0.0f;
        return false;
    }
}

int SubCamera::cam_rail_move(CameraRailPoint *point, CameraRailDefinition *rail, ScePspFVector4 *position) {
    CameraRailPoint tmp __attribute__((aligned(16)));
    tmp.spline = GetNearSection(rail, position);
    if (GetNearPoint(&tmp, rail, position) != 0) {
        if (point->spline == tmp.spline) {
            if (point->coord < tmp.coord) {
                if (tmp.coord - point->coord <= 0.5f) {
                    point->coord = tmp.coord;
                } else {
                    point->coord += 0.5f;
                }
            } else if (point->coord > tmp.coord) {
                if (point->coord - tmp.coord <= 0.5f) {
                    point->coord = tmp.coord;
                } else {
                    point->coord -= 0.5f;
                }
            }
        } else {
            cam_rail_move_sub(point, rail, tmp.spline, tmp.coord);
        }
        point->t = point->coord / rail->target_points[point->spline].w;
        return true;
    } else {
        return false;
    }
}

void SubCamera::cam_plEX_fishing(FishingCameraData *d) {
    Player *pl = Camera::objectPtr->player;
    isActive = false;
    if (!(d->checkResult = Fishing_cam_chk())) {
        d->state.byte = 0;
        return;
    }
    if (d->state.byte == 0) {
        ++d->state.byte;
        d->stage_unique = pl->stage_unique;
        if (GameSys::objectPtr->stage_id != stages::SWAMP_N_4) {
            current_fov = DEGREES_TO_RADIANS(55);
        } else {
            current_fov = DEGREES_TO_RADIANS(50);
        }
        current_roll = 0.0f;
    }
    if (fish_cam_sub(d) == true) {
        isActive = true;
    }
}

struct FishingCameraOffsets {
    ScePspFVector3 position;
    ScePspFVector3 target;
};
extern FishingCameraOffsets D_eboot_089312E0[39];
extern float D_eboot_08931688[39];
extern u8 D_eboot_08931724[267];

bool SubCamera::fish_cam_sub(FishingCameraData *d) {
    Player *pl = Camera::objectPtr->player;
    int i = D_eboot_08931724[GameSys::objectPtr->stage_id];
    FishingCameraOffsets *offsets = &D_eboot_089312E0[i];
    float angle = (int)(u16)pl->next_rotation * (float)(PI / 32768);

    current_fov = D_eboot_08931688[i];

    ScePspFVector4 offset;
    offset.x = offsets->position.x;
    offset.y = offsets->position.y;
    offset.z = offsets->position.z;
    flvecRotY(&offset, angle);
    vadd_q(&current_position, &pl->position, &offset);

    offset.x = offsets->target.x;
    offset.y = offsets->target.y;
    offset.z = offsets->target.z;
    flvecRotY(&offset, angle);
    vadd_q(&current_target, &pl->position, &offset);

    return true;
}

void SubCamera::cam_plEX_zoom(ZoomCameraData *d) {
    Camera *c = Camera::objectPtr;
    Player *pl = c->player;
    u8 type = c->current_cam_sub;
    isActive = 0;

    SubCamera *s = &c->subCameras[type];
    copy_q(&d->positions[0], &s->current_position);
    copy_q(&d->targets[0], &s->current_target);
    d->fieldsOfView[0] = s->current_fov;

    switch (d->state.byte) {
    case 0: // push
        if (d->animationState != 0 /* off */) {
            if (d->animationState == 1 /* zooming in */) {
                d->operationFrame = 0;
            }
            d->state.byte += 1;
            if (d->stackSize != 0) {
                copy_q(&d->positions[d->stackSize], &current_position);
                copy_q(&d->targets[d->stackSize], &current_target);
                d->fieldsOfView[d->stackSize] = current_fov;
            }
            d->stackSize++;
            break;
        }
        return;
    case 1: // hold
        if (d->animationState == 3 /* zooming out */) {
            ++d->state.byte;
        } else if (d->operationFrame < d->animationTotalFrames && ++d->operationFrame >= d->animationTotalFrames) {
            d->animationState = 2;
        }
        break;
    case 2: // pop
        if (d->operationFrame >= 1) {
            d->operationFrame--;
        }
        if (d->animationState == 1 /* zooming in */) {
            d->state.byte = 1;
        } else if (d->operationFrame == 0) {
            if (--d->stackSize != 0) {
                d->state.byte = 1;
                d->animationState = 2;
                d->operationFrame = 15;
                d->animationTotalFrames = 15;
            } else {
                d->state.byte = 0;
                d->animationState = 0;
                d->operationFrame = 0;
                return;
            }
        }
        break;
    }

    int i = d->stackSize - 1;
    float t = zoom_cam_rate(d->operationFrame, d->animationTotalFrames, d->state.byte);

    ScePspFVector4 tmp;
    switch (d->targetTypes[i]) {
    case 0: // dialog
        if (d->npcs[i]->pl_type == 4 /* NPC_POOGIE */) {
            cpInterVector2(&d->direction, &pl->position, &d->npcs[i]->position, 0.5f, 0.5f);
            d->direction.y += 64.0f;
        } else {
            cpInterVector2(&d->direction, &pl->position, &d->position, 0.5f, 0.5f);
            d->direction.y += 150.0f;
        }
        copy_q(&current_position, &d->positions[i]);
        break;
    case 1: // item box
    case 4: // book edit hair
    case 5: // book edit clothing
        if (Camera::objectPtr->enableCameraControls == true) {
            float maxY, minY, minX, maxX;
            switch (d->targetTypes[i]) {
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
                d->yCenters[i] += 2.0f;
                if (d->yCenters[i] > maxY) {
                    d->yCenters[i] = maxY;
                }
            }
            if (Pad::BUTTONS & Ctrl::DOWN) {
                d->yCenters[i] -= 2.0f;
                if (d->yCenters[i] < minY) {
                    d->yCenters[i] = minY;
                }
            }
            if (Pad::BUTTONS & Ctrl::LEFT) {
                d->zSpacings[i] -= 8.0f;
                if (d->zSpacings[i] < minX) {
                    d->zSpacings[i] = minX;
                }
            }
            if (Pad::BUTTONS & Ctrl::RIGHT) {
                d->zSpacings[i] += 8.0f;
                if (d->zSpacings[i] > maxX) {
                    d->zSpacings[i] = maxX;
                }
            }
        }
        // fallthrough
    case 3: // book edit submenu
        copy_q(&d->direction, &pl->position);
        d->direction.y += d->yCenters[i];
        SubVector(&tmp, &d->positions[i], &d->direction);
        flvecNormalize(&tmp);
        vscl_t(&tmp, &tmp, d->zSpacings[i]);
        vadd_q(&tmp, &d->direction, &tmp);
        cpInterVector(&current_position, &tmp, &d->positions[i], t);
        break;
    case 2: // kitchen table
        cpInterVector(&current_position, &d->position, &d->positions[i], t);
        break;
    }

    cpInterVector(&current_target, &d->direction, &d->targets[i], t);
    current_fov = d->fieldOfView * t + d->fieldsOfView[i] * (1.0f - t);
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

int SubCamera::point_camera() {
    DemoCameraData *d = &data.demo;
    Camera *c = Camera::objectPtr;

    if (d->is_quest_clear != false
      && (Camera::objectPtr->rising_edge & Ctrl::SELECT) != 0
      && GameSys::objectPtr->is_gallery == false) {
        return 1;
    }

    int result;
    switch (cam_sub_state.byte) {
    case 0:
        ++cam_sub_state.byte;
        if (d->demo_id != 0xFF) {
            d->pc = (ScePspUnion32 *)demo_cam_tbl[d->demo_id];
        }
        copy_q(&current_position, &c->last_position);
        copy_q(&d->pos_start, &c->last_position);
        copy_q(&d->pos_end, &c->last_position);
        copy_q(&current_target, &c->last_target);
        copy_q(&d->tar_start, &c->last_target);
        copy_q(&d->tar_end, &c->last_target);
        current_roll = c->last_roll;
        current_fov = c->last_fov;
        d->roll_start = d->roll_end = current_roll * (float)(32768 / PI);
        d->fov_start = d->fov_end = current_fov * (float)(32768 / PI);
        d->fov_shake_rate = 0;
        d->roll_shake_rate = 0;
        d->jib_shake_rate = 0;
        d->truck_shake_rate = 0;
        *(u32 *)d->shake_rngs = 0;
        d->interpolation_type = 0;
        d->interpolation_exponent = 1.0f;
        d->move_type = 1;
        d->pos_offset_type = 5;
        d->tar_offset_type = 4;
        d->follow_target = false;
        d->pitch_yaw_end.pitch = d->pitch_yaw_start.pitch = 0;
        d->pitch_yaw_end.yaw = d->pitch_yaw_start.yaw = 0;
        d->offset_end = d->offset_start = 512.0f;
        timer = timer_total = -1;
        // fallthrough
    case 1:
        result = point_cam_sub();
        if (result > 0) {
            cam_sub_state.byte += 1;
    case 2:
            result = 1;
        }
        break;
    }
    return result;
}

int SubCamera::point_cam_sub() {
    DemoCameraData *d = &data.demo;
    int result;
    ScePspUnion32 *pc;
    bool running = true;
    pc = d->pc;
    d->error = 0;
    while (running) {
        CameraCommand *cmd = reinterpret_cast<CameraCommand *>(pc);
        ScePspUnion32 *old_pc = pc;
        pc += old_pc->c[1];
        switch (old_pc->c[0]) {
        case 0:
            d->move_type = old_pc->c[2];
            break;
        case 1:
            d->pos_offset_type = old_pc->c[2];
            d->pos_offset_joint_id = old_pc->c[3];
            break;
        case 2:
            cmd_set_pos(&d->pos_start, cmd);
            break;
        case 3:
            cmd_set_pos(&d->pos_end, cmd);
            break;
        case 4:
            d->tar_offset_type = old_pc->c[2];
            d->tar_offset_joint_id = old_pc->c[3];
            break;
        case 5:
            cmd_set_tar(&d->tar_start, cmd);
            break;
        case 6:
            cmd_set_tar(&d->tar_end, cmd);
            break;
        case 7:
            d->follow_target = old_pc->c[2];
            break;
        case 8:
            d->pitch_yaw_start.pitch = old_pc->s[1];
            break;
        case 9:
            d->pitch_yaw_end.pitch = old_pc->s[1];
            break;
        case 10:
            d->pitch_yaw_start.yaw = old_pc->s[1];
            break;
        case 11:
            d->pitch_yaw_end.yaw = old_pc->s[1];
            break;
        case 12:
            d->offset_start = 0.0625f * cmd->type4.arg0;
            break;
        case 13:
            d->offset_end = 0.0625f * cmd->type4.arg0;
            break;
        case 14:
            d->roll_start = old_pc->s[1];
            break;
        case 15:
            d->roll_end = old_pc->s[1];
            break;
        case 16:
            d->fov_start = old_pc->s[1];
            break;
        case 17:
            d->fov_end = old_pc->s[1];
            break;
        case 18:
            timer = timer_total = old_pc->s[1];
            break;
        case 19:
            d->truck_shake_phase = old_pc->s[1];
            break;
        case 20:
            d->truck_shake_rate = old_pc->s[1];
            break;
        case 21:
            d->truck_shake_magnitude_start = old_pc->s[1];
            break;
        case 22:
            d->truck_shake_magnitude_end = old_pc->s[1];
            break;
        case 23:
            d->jib_shake_phase = old_pc->s[1];
            break;
        case 24:
            d->jib_shake_rate = old_pc->s[1];
            break;
        case 25:
            d->jib_shake_magnitude_start = old_pc->s[1];
            break;
        case 26:
            d->jib_shake_magnitude_end = old_pc->s[1];
            break;
        case 27:
            d->roll_shake_phase = old_pc->s[1];
            break;
        case 28:
            d->roll_shake_rate = old_pc->s[1];
            break;
        case 29:
            d->roll_shake_magnitude_start = old_pc->s[1];
            break;
        case 30:
            d->roll_shake_magnitude_end = old_pc->s[1];
            break;
        case 31:
            d->fov_shake_phase = old_pc->s[1];
            break;
        case 32:
            d->fov_shake_rate = old_pc->s[1];
            break;
        case 33:
            d->fov_shake_magnitude_start = old_pc->s[1];
            break;
        case 34:
            d->fov_shake_magnitude_end = old_pc->s[1];
            break;
        case 35: {
            s8 pct = old_pc->c[3];
            s8 i = old_pc->c[2];
            d->shake_rngs[i] = pct;
            break;
        }
        case 36:
            cmd_copy(old_pc->c[2]);
            break;
        case 37:
            d->interpolation_type = (old_pc->s[1] >> 14) & 0x3;
            d->interpolation_exponent = 0.0009765625f * (old_pc->s[1] & 0x3FFF);
            break;
        case 38:
            timer = timer_total = old_pc->s[1];
            d->loop_pc = pc;
            break;
        case 39:
            if (old_pc->c[2] != 0) {
                cmd_cam_move(cmd);
            }
            timer--;
            if (timer_total <= 0 || timer >= 0) {
                pc = d->loop_pc;
                running = false;
                result = 0;
            } else {
                copy_q(&d->pos_start, &current_position);
                copy_q(&d->tar_start, &current_target);
                d->pitch_yaw_start = d->pitch_yaw_end;
                d->roll_start = d->roll_end;
                d->fov_start = d->fov_end;
                d->truck_shake_rate = 0;
                d->jib_shake_rate = 0;
                d->roll_shake_rate = 0;
                d->fov_shake_rate = 0;
                d->truck_shake_magnitude_start = d->truck_shake_magnitude_end;
                d->jib_shake_magnitude_start = d->jib_shake_magnitude_end;
                d->roll_shake_magnitude_start = d->roll_shake_magnitude_end;
                d->fov_shake_magnitude_start = d->fov_shake_magnitude_end;
                d->interpolation_type = 0;
                d->interpolation_exponent = 1.0f;
                timer = -1;
                timer_total = -1;
            }
            break;
        case 40:
            cmd_cam_move(cmd);
            break;
        case 43:
            d->enable_stage_collision = true;
            break;
        case 45:
            d->jump_pc = pc;
            break;
        case 44:
            if (d->jump_pc != 0) {
                pc = d->jump_pc;
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
        if (d->error != 0) {
            return 1;
        }
        d->pc = pc;
    }
    return result;
}

void SubCamera::CamRailPoint(ScePspFVector4 *out, ScePspFMatrix4 *coeff, float t) {
    // cubic polynomial evaluated in nested form:
    // ((x t + y) t + z) t + w
    // == x t^3 + y t^2 + z t + w
    vscl_q(out, &coeff->x, t);
    vadd_q(out, out, &coeff->y);
    vscl_q(out, out, t);
    vadd_q(out, out, &coeff->z);
    vscl_q(out, out, t);
    vadd_q(out, out, &coeff->w);
}

void SubCamera::GetRailTarget(ScePspFVector4 *out, CameraAreaCnf *data, ScePspFVector4 *in) {
    Camera *c = Camera::objectPtr;
    Player *pl = c->player;
    switch (data->target_type) {
    case 0:
        copy_q(out, in);
        break;
    case 1: {
        ScePspFVector4 offset;
        clear(&offset, sizeof(offset));
        ScePspFMatrix4 *wmat = &pl->transform;
        offset.x = data->rail.model_offset.x;
        offset.y = data->rail.model_offset.y;
        offset.z = data->rail.model_offset.z;
        nlCalcPoint(out, &offset, wmat);
        break;
    }
    case 2: {
        ScePspFVector4 woffset;
        clear(&woffset, sizeof(woffset));
        woffset.x = data->rail.world_offset.x;
        woffset.y = data->rail.world_offset.y;
        woffset.z = data->rail.world_offset.z;
        vadd_q(out, &pl->position, &woffset);
        break;
    }
    }
    switch (data->axes) {
    case 0:
        out->y = in->y;
        break;
    case 1:
        out->x = in->x;
        out->z = in->z;
        break;
    }
}

void SubCamera::GetRailCamPos(ScePspFVector4 *out, CameraAreaCnf *data) {
    Camera *c = Camera::objectPtr;
    Spline(data->rail.cam_points, data->rail.count);
    CamRailPoint(out, &SplineRValue[c->rail_point.spline], c->rail_point.t * data->rail.cam_points[c->rail_point.spline].w);
}

int SubCamera::GetNearSection(CameraRailDefinition *rail, ScePspFVector4 *position) {
    union {
        ScePspFVector4 v4;
        ScePspFVector3 v3;
    } point;
    float distances[16];
    float min_distance = 1e+07;
    // bug?
    ScePspFVector3 *p = (ScePspFVector3 *)rail->target_points;
    float *distance = distances;
    int min_index = 0;
    for (int i = 0; i < rail->count; ++i, ++p, ++distance) {
        clear(&point, sizeof(point));
        point.v3 = *p;
        *distance = flvecCalcDistance(position, &point.v4);
        if (min_distance > *distance) {
            min_index = i;
            min_distance = *distance;
        }
    }
    if (min_index != 0 && min_index < rail->count - 1) {
        if (distances[min_index - 1] < distances[min_index + 1]) {
            --min_index;
        }
    } else if (min_index >= rail->count - 1) {
        min_index = rail->count - 2;
    }
    return min_index;
}

int SubCamera::GetNearPoint(CameraRailPoint *point, CameraRailDefinition *rail, ScePspFVector4 *position) {
    float scratch[5];
    Spline(rail->target_points, rail->count);

    int result = GetOrthogonalPoint(scratch, &SplineRValue[point->spline], position, rail->enable_y);
    if (result == 0) {
        point->coord = 0;
        return 0;
    }

    result = get_near_point_sub(point, rail->target_points + point->spline, scratch, result);
    if (result >= 1) {
        if (rail->count - 2 > point->spline) {
            result = GetOrthogonalPoint(scratch, &SplineRValue[point->spline + 1],  position, rail->enable_y);
            if (result != 0) {
                ++point->spline;
                get_near_point_sub(point, rail->target_points + point->spline, scratch, result);
            }
        }
    } else {
        if (result < 0 && point->spline > 0) {
            result = GetOrthogonalPoint(scratch, &SplineRValue[point->spline - 1], position, rail->enable_y);
            if (result != 0) {
                --point->spline;
                get_near_point_sub(point, rail->target_points + point->spline, scratch, result);
            }
        }
    }
    return 1;
}

int SubCamera::get_near_point_sub(CameraRailPoint *point, ScePspFVector4 *section, float *scratch, int n) {
    float x = *scratch, y, z;
    int result;
    if (x < 0.0f) {
        point->coord = 0;
        x = -x;
        result = -1;
    } else {
        y = section->w;
        if (x > y) {
            x -= y;
            point->coord = y;
            result = 1;
        } else {
            point->coord = x;
            return 0;
        }
    }
    if (n == 1) {
        return result;
    }
    while (--n != 0) {
        ++scratch;
        y = *scratch;
        if (y < 0.0f) {
            if (x > -y) {
                point->coord = 0;
                result = -1;
                x = -y;
            }
        } else {
            z = section->w;
            if (y > z) {
                if (x > y - z) {
                    point->coord = z;
                    result = 1;
                    x = y - z;
                }
            } else {
                point->coord = y;
                return 0;
            }
        }
    }
    return result;
}

inline bool too_small(ScePspFVector4 *v) {
    return v->x * v->x + v->y * v->y + v->z * v->z > 1e-09f;
}

inline bool near_zero(float x) {
    return vabs_s(x) < 0.001f;
}

int SubCamera::GetOrthogonalPoint(float *out, ScePspFMatrix4 *section, ScePspFVector4 *position, int enable_y) {
    float coeffs[6];
    ScePspFVector4 pos;
    copy_q(&pos, position);
    if (enable_y == 0) {
        pos.y = 0;
        ScePspFVector4 delta;
        vsub_q(&delta, &section->w, &pos);
        if (!too_small(&section->x)) {
            if (!too_small(&section->y)) {
                if (!too_small(&section->z)) {
                    return 0;
                } else {
                    out[0] = -(vInnerProductXZ(&section->z, &delta) / vInnerProductXZ(&section->z, &section->z));
                    return 1;
                }
            } else {
                coeffs[0] = vInnerProductXZ(&section->y, &section->y) * 2.0f;
                coeffs[1] = vInnerProductXZ(&section->y, &section->z) * 3.0f;
                coeffs[2] = vInnerProductXZ(&section->y, &delta) * 2.0f + vInnerProductXZ(&section->z, &section->z);
                coeffs[3] = vInnerProductXZ(&section->z, &delta);
                return Cardano(out, coeffs);
            }
        } else {
            coeffs[0] = vInnerProductXZ(&section->x, &section->x) * 3.0f;
            coeffs[1] = vInnerProductXZ(&section->x, &section->y) * 5.0f;
            coeffs[2] = vInnerProductXZ(&section->x, &section->z) * 4.0f + vInnerProductXZ(&section->y, &section->y) * 2.0f;
            coeffs[3] = (vInnerProductXZ(&section->x, &delta) + vInnerProductXZ(&section->y, &section->z)) * 3.0f;
            coeffs[4] = vInnerProductXZ(&section->y, &delta) * 2.0f + vInnerProductXZ(&section->z, &section->z);
            coeffs[5] = vInnerProductXZ(&section->z, &delta);
            Complex roots[5];
            DKAS(roots, coeffs);
            int n = 0;
            for (int i = 0; i < 5; ++i) {
                float x = vabs_s(roots[i].im);
                if (x < 0.001f) {
                    out[n++] = roots[i].re;
                }
            }
            return n;
        }
    } else {
        ScePspFVector4 delta;
        vsub_q(&delta, &section->w, &pos);
        if (!too_small(&section->x)) {
            if (!too_small(&section->y)) {
                if (!too_small(&section->z)) {
                    return 0;
                } else {
                    out[0] = -(vInnerProductXYZ(&section->z, &delta) / vInnerProductXYZ(&section->z, &section->z));
                    return 1;
                }
            } else {
                coeffs[0] = vInnerProductXYZ(&section->y, &section->y) * 2.0f;
                coeffs[1] = vInnerProductXYZ(&section->y, &section->z) * 3.0f;
                coeffs[2] = vInnerProductXYZ(&section->y, &delta) * 2.0f + vInnerProductXYZ(&section->z, &section->z);
                coeffs[3] = vInnerProductXYZ(&section->z, &delta);
                return Cardano(out, coeffs);
            }
        } else {
            coeffs[0] = vInnerProductXYZ(&section->x, &section->x) * 3.0f;
            coeffs[1] = vInnerProductXYZ(&section->x, &section->y) * 5.0f;
            coeffs[2] = vInnerProductXYZ(&section->x, &section->z) * 4.0f + vInnerProductXYZ(&section->y, &section->y) * 2.0f;
            coeffs[3] = (vInnerProductXYZ(&section->x, &delta) + vInnerProductXYZ(&section->y, &section->z)) * 3.0f;
            coeffs[4] = vInnerProductXYZ(&section->y, &delta) * 2.0f + vInnerProductXYZ(&section->z, &section->z);
            coeffs[5] = vInnerProductXYZ(&section->z, &delta);
            Complex roots[5];
            DKAS(roots, coeffs);
            int n = 0;
            for (int i = 0; i < 5; ++i) {
                float x = vabs_s(roots[i].im);
                if (x < 0.001f) {
                    out[n++] = roots[i].re;
                }
            }
            return n;
        }
    }
}

float SubCamera::ZoomRateCalc(CameraAreaCnf *d, float distance) {
    if (distance <= d->near_distance) {
        return d->near_fov;
    }

    if (distance >= d->far_distance) {
        return d->far_fov;
    }

    if (d->far_distance == d->near_distance) {
        return (d->near_fov + d->far_fov) * 0.5f;
    }

    float delta = (d->far_fov - d->near_fov) * (distance - d->near_distance) / (d->far_distance - d->near_distance);
    return delta + d->near_fov;
}

float SubCamera::ZoomBaseAngleRail(CameraRailDefinition *definition, int spline, float t) {
    return definition->fovs[spline]* (1.0f - t) + definition->fovs[spline + 1] * t;
}

float SubCamera::RollAngleRail(CameraRailDefinition *definition, int spline, float t) {
    union {
        s16 s[2];
        s32 i;
    } delta;
    int range = definition->rolls[spline + 1];
    s16 start = definition->rolls[spline];

    range -= start;
    delta.i = (int)(t * 65536) * range;
    delta.s[1] += start;

    float result = (float)(PI / 32768);
    result *= delta.s[1];
    return result;
}

void SubCamera::Spline(ScePspFVector4 *p, int n) {
    struct {
        float subdiag[16];
        float diag[16];
        float superdiag[16];
        float b1[16], b2[16], b3[16];
        float x1[16], x2[16], x3[16];
    } f;

    f.subdiag[0] = 0;
    f.superdiag[0] = p[0].w;
    f.diag[0] = f.superdiag[0] + f.superdiag[0];

    f.b1[0] = (p[1].x - p[0].x) * 3;
    f.b2[0] = (p[1].y - p[0].y) * 3;
    f.b3[0] = (p[1].z - p[0].z) * 3;

    for (int i = 1; i < n - 1; i++) {
        float left = p[i].w / p[i - 1].w;
        float right = p[i - 1].w / p[i].w;
        f.subdiag[i] = p[i].w;
        f.diag[i] = (p[i - 1].w + p[i].w) * 2;
        f.superdiag[i] = p[i - 1].w;
        f.b1[i] = (left * (p[i].x - p[i - 1].x) + right * (p[i + 1].x - p[i].x)) * 3;
        f.b2[i] = (left * (p[i].y - p[i - 1].y) + right * (p[i + 1].y - p[i].y)) * 3;
        f.b3[i] = (left * (p[i].z - p[i - 1].z) + right * (p[i + 1].z - p[i].z)) * 3;
    }

    f.subdiag[n - 1] = p[n - 2].w;
    f.diag[n - 1] = p[n - 2].w + p[n - 2].w;
    f.superdiag[n - 1] = 0;
    f.b1[n - 1] = (p[n - 1].x - p[n - 2].x) * 3;
    f.b2[n - 1] = (p[n - 1].y - p[n - 2].y) * 3;
    f.b3[n - 1] = (p[n - 1].z - p[n - 2].z) * 3;

    tri_diag(f.x1, f.subdiag, f.diag, f.superdiag, f.b1, n);
    tri_diag(f.x2, f.subdiag, f.diag, f.superdiag, f.b2, n);
    tri_diag(f.x3, f.subdiag, f.diag, f.superdiag, f.b3, n);

    ScePspFMatrix4 *m = SplineRValue;
    for (int i = 0; i < n - 1; ++i, ++m) {
        float d = 1 / p[i].w;
        float d2 = d * d;
        float d3 = 1 / sceVfpuScalarPow(p[i].w, 3);

        m->x.x = (p[i].x - p[i + 1].x) * 2 * d3 + (f.x1[i] + f.x1[i + 1]) * d2;
        m->y.x = (p[i + 1].x - p[i].x) * 3 * d2 - (f.x1[i] * 2 + f.x1[i + 1]) * d;
        m->z.x = f.x1[i];
        m->w.x = p[i].x;

        m->x.y = (p[i].y - p[i + 1].y) * 2 * d3 + (f.x2[i] + f.x2[i + 1]) * d2;
        m->y.y = (p[i + 1].y - p[i].y) * 3 * d2 - (f.x2[i] * 2 + f.x2[i + 1]) * d;
        m->z.y = f.x2[i];
        m->w.y = p[i].y;

        m->x.z = (p[i].z - p[i + 1].z) * 2 * d3 + (f.x3[i] + f.x3[i + 1]) * d2;
        m->y.z = (p[i + 1].z - p[i].z) * 3 * d2 - (f.x3[i] * 2 + f.x3[i + 1]) * d;
        m->z.z = f.x3[i];
        m->w.z = p[i].z;
    }
}

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

struct CameraScriptExt {
    u16 demo_id;
    u16 pac_offset;
    u16 flags;
};

extern CameraScriptExt D_eboot_08935A7C[10];

int SubCamera::ex_ev_camera() {
    DemoCameraData *d = &data.demo;
    Camera *c = Camera::objectPtr;
    CameraScriptExt *definition = NULL;

    if (d->is_quest_clear != false
      && (Camera::objectPtr->rising_edge & Ctrl::SELECT) != 0
      && GameSys::objectPtr->is_gallery == false) {
        return 1;
    }
    int result = 0;
    switch (cam_sub_state.byte) {
    case 0:
        d->pc = NULL;
        for (int i = 0; D_eboot_08935A7C[i].demo_id != 0xFFFF; ++i) {
            if (d->demo_id == D_eboot_08935A7C[i].demo_id) {
                DataManager::entry *stage = &DataManager::objectPtr->entries[0];
                pac_header *pac;
                if ((stage->flags & 2) != 0) {
                    pac = (pac_header *)stage->buffer;
                } else {
                    pac = NULL;
                }
                if (pac != NULL) {
                    d->pc = (ScePspUnion32 *)pac->data(6 + D_eboot_08935A7C[i].pac_offset);
                    d->ex_ev_id = i;
                    definition = &D_eboot_08935A7C[d->ex_ev_id];
                    break;
                }
            }
        }
        if (d->pc == NULL) {
            result = 1;
            break;
        }
        if ((definition->flags & 4) != 0) {
            d->is_quest_clear = true;
        }
        timer = 0;
        if (c->is_yama_tsukami_quest_clear_0 == true) {
            timer_total = 0;
        } else {
            timer_total = 2;
        }
        cam_sub_state.byte = 1;
        // fallthrough
    case 1:
        ex_ev_cam_sub();
        if (timer_total != 0) {
            --timer_total;
            break;
        }
        cam_sub_state.byte = 2;
        // fallthrough
    case 2:
        if (ex_ev_cam_sub() == false) {
            ++timer;
            break;
        }
        cam_sub_state.byte = 3;
    case 3:
        timer = 0;
        result = 1;
        break;
    }
    return result;
}

struct ExEvKeyframe {
    int time;
    float fov;
    float roll;
    ScePspFVector4 position;
    ScePspFVector4 target;
};

bool SubCamera::ex_ev_cam_sub() {
    Camera *c = Camera::objectPtr;
    DemoCameraData *d;
    int time = 2 * timer;
    ExEvKeyframe *pc = (ExEvKeyframe *)data.demo.pc;
    u8 ev_id = data.demo.ex_ev_id;

    CameraScriptExt *definition = D_eboot_08935A7C + ev_id;

    d = &data.demo;
    if ((definition->flags & 2) != 0)  {
        ScePspFVector4 camera_pos;
        c->get_camera_pos(&camera_pos);
        copy_q(&d->pos_start, &camera_pos);
    }
    while ((bool)(pc->time >= 0)) {
        if (time == pc->time) {
            copy_q(&current_position, &pc->position);
            copy_q(&current_target, &pc->target);
            current_roll = -(pc->roll / 360.f * (float)(2 * PI));
            current_fov = pc->fov / 360.f * (float)(2 * PI);
            break;
        }
        if (time < pc->time) {
            ExEvKeyframe *prev = pc - 1;
            float t = (float)(time - prev->time)/(float)(pc->time - prev->time);
            ScePspFVector4 delta;
            vsub_q(&delta, &pc->position, &prev->position);
            vscl_q(&delta, &delta, t);
            vadd_t(&current_position, &delta, &prev->position);
            vsub_q(&delta, &pc->target, &prev->target);
            vscl_q(&delta, &delta, t);
            vadd_t(&current_target, &delta, &prev->target);
            float roll = pc->roll - prev->roll;
            roll *= t;
            roll += prev->roll;
            current_roll = -(roll / 360.f * (float)(2 * PI));
            float fov = pc->fov - prev->fov;
            fov *= t;
            fov += prev->fov;
            current_fov = fov / 360.f * (float)(2 * PI);
            break;
        }
        ++pc;
    }
    if ((definition->flags & 1) != 0) {
        Enemy *e = d->enemy;
        if (e != NULL) {
            plvecRotY(&current_position, (((e->rotation.y * 360.0f) / 65536) / 360.0f) * (float)(2 * PI));
            plvecRotY(&current_target, (((e->rotation.y * 360.0f) / 65536) / 360.0f) * (float)(2 * PI));
            vadd_t(&current_position, &current_position, &e->position);
            vadd_t(&current_target, &current_target, &e->position);
        }
    }
    if ((definition->flags & 2) != 0) {
        ScePspFVector4 hit;
        if (HitManager::objectPtr->GetWallHitLineCam(&d->pos_start, &current_position, 28.0f, &hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */)) {
            copy_q(&current_position, &hit);
        }
        if (HitManager::objectPtr->GetWallHitLineCam(&current_target, &current_position, 28.0f, &hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */)) {
            copy_q(&current_position, &hit);
        }
        hit.y = HitManager::objectPtr->GetGroundHit(&current_position);
        if (hit.y + 28.0f > current_position.y) {
            current_position.y = hit.y + 28.0f;
        }
    }
    return pc->time < 0;
}

void SubCamera::std_cam_sw_set_sub() {
    Camera *c = Camera::objectPtr;
    StdCameraData *d = &data.std;
    if (Camera::objectPtr->current_cam_sub == 0 && Camera::objectPtr->subCameras[5].isActive == false) {
        if (Manual_cam_chk() == true) {
            d->buttons = c->buttons;
            d->rising_edge = c->rising_edge;
            return;
        }
        if (Osk::objectPtr->visible == false) {
            d->buttons = c->buttons & Ctrl::L_TRIGGER;
            d->rising_edge = c->rising_edge & Ctrl::L_TRIGGER;
            return;
        }
    }
    d->rising_edge = 0;
    d->buttons = 0;
}

void SubCamera::GetPanTarget(ScePspFVector4 *out, CameraAreaCnf *data) {
    Player *pl = Camera::objectPtr->player;
    switch (data->target_type) {
    case 0:
        out->x = data->pan.target_position.x;
        out->y = data->pan.target_position.y;
        out->z = data->pan.target_position.z;
        break;
    case 1: {
        ScePspFVector4 offset;
        ScePspFMatrix4 *wmat = &pl->transform;
        offset.x = data->pan.model_offset.x;
        offset.y = data->pan.model_offset.y;
        offset.z = data->pan.model_offset.z;
        offset.w = 1.0f;
        nlCalcPoint(out, &offset, wmat);
        break;
    }
    case 2:
        out->x = pl->position.x + data->pan.world_offset.x;
        out->y = pl->position.y + data->pan.world_offset.y;
        out->z = pl->position.z + data->pan.world_offset.z;
        out->w = 1.0f;
        break;
    }
    switch (data->axes) {
    case 0:
        out->y = data->pan.target_position.y;
        break;
    case 1:
        out->x = data->pan.target_position.x;
        out->z = data->pan.target_position.z;
        break;
    }
}

float SubCamera::func_eboot_0888CEC0() {
    StdCameraData *d = &data.std;
    ScePspFVector4 hit_pos;
    if (HitManager::objectPtr->cmGetGroundHitLine(&d->position, &Camera::objectPtr->player->hierarchy.roots[0][20 /* HEAD */].globalPose.w, &hit_pos)) {
        float camera_ground_y = HitManager::objectPtr->GetGroundHit(&d->position);
        float target_ground_y = HitManager::objectPtr->GetGroundHit(&d->target);
        if (camera_ground_y - target_ground_y > 150.0f) {
            d->ground_y_adj += 20.0f;
            if (d->ground_y_adj > 300.0f) {
                d->ground_y_adj = 300.0f;
            }
            return d->ground_y_adj;
        }
    }
    d->ground_y_adj -= 20.0f;
    if (d->ground_y_adj < 0.0f) {
        d->ground_y_adj = 0.0f;
    }
    return d->ground_y_adj;
}

bool SubCamera::func_eboot_0888CFB8() {
    StdCameraData *d = &data.std;
    ScePspFVector4 hit_pos, wall_xz, camera_xz;
    if (HitManager::objectPtr->GetWallHitLine(&d->goal_target, &d->goal_position, &hit_pos, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */) != false) {
        copy_q(&wall_xz, &hit_pos);
        copy_q(&camera_xz, &d->goal_position);
        wall_xz.y = 0;
        camera_xz.y = 0;
        float dist = flvecCalcDistance(&wall_xz, &camera_xz);
        if (dist > 50.0f) {
            return true;
        }
    }
    return false;
}

void SubCamera::kabegiwa_cam_chk() {
    Camera *c = Camera::objectPtr;
    StdCameraData *d = &data.std;
    if (d->kabegiwa_timer == 60) {
        if ((bool)data.std.sdc_flag != true) {
            if ((d->rising_edge & (Ctrl::DOWN | Ctrl::UP)) != 0) {
                d->kabegiwa_timer = 0;
            } else {
                if (func_eboot_0888CFB8() == false) {
                    --d->kabegiwa_timer;
                }
            }
        }
    } else {
        if (d->kabegiwa_timer != 0) {
            --d->kabegiwa_timer;
            if ((d->rising_edge & (Ctrl::DOWN | Ctrl::UP)) != 0) {
                d->kabegiwa_timer = 0;
            }
        }
        if ((bool)data.std.sdc_flag == true && func_eboot_0888CFB8() == true) {
            Player *pl = c->player;

            d->kabegiwa_timer = 60;

            ScePspFVector4 delta;
            SubVector(&delta, &pl->next_position, &pl->position);
            if (d->inertia_timer == 0) {
                float angle = flArcTan2(delta.x, delta.z);
                d->goal_rotation = angle * (float)(32768.0f / PI);
            }
        }
    }

}

bool SubCamera::Manual_cam_chk() {
    Player *pl = Camera::objectPtr->player;
    if (Cockpit::objectPtr->is_delivery_box_open != false) {
        return false;
    }
    if (Cockpit::objectPtr->Cockpit_menu_chk() == true) {
        return false;
    }
    if ((bool)(pl->attributes & 2 /* SUPPLY_BOX_OPEN */) == true) {
        return false;
    }
    if ((bool)pl->scope_aim_increment == true) {
        return false;
    }
    if (Fishing_cam_chk() != false) {
        return false;
    }
    if (pl->pl_type == 1 /* B_BOWGUN */ || pl->pl_type == 5 /* B_BOWGUN2 */) {
        if ((bool)(pl->flags_0x410 & 8 /* WEAPON_DRAWN */) != false && (bool)(pl->attributes & 0x8000 /* SHOULDER_CAM */) == true) {
            return false;
        }
    }
    return data.std.gun_targeting_state < 0;
}

bool SubCamera::Fishing_cam_chk() {
    return Camera::objectPtr->player->method_08865C80(0x80000) != 0;
}

bool SubCamera::pl_falldown_status() {
    StdCameraData *d = &data.std;
    Player *pl = Camera::objectPtr->player;
    if (pl->action_type == 2 /* DAMAGE */) {
        d->is_falldown = false;
        return false;
    }
    u8 state = 0;
    if (pl->posture == 2 /* FLY */) {
        state = 1;
        if (pl->position.y < pl->next_position.y - 10.0f) {
            state = 2;
            float ground_y = HitManager::objectPtr->GetGroundHit(&pl->position);
            if (pl->position.y > ground_y + 300.0f) {
                state = 3;
            }
        }
    }
    switch (d->is_falldown) {
    case 0:
        if (state < 3) {
            d->falldown_timer = 4;
            break;
        }
        if (--d->falldown_timer > 0) {
            break;
        }
        ++d->is_falldown;
        // fallthrough
    case 1:
        if ((state & 1) != 0) {
            d->falldown_timer = 15;
            return true;
        }
        if (--d->falldown_timer > 0) {
            return true;
        }
        d->is_falldown = 0;
        break;
    }
    return false;
}

u8 SubCamera::PachiTypeCheck() {
    Player *pl = Camera::objectPtr->player;
    if (pl->act_ck(0 /* NORMAL */, 101 /* USE_BINOCULARS */) == true || pl->act_ck(0 /* NORMAL */, 102 /* USE_BINOCULARS_CROUCHING */) == true) {
        return 1 /* BINOCULARS */;
    } else if (pl->Pl_bari_ck() == true) {
        return 2 /* BALLISTA */;
    } else if (pl->act_ck(8 /* LOBBY? */, 15 /* NET_LAUNCHER */) == true) {
        return 3 /* NET_LAUNCHER */;
    } else if (pl->pl_type == 1 /* B_BOWGUN (heavy bowgun) */ || pl->pl_type == 5 /* B_BOWGUN (light bowgun) */) {
        return 0 /* BOWGUN */;
    } else {
        return -1;
    }
}

void SubCamera::pachinger_mat(ScePspFMatrix4 *out, s32 alpha, s32 beta, ScePspFVector4 *position) {
    flmatInit(out);
    flmatRotXYZ33(out, alpha * (float)(PI / 32768), beta * (float)(PI / 32768), 0.0f);
    copy_q(&out->w, position);
    out->w.w = 1.0f;
}

// 20th roots of unity
DECLSPEC_DATA const float D_eboot_089AA1D8[5][2] = {
    {/* cos( 18°) */  0.95105648f, /* sin( 18°) */ 0.309017f},
    {/* cos( 90°) */  0.0f,        /* sin( 90°) */ 1.0f},
    {/* cos(162°) */ -0.95105648f, /* sin(162°) */ 0.309017f},
    {/* cos(234°) */ -0.58778518f, /* sin(234°) */ -0.809017f},
    {/* cos(306°) */  0.58778518f, /* sin(306°) */ -0.809017f},
};

float D_eboot_089AA200[6] = {
    0,
    1.0f / 1,
    1.0f / 2,
    1.0f / 3,
    1.0f / 4,
    1.0f / 5,
};

inline void dCnvComplex(Complex *out, float re, float im) {
    out->re = re;
    out->im = im;
}

inline void dSubComplex(Complex *out, Complex *lhs, Complex *rhs) {
    out->re = lhs->re - rhs->re;
    out->im = lhs->im - rhs->im;
}

inline void dMulComplex(Complex *out, Complex *lhs, Complex *rhs) {
    Complex tmp;
    float c = rhs->re;
    float b = lhs->im;
    float d = rhs->im;
    float a = lhs->re;
    tmp.re = a * c - b * d;
    tmp.im = a * d + b * c;
    *out = tmp;
}

inline void dDivComplex(Complex *out, Complex *lhs, Complex *rhs) {
    float power;
    Complex tmp;
    float a;
    float b;
    float d = rhs->im;
    float c = rhs->re;

    power = c * c + d * d;
    if (power >= 0.001f) {
        power = 1.0f / power;
        b = lhs->im;
        a = lhs->re;
        tmp.re = power * (a * c + b * d);
        tmp.im = power * (b * c - a * d);
        *out = tmp;
    } else {
        *out = *lhs;
    }
}

// Durand-Kerner method
void SubCamera::DKAS(Complex *out, float *coeffs) {
    Complex denominator, numerator, tmp, difference, error;

    // rescale so the x^5 coefficient is 1
    float scoeffs[6];
    float divisor = 1.0f / coeffs[0];
    for (int i = 1; i <= 5; i++) {
        scoeffs[i] = divisor * coeffs[i];
    }

    // initial guesses for roots
    float radius = 0.0f;
    for (int i = 2; i <= 5; i++) {
        float magnitude = sceVfpuScalarPow(vabs_s(scoeffs[i]), D_eboot_089AA200[i]); // raise values by powers
        if (magnitude > radius) {
            radius = magnitude;
        }
    }
    radius *= 5.0f;
    for (int i = 0; i < 5; ++i) {
        dCnvComplex(out + i, radius * D_eboot_089AA1D8[i][0], radius * D_eboot_089AA1D8[i][1]);
    }

    for (int iters = 25; iters >= 0; iters--) {
        for (int j = 0; j < 5; j++) {
            dCnvComplex(&denominator, 1.0f, 0.0f);            Complex numerator;
            dCnvComplex(&numerator, 1.0f, 0.0f);

            tmp = out[j];
            for (int k = 0; k < 5; k++) {
                dMulComplex(&numerator, &numerator, &tmp); // total kept in &90

                numerator.re += scoeffs[k + 1];

                if (k != j) {
                    dSubComplex(&difference, &tmp, out + k);
                    dMulComplex(&denominator, &denominator, &difference);
                }
            }
            dDivComplex(&error, &numerator, &denominator);
            dSubComplex(out + j, &tmp, &error);
        }
    }
}

u8 D_eboot_089AA218[8] = {}; // pad

int SubCamera::Cardano(float *out, float *coeffs) {
    float divisor, b, c, d,
          b_3, bsq_9,
          P_3, Psq_9, P_3U, Q_2,
          D, U, Usq, sqrt_minus_P_3,
          theta, C, S;

    // rescale so the x^3 coefficient is 1
    divisor = 1.0f / coeffs[0];
    b = coeffs[1] * divisor;
    c = coeffs[2] * divisor;
    d = coeffs[3] * divisor;

    // depressed cubic form t^3 + P t + Q
    // apply the transform x = t - b / 3
    // and collect like terms, yielding:
    // P = c - b^2 / 3
    // Q = 2 b^3 / 27 - b c / 3 + d
    b_3 = b * (1.0f / 3);
    bsq_9 = b_3 * b_3;
    P_3 = c * (1.0f / 3) - bsq_9;
    Q_2 = ((bsq_9 + bsq_9 - c) * b_3 + d) * (1.0f / 2);

    // triple root: t^3 = 0
    if (vabs_s(P_3) < 1.0e-6f && vabs_s(Q_2) < 1.0e-6f) {
        out[0] = -b_3;
        return 1;
    }

    // the discriminant is -(4 P^3 + 27 Q^2)
    // a scaled version is convenient here
    // D = - discriminant / 108
    // as it's the radicand in Cardano's formula
    Psq_9 = P_3 * P_3;
    D = Psq_9 * P_3 + Q_2 * Q_2;

    // negative discriminant: one real root
    if (D > 0.0f) {
        if ((Q_2 >= 0.0f)) {
            U = sceVfpuScalarPow(Q_2 + vsqrt_s(D), (1.0f / 3));
        } else {
            U = -sceVfpuScalarPow(-Q_2 + vsqrt_s(D), (1.0f / 3));
        }
        P_3U = P_3 / U;
        if (P_3 < 0.0f) {
            out[0] = P_3U - U;
        } else {
            Usq = U * U;
            out[0] = -2.0f * Q_2 * Usq / ((Usq + P_3) * Usq + Psq_9);
        }
        out[0] -= b_3;
        return 1;
    }

    if (Q_2 < 0.0f) {
        sqrt_minus_P_3 = vsqrt_s(-P_3);
    } else {
        sqrt_minus_P_3 = -vsqrt_s(-P_3);
    }

    // zero discriminant: 2 roots
    if (vabs_s(D) < 1.0e-6f) {
        out[0] = sqrt_minus_P_3 + sqrt_minus_P_3 - b_3;
        out[1] = -sqrt_minus_P_3 - b_3; // double root
        return 2;
    }

    // positive discriminant: 3 roots
    // casus irreducibilis, fall back to a trigonometric solution
    // derived from substituting t = r cos(θ),
    // identifying coefficients with the triple angle identity, and
    // obtaining two more solutions from the first by adding/subtracting 2π/3
    // (these angles also satisfy the triple angle identity)
    theta = flArcTan2(vsqrt_s(-D), -Q_2) * (1.0f / 3);
    C = sqrt_minus_P_3 * flCos(theta);
    S = sqrt_minus_P_3 * flSin(theta) * (1.7320508f /* 2 * sin(PI/3) */);
    out[0] = (+C + C) - b_3;
    out[1] = (-C - S) - b_3;
    out[2] = (-C + S) - b_3;
    return 3;
}

float SubCamera::vInnerProductXYZ(ScePspFVector4 *a, ScePspFVector4 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float SubCamera::vInnerProductXZ(ScePspFVector4 *a, ScePspFVector4 *b) {
    return a->x * b->x + a->z * b->z;
}

ScePspFMatrix4 *SubCamera::get_em_local() {
    DemoCameraData *d = &data.demo;
    Enemy *e = d->enemy;
    if (e) {
        for (int i = 0; i < 20; ++i) {
            if (EnemyManager::objectPtr->by_index(i) == e) {
                return &e->transform;
            }
        }
    }
    d->error = true;
    return 0;
}

void SubCamera::get_angle(CameraAngle *out) {
    DemoCameraData *d = &data.demo;
    if (timer_total > 0) {
        out->pitch = d->pitch_yaw_start.pitch - d->pitch_yaw_end.pitch;
        out->pitch = d->pitch_yaw_end.pitch + out->pitch * timer / timer_total;
        out->yaw = d->pitch_yaw_start.yaw - d->pitch_yaw_end.yaw;
        out->yaw = d->pitch_yaw_end.yaw + out->yaw * timer / timer_total;
    } else {
        *out = d->pitch_yaw_start;
    }
}

void SubCamera::Camera_hokan_start(int steps) {
    cam_sub_state.byte = 1;
    timer = steps - 1;
    hokan_divisor = 1.0f / steps;
}

void SubCamera::Camera_hokan_chk(CameraAreaCnf *area) {
    if (Camera::objectPtr->is_changing_area == false) {
        return;
    }
    int steps = 15;
    if (area != NULL && area->hokan_info != NULL) {
        int n = 16;
        CameraHokanInfo *info = area->hokan_info;
        for (int i = 16; i != 0; --i, ++info) {
            if (info->area_id == Camera::objectPtr->area_id) {
                steps = info->steps;
                break;
            }
        }
    }
    switch (steps) {
    case 0:
        break;
    case 1:
        break;
    default:
        Camera_hokan_start(steps);
        return;
    }
    cam_sub_state.byte = 0;
}

float SubCamera::Camera_hokan_sub() {
  float t = timer * hokan_divisor;
  if (t < 0.5f) {
    t = 0.5f * sceVfpuScalarPow(t * 2.0f, 2.0f);
  }
  else {
    t = 1.0f - sceVfpuScalarPow((1.0f - t) * 2.0f, 2.0f) * 0.5f;
  }
  if (--timer == 0) {
    cam_sub_state.byte = 0;
  }
  return t;
}

bool SubCamera::Cam_senkai_chk() {
    Player *pl = Camera::objectPtr->player;
    StdCameraData *d = &data.std;
    if (pl->Pl_ClimbChk(pl) == true && d->is_kabegiwa != false) {
        return true;
    }
    if (GameSys::objectPtr->Game_clear_ck(1) == true || pl->dialog_timer != 0) {
        return false;
    }
    if (pl->Pl_senkai_chk() == true) {
        return true;
    }
    if (pl->pl_type == 10 /* BOW */
        && (d->rising_edge & Ctrl::R_TRIGGER)
        && (pl->buttons & Ctrl::R_TRIGGER)
        && (bool)(pl->flags_0x410 & 8 /* WEAPON_DRAWN */) != false
        && pl->action_type != 2 /* DAMAGE */
        && pl->action_type != 6 /* CHAT? */) {
        return true;
    }
    return false;
}

float SubCamera::cmGetGroundHit(ScePspFVector4 *camera_position, Player *player) {
    float ground_y = HitManager::objectPtr->GetGroundHit(camera_position);
    if (player->position.y <= ground_y) {
        return ground_y;
    }
    ScePspFVector4 raised;
    copy_q(&raised, camera_position);
    raised.y += 150.0f;
    return HitManager::objectPtr->GetGroundHit(&raised);
}

s16 SubCamera::SenkaiChousei(u32 angle, float min, float max, float rate) {
    if (angle <= min) {
        return 0;
    }
    if (!(angle < max)) {
        if (angle <= 65536.0f - max) {
            return 0;
        }
        if (angle < 65536.0f - min) {
            angle = 0x10000 - angle;
            rate = -rate;
        } else {
            return 0;
        }
    }
    float a = ((s32)angle - min);
    float b = (max - (s32)angle);
    return rate * (a * a * (1 / 65536.0f) * (b * b * (1 / 65536.0f)));
}

void SubCamera::cmd_set_pos(ScePspFVector4 *out, CameraCommand *pc) {
    DemoCameraData *d = &data.demo;
    Player *pl = Camera::objectPtr->player;
    float divisor = 1 / 4096.0f;

    out->x = divisor * pc->type444.arg0;
    out->y = divisor * pc->type444.arg1;
    out->z = divisor * pc->type444.arg2;

    Joint *j;
    ScePspFMatrix4 *m;
    switch (d->pos_offset_type) {
    case 2:
        j = pl->hierarchy.roots[0];
        nlCalcPoint(out, out, &j[d->pos_offset_joint_id].globalPose);
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
        vadd_q(out, out, &d->tar_start);
        break;
    case 5:
    default:
        break;
    }
}

void SubCamera::cmd_set_tar(ScePspFVector4 *out, CameraCommand *pc) {
    DemoCameraData *d = &data.demo;
    Player *pl = Camera::objectPtr->player;
    float divisor = 1 / 4096.0f;

    out->x = divisor * pc->type444.arg0;
    out->y = divisor * pc->type444.arg1;
    out->z = divisor * pc->type444.arg2;

    Joint *j;
    ScePspFMatrix4 *m;
    switch (d->tar_offset_type) {
    case 2:
        j = pl->hierarchy.roots[0];
        nlCalcPoint(out, out, &j[d->tar_offset_joint_id].globalPose);
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
    DemoCameraData *d = &data.demo;
    if (flags & 1) {
        copy_q(&d->pos_end, &d->pos_start);
    }
    if (flags & 2) {
        copy_q(&d->tar_end, &d->tar_start);
    }
    if (flags & 4) {
        d->roll_end = d->roll_start;
    }
    if (flags & 8) {
        d->fov_end = d->fov_start;
    }
    if (flags & 16) {
        d->truck_shake_magnitude_end = d->truck_shake_magnitude_start;
    }
    if (flags & 32) {
        d->jib_shake_magnitude_end = d->jib_shake_magnitude_start;
    }
    if (flags & 64) {
        d->roll_shake_magnitude_end = d->roll_shake_magnitude_start;
    }
    if (flags & 128) {
        d->fov_shake_magnitude_end = d->fov_shake_magnitude_start;
    }
}

inline float SubCamera::interpolant() {
    DemoCameraData *d = &data.demo;
    float t;
    if (timer_total >= 1) {
        t = (float)timer / timer_total;
        switch (d->interpolation_type) {
        case 1:
            t = sceVfpuScalarPow(t, d->interpolation_exponent);
            break;
        case 2:
            t = 1.0f - sceVfpuScalarPow(1.0f - t, d->interpolation_exponent);
            break;
        case 3:
            if (t < 0.5f) {
                t = 0.5f * sceVfpuScalarPow(2.0f * t, d->interpolation_exponent);
            } else {
                t = 1.0f - 0.5f * sceVfpuScalarPow(2.0f * (1.0f - t), d->interpolation_exponent);
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
    DemoCameraData *d = &data.demo;

    ScePspFVector4 offset;
    CameraAngle angle;
    ScePspFMatrix4 rotation;
    ScePspFMatrix4 *m;

    const float divisor = 3.1415927f / 32768;
    float t = interpolant();
    float u = 1.0f - t;

    offset = D_eboot_089AA220;
    switch (d->move_type) {
    case 0:
        get_angle(&angle);
        flmatInit(&rotation);
        flmatRotXYZ33(&rotation, divisor * angle.pitch , divisor * angle.yaw, 0);
        if (d->follow_target == 1) {
            cpInterVector(&current_position, &d->pos_start, &d->pos_end, t);
            flvecApplyMat33_2(&offset, &rotation);
            switch (d->pos_offset_type) {
            case 2: {
                Joint *j = pl->hierarchy.roots[0];
                flvecApplyMat33_2(&offset, &j[d->tar_offset_joint_id].globalPose);
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
            cpInterVector(&current_target, &d->tar_start, &d->tar_end, t);
            offset.z = d->offset_start * t + d->offset_end * u;
            flvecApplyMat33_2(&offset, &rotation);
            switch (d->tar_offset_type) {
            case 2: {
                Joint *j = pl->hierarchy.roots[0];
                m =  &j[d->pos_offset_joint_id].globalPose;
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
        cpInterVector2(&current_target, &d->tar_start, &d->tar_end, t, u);
        cpInterVector2(&current_position, &d->pos_start, &d->pos_end, t, u);
        break;
    }

    if (d->enable_stage_collision) {
        ScePspFVector4 hit;
        if (HitManager::objectPtr->GetWallHitLineCam(&current_target, &current_position, 28.0f, &hit, 0x8000 | 0x8 | 0x1 /* CLIMBING_WALL | CEILING | FLOOR */)) {
            copy_q(&current_position, &hit);
        }
        hit.y = HitManager::objectPtr->GetGroundHit(&current_position);
        if (hit.y + 30.0f > current_position.y) {
            current_position.y = hit.y + 30.0f;
        }
    }

    float roll_start = (d->roll_start * divisor);
    roll_start *= t;
    float roll_end = (d->roll_end * divisor);
    roll_end *= u;
    current_roll = roll_start  + roll_end;
    float new_var = 1.0f;
    float fov_start = (d->fov_start * divisor);
    fov_start *= t;
    float fov_end = (d->fov_end * divisor);
    fov_end *= u;
    current_fov = fov_start + fov_end;

    if (d->roll_shake_rate != 0) {
        float shake_start = d->roll_shake_magnitude_start * t;
        float shake_end = d->roll_shake_magnitude_end * u;
        float shake = shake_start + shake_end;
        float dummy_divisor = dummy(divisor);
        shake *= flCos((d->roll_shake_phase * 3.1415927f) / 32768);
        d->roll_shake_phase += d->roll_shake_rate;
        if (d->shake_rng.roll != 0) {
            shake += shake * d->shake_rng.roll * ((u8)System::objectPtr->next_index(1) - 0x80) * (1 / 32640.0f);
        }
        current_roll += dummy_divisor * shake;
    }

    if (d->fov_shake_rate != 0) {
        float shake_start = d->fov_shake_magnitude_start * t;
        float shake_end =   d->fov_shake_magnitude_end * u;
        float shake = shake_start + shake_end;
        float dummy_divisor = dummy(divisor);
        shake *= flCos((d->fov_shake_phase * 3.1415927f) / 32768);
        d->fov_shake_phase += d->fov_shake_rate;
        if (d->shake_rng.fov != 0) {
            float percent = shake * d->shake_rng.fov * ((u8)System::objectPtr->next_index(1) - 0x80) * (1 / 32640.0f);
            shake += percent;
        }
        current_fov += dummy_divisor * shake;
    }

    ScePspFVector4 shift;
    shift.y = 0;
    shift.x = 0;
    u8 has_shift = 0;

    if (d->truck_shake_rate != 0) {
        float shake_start = d->truck_shake_magnitude_start * t;
        float shake_end =   d->truck_shake_magnitude_end * u;
        float shake = shake_start + shake_end;
        shake *= flCos((d->truck_shake_phase * 3.1415927f) / 32768);
        d->truck_shake_phase += d->truck_shake_rate;
        shift.x = shake;
        has_shift |= 1;
        if (d->shake_rng.truck != 0) {
            // unused
            shake += shake * d->shake_rng.truck * ((u8)System::objectPtr->next_index(1) - 0x80) * (1 / 32640.0f);
        }
    }

    if (d->jib_shake_rate != 0) {
        float shake_start = d->jib_shake_magnitude_start * t;
        float shake_end =   d->jib_shake_magnitude_end * u;
        float shake = shake_start + shake_end;
        shake *= flCos((d->jib_shake_phase * 3.1415927f) / 32768);
        d->jib_shake_phase += d->jib_shake_rate;
        shift.y = shake;
        has_shift |= 1;
        if (d->shake_rng.jib != 0) {
            // unused
            shake += shake * d->shake_rng.jib * ((u8)System::objectPtr->next_index(1) - 0x80) * (1 / 32640.0f);
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

inline float viewport_bottom_z(float &current_fov) {
    // oops, should be cos/sin
    float asin = sceVfpuScalarAsin(current_fov * 0.5f);
    float acos = sceVfpuScalarAcos(current_fov * 0.5f);
    float not_tan = acos / asin;
    return not_tan * (272.0f / 2);
}

void SubCamera::Pl_OoS_Adj() {
    ScePspFMatrix4 look;
    ScePspFMatrix4 inverse_look;
    ScePspFVector4 player_joint_position;
    ScePspFVector4 position;
    ScePspFVector4 down_back;
    ScePspFVector4 down_forward;
    ScePspFVector4 left;
    ScePspFVector4 cam2joint;
    ScePspFVector4 quaternion;

    SubVector(&current_direction, &current_target, &current_position);
    if (posa_sphere_make(&player_joint_position) == true) {
        Roll2Upvec(&current_up, &current_position, &current_target, current_roll);
        flmatMakeLookAt(&look, &current_position, &current_target, &current_up);
        flmatInvert(&inverse_look, &look);
        float z = viewport_bottom_z(current_fov);
        copy_q(&position, &current_position);
        down_back.x = 0;
        down_back.y = -z;
        down_back.z = 272.0f / 2;
        down_back.w = 0;
        flvecApplyMat33_2(&down_back, &inverse_look);
        flvecNormalize(&down_back);
        down_forward.x = 0;
        down_forward.y = -272.0f / 2;
        down_forward.z = -z;
        down_forward.w = 0;
        flvecApplyMat33_2(&down_forward, &inverse_look);
        flvecNormalize(&down_forward);
        SubVector(&cam2joint, &player_joint_position, &position);
        float y = flvecInnerProduct(&cam2joint, &down_back);
        if (y > 0.0f) {
            float x = flvecInnerProduct(&cam2joint, &down_forward);
            flvecOuterProduct(&left, &down_forward, &down_back);
            float theta = flArcTan2(y, x);
            sceVfpuQuaternionFromRotate(&quaternion, &left, theta);
            sceVfpuQuaternionToMatrix(&look, &quaternion);
            flvecApplyMat33(&left, &current_direction, &look);
            AddVector(&current_target, &current_position, &left);
        }
    }
}

bool SubCamera::posa_sphere_make(ScePspFVector4 *joint_pos) {
    Player *pl = Camera::objectPtr->player;

    pl->get_joint_pos(joint_pos, 20 /* HEAD */);
    if (joint_pos->y >= current_position.y) {
        return false;
    }

    ScePspFVector4 joint_direction;
    SubVector(&joint_direction, joint_pos, &current_position);
    if (flvecInnerProduct(&current_direction, &joint_direction) > 0.0f) {
        return true;
    }

    pl->get_joint_pos(joint_pos, 2 /* HIPS */);
    SubVector(&joint_direction, joint_pos, &current_position);
    if (flvecInnerProduct(&current_direction, &joint_direction) > 0.0f) {
        return true;
    }

    return false;
}

void SubCamera::sdc_flag_set() {
    Player *pl = Camera::objectPtr->player;
    StdCameraData *d = &data.std;

    d->sdc_flag = false;

    if ((bool)(GameSys::objectPtr->flags_0x6AF14 & 1 /* LOBBY_TASK */) == false) {
        bool flung;
        if (pl->action_type != 2 /* DAMAGE */) {
            flung = false;
        } else {
            switch (pl->action_id) {
                case 2:  /* backward (e.g. lance charge) */
                case 5:  /* forward */
                case 7:  /* up & backward (e.g. hammer golfswing) */
                case 14: /* up & forward */
                    flung = true;
                    break;
                default:
                    flung = false;
                    break;
            }
        }
        if (flung == true) {
            d->sdc_flag = true;
        }
    }

    s8 state;
    if (d->sdc_flag == false) {
        state = pl->Gun_targeting_stat();
    } else {
        state = -1;
    }

    if (state >= 0) {
        if (d->gun_targeting_state < 0 && --d->shoulder_cam_timer > 0) {
            state = -1;
        }
    } else {
        d->shoulder_cam_timer = 3;
    }
    d->gun_targeting_state = state;
}

bool SubCamera::pl_approaching_wall_chk() {
    StdCameraData *d = &data.std;
    Player *pl = Camera::objectPtr->player;
    if (d->inertia_timer != 0) {
        return true;
    }
    u32 angle = pl->rotation.y;
    angle -= d->goal_rotation;
    angle = (u16) angle;
    if (0x4000U > angle) {
        return true;
    } else {
        return angle > 0xC000U;
    }
}
