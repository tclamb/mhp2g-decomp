#include "camera.hpp"

#include "cockpit.hpp"
#include "common.h"
#include "enemy.hpp"
#include "gamestage.hpp"
#include "ge.hpp"
#include "game_sys.hpp"
#include "pad.hpp"
#include "player.hpp"
#include "player_manager.hpp"
#include "psptypes.h"
#include "quest.hpp"
#include "vfpu.h"

#pragma opt_unroll_loops on

template<> Camera *Singleton<Camera>::objectPtr;

Camera::Camera() {
    zClipping = true;
    viewport_offset.x = 1808;
    viewport_offset.y = 1912;
    fov = DEGREES_TO_VFPU(78.5);
    fov_cache = -999;
    roll = 0;
}

ScePspFVector4 D_eboot_089310B0 = {0, 0, -50, 0};
ScePspFVector4 D_eboot_089310C0 = {0, 1, 0, 0};

void Camera::View_initialize() {
    ScePspFVector4 position, target, up;
    position = D_eboot_089310B0;
    clear(&target, sizeof(target));
    up = D_eboot_089310C0;

    Ge::objectPtr->norm = 1;
    near_z = 30;
    far_z = 65000;
    fov = DEGREES_TO_VFPU(78.5);
    aspect_ratio = 480.0f / 272;
    SetViewport();
    flmatMakeLookAt(&position, &target, &up);
    unknown_0xDC0 = 0;
    enableCameraControls = 0;
    View_init();
    height_id = 3;
    next_demo_id = 0;
    demo_enemy = NULL;
}

void Camera::CameraInit() {
    state.word = 0;
    unknown_0xB3C = 1;
    View_init();
    state.word = 0;
    player = PlayerManager::objectPtr->getPlayerPtr(GameSys::objectPtr->player_id);
    for (int i = 2; i >= 0; --i) {
        quakes[i].is_active = false;
    }
    wyvern_find_player_flag = 0;
    unknown_0xA8A = 0;
    is_std_cam = false;
    demo_enemy = 0;
    next_demo_id = 0;
    StageCamInit();
    CameraMove();
}

void Camera::View_init() {
    near_z = 30;
    far_z = 65000;
    fov = DEGREES_TO_VFPU(78.5);
    aspect_ratio = (float)(30.0 / 17.0);
    roll = 0;
    set_aov();
}

float D_eboot_089310D0 = 0.075;

void Camera::CameraMove() {
    is_std_cam = false;
    is_areas_disabled = false;
    area_col = 0;
    area_row = 0;
    is_changing_area = false;

    if (state.byte == 0) {
        ++state.byte;

        area_cnf = NULL;
        area_id = 0xFF;
        next_area_id = 0xFF;
        SetAreaData();
        next_area_id = area_cnf->id;

        area_sub_camera_id = SubCameraId::STD;
        if (area_cnf->move_type == CameraAreaMoveType::STD) {
            area_sub_camera_id = SubCameraId::STD;
        } else {
            area_sub_camera_id = SubCameraId::STG;
        }
        last_sub_camera_id = area_sub_camera_id;

        for (int i = 0; i < SubCameraId::MAX_ID; ++i) {
            subCameras[i].cam_init(i);
        }

        falling_edge = 0;
        rising_edge = 0;
        buttons = 0;
        analog_magnitude = 0;
        analog_angle = 0;
    } else {
        cameraUpdateInput();
    }

    if (next_area_id == 0) {
        if (SetAreaData() > 0) {
            area_id = next_area_id;
            next_area_id = area_cnf->id;
            is_changing_area = true;
        }
    } else {
        if (CameraAreaCheck(area_cnf, CameraAreaZoneFlag::EXIT) != 0) {
            area_id = next_area_id;
            is_changing_area = true;
            if (SetAreaData() >= 0) {
                next_area_id = area_cnf->id;
            }
        }
    }

    last_sub_camera_id = area_sub_camera_id;
    switch (area_cnf->move_type) {
    case CameraAreaMoveType::STD:
        area_sub_camera_id = SubCameraId::STD;
        break;
    case CameraAreaMoveType::FIXED_PAN:
    case CameraAreaMoveType::RAIL:
    case CameraAreaMoveType::OFFSET_PAN:
        area_sub_camera_id = SubCameraId::STG;
        break;
    }

    for (int i = 0; i < SubCameraId::MAX_ID; ++i) {
        subCameras[i].cam_sub();
    }

    cam2view();
    wyvern_find_player_flag = false;
    View_move();
}

u8 D_eboot_089310D4[3] = {PlJointId::FLOOR, PlJointId::HIPS, PlJointId::HEAD};
s16 D_eboot_089310D8[8] = {10, 20, 30, 40, 50, 60, 70, 80};
float D_eboot_089310E8[3] = {2000, 3000, 4000};

void Camera::quake_sub(CameraQuake *quake) {
    if (quake->is_active == false) {
        return;
    }

    float t = 90.0f * ((float)quake->timer / (float)D_eboot_089310D8[quake->magnitude_id & 0x7F]);

    float amplitude;
    if ((quake->magnitude_id & 0x7F) < 2) {
        amplitude = 10.0f;
    } else {
        amplitude = 12.0f;
    }

    amplitude *= vsin_s((float)(2 * PI) * (t / 360.0f));

    float falloff;
    if (quake->magnitude_id & 0x80) {
        falloff = 1.0f;
    } else {
        float d = flvecCalcDistance(&quake->epicenter, &position);
        if (d >= D_eboot_089310E8[quake->radius_id]) {
            falloff = 0.0f;
        } else {
            falloff = 1.0f - d / D_eboot_089310E8[quake->radius_id];
        }
    }
    amplitude *= falloff;

    if (ext(quake->timer, 1, 1)) {
        amplitude = -amplitude;
    }

    position.y += amplitude;
    target.y += amplitude;
}

void Camera::set_quake_sub(int magnitude_id, ScePspFVector4 *epicenter) {
    quakes[0].is_active = true;
    quakes[0].magnitude_id = magnitude_id;
    quakes[0].timer = D_eboot_089310D8[magnitude_id];
    quakes[0].epicenter.x = epicenter->x;
    quakes[0].epicenter.y = epicenter->y;
    quakes[0].epicenter.z = epicenter->z;
    quakes[0].epicenter.w = epicenter->w;
    quakes[0].radius_id = 0;
}

void Camera::set_quake_sub2(int magnitude_id) {
    quakes[0].is_active = true;
    quakes[0].magnitude_id = magnitude_id | 0x80 /* GLOBAL */;
    quakes[0].timer = D_eboot_089310D8[magnitude_id];
}

void Camera::Em_set_quake_sub(int magnitude_id, ObjBase *em) {
    if (em->Pl_stg_ck() != false) {
        quakes[0].is_active = true;
        quakes[0].magnitude_id = magnitude_id;
        quakes[0].timer = D_eboot_089310D8[magnitude_id];
        quakes[0].epicenter.x = em->position.x;
        quakes[0].epicenter.y = em->position.y;
        quakes[0].epicenter.z = em->position.z;
        quakes[0].epicenter.w = em->position.w;
        quakes[0].radius_id = 0;
    }
}

void Camera::Em_set_quake_sub_W(int magnitude_id, ObjBase *em, int radius_id) {
    if (em->Pl_stg_ck() != false) {
        quakes[0].is_active = true;
        quakes[0].magnitude_id = magnitude_id;
        quakes[0].timer = D_eboot_089310D8[magnitude_id];
        quakes[0].epicenter.x = em->position.x;
        quakes[0].epicenter.y = em->position.y;
        quakes[0].epicenter.z = em->position.z;
        quakes[0].epicenter.w = em->position.w;
        quakes[0].radius_id = radius_id;
    }
}

void Camera::Pachinger_set_quake_sub(int magnitude_id, ObjBase *pl) {
    if (pl->Pl_stg_ck() != false) {
        quakes[1].is_active = true;
        quakes[1].magnitude_id = magnitude_id;
        quakes[1].timer = D_eboot_089310D8[magnitude_id];
        quakes[1].epicenter.x = pl->position.x;
        quakes[1].epicenter.y = pl->position.y;
        quakes[1].epicenter.z = pl->position.z;
        quakes[1].epicenter.w = pl->position.w;
        quakes[1].radius_id = 0;
    }
}

bool Camera::Camera_kyu_senkai_chk() {
    if (sub_camera_id >= 0) {
        return false;
    }
    if (area_sub_camera_id != SubCameraId::STD) {
        return false;
    }
    return subCameras[SubCameraId::STD].data.std.is_fast_rotate != false;
}

void Camera::WyvernFindPlayer(Player *plPtr) {
    if (plPtr->Pl_master_ck() == true) {
        wyvern_find_player_flag = true;
    }
}

void Camera::BBQcamera_set(Player *plPtr) {
    if (plPtr->Pl_master_ck() == true) {
        subCameras[SubCameraId::STD].data.std.is_view_blocked = false;
        subCameras[SubCameraId::STD].data.std.goal_rotation = plPtr->rotation.y + 0x2000;
        height_id = 2;
    }
}

void Camera::NPCZoomInCameraRequest(Npc *npc, bool skipAnimation) {
    ZoomCameraData *d = &subCameras[SubCameraId::PLAYER_EX].data.playerEX.zoom_cam;
    if (skipAnimation == true) {
        d->state.byte = 0;
        d->operation = 2;
        d->timer = 15;
    } else {
        if (d->operation == 2) {
            d->state.byte = 0;
        }
        d->operation = 1;
    }
    if (npc != NULL) {
        d->state.byte = 0;
        d->stack_size = 0;
        d->npcs[d->stack_size] = npc;
        d->fov = DEGREES_TO_RADIANS(30);
        d->timer_total = 15;
        d->target_types[d->stack_size] = 0;
        copy_q(&d->position, &npc->position);
    } else {
        d->npcs[d->stack_size] = npc;
        d->fov = DEGREES_TO_RADIANS(30);
        d->timer_total = 15;
        switch (d->target_types[d->stack_size]) {
        case NpcType::POOGIE:
            d->y_offsets[d->stack_size] = 160.0f;
            d->z_offsets[d->stack_size] = 120.0f;
            break;
        default:
            d->y_offsets[d->stack_size] = 120.0f;
            d->z_offsets[d->stack_size] = 300.0f;
            break;
        }
    }

}

void Camera::BookEditCameraRequest(float y, float z) {
    ZoomCameraData *d = &subCameras[SubCameraId::PLAYER_EX].data.playerEX.zoom_cam;
    d->state.byte = 0;
    d->operation = 1;
    d->npcs[d->stack_size] = NULL;
    d->fov = DEGREES_TO_RADIANS(30);
    d->target_types[d->stack_size] = 3;
    d->y_offsets[d->stack_size] = y;
    d->z_offsets[d->stack_size] = z;
    d->timer_total = 15;
}

void Camera::NPCZoomInCameraCancel(bool smoothing) {
    SubCamera *c = &subCameras[SubCameraId::PLAYER_EX];
    ZoomCameraData *d = &c->data.playerEX.zoom_cam;
    d->operation = 3;
    d->timer_total = 15;
    if (smoothing) {
        --d->stack_size;
        if (d->stack_size != 0) {
            d->state.byte = 1;
            d->operation = 2;
            d->timer = 15;
            d->timer_total = 15;
        } else {
            d->state.byte = 0;
            d->operation = 0;
            d->timer = 0;
        }
    } else {
        int i = d->stack_size - 1;
        if (d->target_types[i] == 5 && d->timer != 0) {
            float dpos = flvecCalcDistance(&c->current_position, &d->positions[i]);
            float dtar = flvecCalcDistance(&c->current_target, &d->targets[i]);
            s16 steps = (D_eboot_089310D0 * (dpos + dtar));
            d->timer_total = d->timer = steps + 1;
        }
    }
}

bool Camera::NPCZoomInCameraCheck() {
    bool result = true;
    switch (subCameras[SubCameraId::PLAYER_EX].data.playerEX.zoom_cam.operation) {
    case 2:
    case 1:
        break;
    default:
        result = false;
        break;
    }
    return result;
}

bool Camera::NPCZoomInCameraCancelCheck() {
    bool result = true;
    switch (subCameras[SubCameraId::PLAYER_EX].data.playerEX.zoom_cam.operation) {
    case 3:
    case 1:
        break;
    default:
        result = false;
        break;
    }
    return result;
}

void Camera::LobbyFixCameraRequest(ScePspFVector4 *position,ScePspFVector4 *target,float *fov, bool skipAnimation) {
    ZoomCameraData *d = &subCameras[SubCameraId::PLAYER_EX].data.playerEX.zoom_cam;
    copy_q(&d->position, position);
    copy_q(&d->target, target);
    d->fov = *fov;
    if (skipAnimation == true) {
        d->state.byte = 0;
        d->operation = 2;
        d->timer = 15;
    } else {
        d->operation = 1;
    }
    d->npcs[d->stack_size] = NULL;
    d->target_types[d->stack_size] = 2;
}

void Camera::NPCZoomInCameraCancel2(bool x) {
    NPCZoomInCameraCancel(x);
}

void Camera::DemoCameraRequest(int demo_id, Enemy *em, bool is_yama_tsukami_quest_clear_0) {
    zClipping = false;
    next_demo_id = demo_id;
    demo_enemy = em;
    this->is_yama_tsukami_quest_clear_0 = is_yama_tsukami_quest_clear_0;
}

void Camera::DemoCameraRequest(int demo_id, Enemy * em) {
    zClipping = false;
    if (next_demo_id == 0) {
        next_demo_id = demo_id;
        demo_enemy = em;
    }
}

extern u8 quest_clear_camera_tbl[90];

// FIXME: enum
void Camera::QuestClearCameraRequest(bool interrupt) {
    Enemy *em = Quest::objectPtr->camera_enemy;
    if (em == NULL) {
        if (Quest::objectPtr->boss == NULL) {
            return;
        } else {
            em = Quest::objectPtr->boss;
        }
        if (em->pl_type != EmType::LAOSHAN_LUNG
            && em->pl_type != EmType::ASHEN_LAOSHAN_LUNG
            && em->pl_type != EmType::SHEN_GAOREN) {
            return;
        }
    }

    int em_type = em->pl_type;
    if (em_type != EmType::FATALIS
        && em_type != EmType::CRIMSON_FATALIS
        && em_type != EmType::WHITE_FATALIS
        && (Quest::objectPtr->mission_flags & (MissionFlag::SLAYING | MissionFlag::HUNTING)) == 0) {
        return;
    }

    if (em->stageId != GameSys::objectPtr->stage_id) {
        return;
    }

    int demo_id;
    switch (em_type) {
    case EmType::LAOSHAN_LUNG:
    case EmType::ASHEN_LAOSHAN_LUNG:
        if (StageId::FORTRESS_AREA_5 == Gamestage::stage_night_to_day(GameSys::objectPtr->stage_id)) {
            if (em->current_health <= 0) {
                demo_id = DemoId::LAOSHAN_LUNG_QUEST_CLEAR;
                break;
            }
        }
        return;
    case EmType::FATALIS:
    case EmType::CRIMSON_FATALIS:
    case EmType::WHITE_FATALIS:
        if (em->current_health > 0) {
            return;
        }
        demo_id = DemoId::FATALIS_QUEST_CLEAR;
        break;
    case EmType::SHEN_GAOREN:
        if (MapId::FORTRESS_D != Gamestage::Map_night_to_day(GameSys::objectPtr->map_id)
        || StageId::FORTRESS_AREA_5 == Gamestage::stage_night_to_day(GameSys::objectPtr->stage_id)) {
            if (em->current_health <= 0) {
                demo_id = DemoId::SHEN_GAOREN_QUEST_CLEAR;
                break;
            }
        }
        return;
    case EmType::KUSHALA_DAORA:
    case EmType::RUSTED_KUSHALA_DAORA:
    case EmType::CHAMELEOS:
        if (em->current_health <= 0) {
            demo_id = DemoId::BOSS_QUEST_CLEAR;
            break;
        }
        return;
    case EmType::LUNASTRA:
    case EmType::TEOSTRA:
        if (em->current_health <= 0) {
            demo_id = DemoId::BOSS_QUEST_CLEAR;
            break;
        }
        return;
    case EmType::AKANTOR:
        if (em->current_health <= 0) {
            demo_id = DemoId::AKANTOR_QUEST_CLEAR;
            break;
        }
        return;
    case EmType::UKANLOS:
        if (em->current_health <= 0) {
            if (em->action_type == 5 /* DIE */ && em->action_id == 3) {
                return;
            }
            demo_id = DemoId::UKANLOS_QUEST_CLEAR;
            break;
        }
        return;
    case EmType::YAMA_TSUKAMI_1:
        if (em->act_ck(5, 0) == true) {
            demo_id = DemoId::YAMA_TSUKAMI_QUEST_CLEAR_0;
        } else if (em->act_ck(5, 1) == true) {
            demo_id = DemoId::YAMA_TSUKAMI_QUEST_CLEAR_1;
        }
        break;
    case EmType::SHOGUN_CEANATAUR:
    case EmType::TERRA_SHOGUN_CEANATAUR:
        if (em->act_ck(5, 2) == true) {
            return;
        }
        demo_id = quest_clear_camera_tbl[GameSys::objectPtr->em_moto_type_ck(em->pl_type)];

        if (demo_id == DemoId::INVALID_ID) {
            return;
        }
        break;
    case EmType::PLESIOTH:
    case EmType::GREEN_PLESIOTH:
    case EmType::LAVASIOTH:
        if (em->action_type == 5 /* DIE */ && em->action_id == 2) {
            return;
        }
        // fallthrough
    default:
        demo_id = quest_clear_camera_tbl[GameSys::objectPtr->em_moto_type_ck(em->pl_type)];

        if (demo_id == DemoId::INVALID_ID) {
            return;
        }

        break;
    }


    if (check_clear_camera_disable() == true) {
        DemoCameraCancel();
    }

    if (interrupt) {
        if (!DemoCameraCheck()) {
            return;
        }
        if (GetDemoCameraID() == demo_id) {
            return;
        }
        DemoCameraCancel();
    }

    DemoCameraRequest(demo_id, em, false);
    subCameras[SubCameraId::DEMO].data.demo.is_quest_clear = true;
    Cockpit::Pit_QCC_flag_set();
}

void Camera::FB_DieCameraRequest() {
    QuestClearCameraRequest(true);
}

void Camera::EnemyEscapeCamera(Enemy *em) {
    if (check_clear_camera_disable() == true) {
        DemoCameraCancel();
    }
    int demo_id;
    switch (em->pl_type) {
    case EmType::LAOSHAN_LUNG:
    case EmType::ASHEN_LAOSHAN_LUNG:
        demo_id = DemoId::LAOSHAN_LUNG_ESCAPE;
        break;
    case EmType::FATALIS:
    case EmType::CRIMSON_FATALIS:
    case EmType::WHITE_FATALIS:
        if (em->posture != 2 /* FLY */) {
            demo_id = DemoId::FATALIS_ESCAPE;
        } else {
            demo_id = DemoId::FATALIS_FLYING_ESCAPE;
        }
        break;
    case EmType::SHEN_GAOREN:
        demo_id = DemoId::SHEN_GAOREN_ESCAPE;
        break;
    case EmType::KUSHALA_DAORA:
    case EmType::RUSTED_KUSHALA_DAORA:
        demo_id = DemoId::KUSHALA_DAORA_ESCAPE;
        break;
    case EmType::LUNASTRA:
    case EmType::TEOSTRA:
        demo_id = DemoId::LUNASTRA_ESCAPE;
        break;
    case EmType::CHAMELEOS:
        demo_id = DemoId::CHAMELEOS_ESCAPE;
        break;
    case EmType::AKANTOR:
    case EmType::UKANLOS:
        demo_id = DemoId::AKANTOR_ESCAPE;
        break;
    default:
        return;
    }
    DemoCameraRequest(demo_id, em, false);
}

bool Camera::check_clear_camera_disable() {
    switch (subCameras[SubCameraId::DEMO].data.demo.demo_id) {
    case DemoId::FORTRESS_PILEBUNKER:
    case DemoId::CASTLE_SCHRADE_PILEBUNKER:
    case DemoId::TOWN_PILEBUNKER:
        return true;
    default:
        return false;
    }
}

void Camera::PlayerDieCameraRequest() {
    if (area_sub_camera_id != SubCameraId::STG) {
        DemoCameraRequest(DemoId::PLAYER_DIE);
    }
}

void Camera::PlComebackCameraRequest() {
    switch (GameSys::objectPtr->map_id) {
    case MapId::TOWN_D:
        break;
    default:
        DemoCameraRequest(DemoId::PLAYER_COMEBACK);
        break;
    }
}

void Camera::PilebunkerCameraRequest() {
    int demo_id;
    switch (GameSys::objectPtr->stage_id) {
    case StageId::FORTRESS_AREA_5:
        demo_id = DemoId::FORTRESS_PILEBUNKER;
        break;
    case StageId::CASTLE_SCHRADE_AREA_2:
        demo_id = DemoId::CASTLE_SCHRADE_PILEBUNKER;
        break;
    case StageId::TOWN_D_AREA_3:
        demo_id = DemoId::TOWN_PILEBUNKER;
        break;
    default:
        return;
    }
    DemoCameraRequest(demo_id, NULL);
}

void Camera::FishWyvernCameraRequest(Enemy *em) {
    DemoCameraRequest(DemoId::FISH_WYVERN, em, false);
}

void Camera::DemoCameraCancel() {
    SubCamera *sc = &subCameras[SubCameraId::DEMO];
    DemoCameraData *d = &sc->data.demo;
    sc->cam_sub_mode.byte = 0;
    sc->timer = 0;
    d->demo_id = DemoId::NONE;
    d->demo_state = 0;
    d->enemy = NULL;
    d->enable_stage_collision = false;
    zClipping = true;
    next_demo_id = DemoId::NONE;
    d->is_quest_clear = false;
}

int Camera::DemoCameraCheck() {
    int state = subCameras[SubCameraId::DEMO].data.demo.demo_state;
    DemoCameraData *d = &subCameras[SubCameraId::DEMO].data.demo;
    if (state != 0) {
        return (s8)state;
    } else if (next_demo_id != DemoId::NONE || d->demo_id != DemoId::NONE) {
        return 2;
    } else {
        return 0;
    }
}

u8 Camera::GetDemoCameraID() {
    return subCameras[SubCameraId::DEMO].data.demo.demo_id;
}

u8 Camera::DemoCameraCheckQC() {
    return subCameras[SubCameraId::DEMO].data.demo.is_quest_clear;
}

bool Camera::method_08814BE0() {
    if (subCameras[SubCameraId::DEMO].isActive) {
        switch (subCameras[SubCameraId::DEMO].data.demo.demo_id) {
        case DemoId::POKKE_VILLAGE_INTRO:
        case DemoId::POKKE_ROOM_INTRO:
        case DemoId::KITCHEN_DEMO_0x52:
        case DemoId::KITCHEN_DEMO_0x53:
        case DemoId::KITCHEN_DEMO_0x54:
        case DemoId::KITCHEN_DEMO_0x55:
        case DemoId::KITCHEN_DISMISS_CAT:
        case DemoId::FARM_BOMB:
        case DemoId::FARM_FISH:
        case DemoId::KITCHEN_DEMO_0x59:
        case DemoId::KITCHEN_NEUTRAL_OR_GOOD_MEAL:
        case DemoId::KITCHEN_BAD_MEAL:
        case DemoId::KITCHEN_DEMO_0x5C:
        case DemoId::TRENYA_DEMO_0x5D:
        case DemoId::FARM_DEMO_0x5E:
        case DemoId::KITCHEN_CHANGE_CAT_TYPE:
        case DemoId::FARM_DEMO_0x60:
        case DemoId::FARM_DEMO_0x61:
        case DemoId::FARM_DEMO_0x62:
        case DemoId::FARM_DEMO_0x63:
        case DemoId::FARM_DEMO_0x64:
        case DemoId::FARM_DEMO_0x65:
        case DemoId::FARM_DEMO_0x66:
        case DemoId::FARM_DEMO_0x67:
        case DemoId::FARM_DEMO_0x68:
        case DemoId::FARM_DEMO_0x69:
            return true;
        default:
            break;
        }
    }
    return false;
}

int Camera::GetDemoCameraTimer() {
    if (GameSys::objectPtr->is_demo == false) {
        return -1;
    } else {
        return subCameras[SubCameraId::DEMO].timer;
    }
}

bool Camera::method_08814C50() {
    if (GameSys::objectPtr->is_demo == true) {
        return true;
    }
    DemoCameraData &data = subCameras[SubCameraId::DEMO].data.demo;
    if (data.is_quest_clear) {
        return true;
    }
    switch (data.demo_id) {
    case DemoId::FORTRESS_PILEBUNKER:
    case DemoId::CASTLE_SCHRADE_PILEBUNKER:
    case DemoId::TOWN_PILEBUNKER:
    case DemoId::FISH_WYVERN:
    case DemoId::LAOSHAN_LUNG_ESCAPE:
    case DemoId::FATALIS_ESCAPE:
    case DemoId::FATALIS_FLYING_ESCAPE:
    case DemoId::SHEN_GAOREN_ESCAPE:
    case DemoId::KUSHALA_DAORA_ESCAPE:
    case DemoId::CHAMELEOS_ESCAPE:
    case DemoId::LUNASTRA_ESCAPE:
    case DemoId::AKANTOR_ESCAPE:
        return true;
    case DemoId::BOSS_QUEST_CLEAR:
    case DemoId::SMALL_MONSTER_QUEST_CLEAR:
    case DemoId::LARGE_MONSTER_QUEST_CLEAR:
    case DemoId::MEDIUM_MONSTER_QUEST_CLEAR:
    default:
        return false;
    }
}

bool Camera::method_08814CC4() {
    u8 demo_id = subCameras[SubCameraId::DEMO].data.demo.demo_id;
    if (   demo_id != DemoId::NARGACUGA_INTRO
        && demo_id != DemoId::HYPNOCATRICE_INTRO
        && demo_id != DemoId::LAVASIOTH_INTRO
        && demo_id != DemoId::UKANLOS_INTRO
        && demo_id != DemoId::YAMA_TSUKAMI_INTRO
        && demo_id != DemoId::AKANTOR_INTRO
        && demo_id != DemoId::WHITE_FATALIS_INTRO
        && demo_id != DemoId::CRIMSON_FATALIS_INTRO
        // && demo_id != DemoId::FATALIS_INTRO
        // && demo_id != DemoId::ABSOLUTE_POWER_DEMO
        // && demo_id != DemoId::SINKING_FEELING_DEMO
        && demo_id != DemoId::SHEN_GAOREN_INTRO
        && demo_id != DemoId::ASHEN_LAOSHAN_LUNG_INTRO
        // && demo_id != DemoId::LAOSHAN_LUNG_INTRO
        && demo_id != DemoId::THE_LONE_BLACK_GARUGA_DEMO
        // && demo_id != DemoId::THE_FINAL_INVITATION_DEMO
        // && demo_id != DemoId::THE_ELDER_DRAGON_OF_MIST_DEMO
        && demo_id != DemoId::BATTLE_OF_THE_BLOS_DEMO
        && demo_id != DemoId::THE_LEGENDARY_KIRIN_DEMO
        && demo_id != DemoId::OVERSEER_OF_THE_ANCIENTS_DEMO
        // && demo_id != DemoId::TERROR_OF_THE_GRAVIOS_DEMO
        && demo_id != DemoId::EMPEROR_OF_FLAME_DEMO
        // && demo_id != DemoId::THE_EMPRESS_S_BLAZING_THRONE_DEMO
        && demo_id != DemoId::THE_FROZEN_DICTATOR_DEMO
        && demo_id != DemoId::THE_RUNAWAY_DIABLOS_DEMO
        // && demo_id != DemoId::COMMANDER_IN_THE_FLAMES_DEMO
        // && demo_id != DemoId::WATER_WYVERN_IN_THE_DESERT_DEMO
        && demo_id != DemoId::THE_RULER_OF_THE_SNOW_DEMO
        && demo_id != DemoId::SHADOW_IN_THE_SNOW_DEMO
        // && demo_id != DemoId::THE_ELDER_DRAGON_OF_WIND_DEMO
        && demo_id != DemoId::RAREST_OF_THE_RARE_BEASTS_DEMO
        // && demo_id != DemoId::THE_LURKING_DESERT_GIANT_DEMO
        && demo_id != DemoId::GYPCEROS_VENOMOUS_TERROR_DEMO
        // && demo_id != DemoId::THE_LAND_SHARK_DEMO
        && demo_id != DemoId::JUNGLE_MENACE_DEMO
        && demo_id != DemoId::THE_CARNIVOROUS_LEADER_DEMO
        && demo_id != DemoId::HUNT_DOWN_THE_VELOCIDROME_DEMO
        && demo_id != DemoId::RECKLESS_BULLDROME_HUNTER_DEMO) {
        return false;
    }
    return true;
}

bool Camera::PachingerCamChk(Player *plPtr) {
    if (subCameras[SubCameraId::DEMO].isActive != false) {
        return false;
    }
    if ((bool)plPtr->scope_aim_increment == true) {
        return true;
    }
    return subCameras[SubCameraId::PCHNGR].isActive != false;
}

s8 Camera::GetPachingerInfo(u8 *crosshairsOut, u8 *isVariableOut, float *zoomCursorOut) {
    SubCamera *sc = &subCameras[SubCameraId::PCHNGR];
    PchngrCameraData *d = &sc->data.pchngr;
    *crosshairsOut = sc->data.pchngr.crosshairs;
    *isVariableOut = sc->data.pchngr.is_variable;
    if (sc->data.pchngr.is_variable != false) {
        *zoomCursorOut = sc->current_fov - d->min_fov;
        *zoomCursorOut *= d->inv_fov_range;
    }
    return d->pachi_type;
}

void Camera::GetCameraEye(ScePspFVector4 *out) {
    out->x = position.x;
    out->y = position.y;
    out->z = position.z;
    out->w = 0.0f;
}

void Camera::GetCameraAt(ScePspFVector4 *out) {
    out->x = target.x;
    out->y = target.y;
    out->z = target.z;
    out->w = 0.0f;
}

void Camera::GetCameraUp(ScePspFVector4 *out) {
    copy_q(out, &up);
}

bool Camera::player_trnsl_chk(u8 *alphaOut, ObjBase *plPtr) {
    Joint *joints = plPtr->hierarchy.roots[0];
    u8 closest_index = 0xFF;
    float min_distance = 65000.0f;
    for (int i = 0; i < (int)(sizeof(D_eboot_089310D4) / sizeof(D_eboot_089310D4[0])); ++i) {
        Joint *joint = joints + D_eboot_089310D4[i];
        float distance = distanceSquared(&joint->globalPose.w, &position);
        if (distance < min_distance) {
            closest_index = i;
            min_distance = distance;
        }
    }
    if (closest_index != 0xFF) {
        if (min_distance < 10000.0f) {
            if (min_distance <= 3600.0f) {
                *alphaOut = 0x4C;
            } else {
                float t = (min_distance - 3600.0f) / 6400.0f * (float)(0xFF - 0x4C);
                *alphaOut = 0x4C + (u8)t;
            }
        } else {
            *alphaOut = 0xFF;
            return false;
        }
    } else {
        *alphaOut = 0xFF;
    }
    return true;
}

void Camera::flmatMakeLookAt(ScePspFVector4 *position, ScePspFVector4 *target, ScePspFVector4 *up) {
    this->position = *position;
    this->target = *target;
    this->up = *up;
    ScePspFVector4 direction;
    vsub_q(&direction, target, position);
    distance = flvecInnerProduct(&direction, &direction);
    distance = vsqrt_s(distance);
    ::flmatMakeLookAt(&worldToView, position, target, up);
    ScePspFVector4 nposition, ntarget;
    float norm = Ge::objectPtr->norm;
    vscl_q(&nposition, position, norm);
    vscl_q(&ntarget, target, norm);
    ::flmatMakeLookAt(&nworldToView, &nposition, &ntarget, up);
    flmatInvert(&viewToWorld, &worldToView);
    flmatInvert(&viewToNworld, &nworldToView);
}

void Camera::flmatrMakeProjection(ScePspFMatrix4 *out, float fov, float aspect, float near, float far) {
    memset(out, 0, sizeof(*out));
    float nnear = near * Ge::objectPtr->norm;
    float far_sq = far * far;
    fov *= 0.5f;
    float cos_fov = vcos_s(fov);
    float sin_fov = vsin_s(fov);
    out->x.x = (cos_fov / sin_fov) / aspect;
    out->y.y = cos_fov / sin_fov;
    out->z.z = -(far_sq + nnear) / (far_sq - nnear);
    out->z.w = -1.0f;
    out->w.z = -(far_sq * 2.0f * nnear) / (far_sq - nnear);

}

void Camera::set_viewproj(float fov, float aspect, float near, float far) {
    flmatrMakeProjection(&viewToProjection, fov, aspect, near, far);
    flmatInvert(&projectionToView, &viewToProjection);
    plSetupFOVClipPlanes();
}

void Camera::project(ScePspFVector4 *out, ScePspFVector4 *in) {
    vscl_q(out, in, Ge::objectPtr->norm);
    out->w = 1.0f;

    ScePspFVector4 tmp;
    vtfm4_q(&tmp, out, &nworldToView);
    vtfm4_q(out, &tmp, &viewToProjection);

    bool near_zero = false;
    if (-1.0e-5f < out->w) {
        if (out->w < 1.0e-5f) {
            near_zero = true;
        }
    }
    if (near_zero == true) {
        out->w = 1.0f;
    }

    out->x = viewport_center.fv.x + viewport_scale.fv.x * out->x / out->w;
    out->y = viewport_center.fv.y + viewport_scale.fv.y * out->y / out->w;
    out->z = viewport_center.fv.z + viewport_scale.fv.z * out->z / out->w;
    out->x -= viewport_offset.x;
    out->y -= viewport_offset.y;
}

u16 Camera::Get_view_dir() {
    float dx = target.x - position.x;
    float dz = target.z - position.z;
    return (int)(flArcTan2(-dz, dx) * 65536.0f / (float)(2 * PI) + 0.5f);
}

void Camera::StageCamInit() {
    if (area_data == NULL) {
        default_area_data();
    }
    area_row_count = area_data->rows;
    area_col_count = area_data->columns;
    unknown_0xB20 = area_data->unknown_0x8;
    unknown_0xB22 = area_data->unknown_0xA;
    unknown_0xB24 = area_data->unknown_0xC;
    unknown_0xB28 = area_data->unknown_0x10;
    unknown_0xB2C = area_data->unknown_0x14;
    unknown_0xB30 = area_data->unknown_0x18;
}

void Camera::SetCameraData(CameraAreaHeader *data) {
    CameraAreaList *cell;
    CameraAreaCnf **cnfs;
    int n;
    int i;

    if (data == NULL) {
        area_data = NULL;
        return;
    }
    area_data = data;
    n = (int)(data + 1);
    data->first = reinterpret_cast<CameraAreaCnf *>(n);
    for (i = data->count; i > 0; --i) {
        CameraAreaCnf *cnf = reinterpret_cast<CameraAreaCnf *>(n);
        if (cnf->hokan_info != NULL) {
            cnf->zones = reinterpret_cast<CameraAreaZone *>((int)cnf + (int)cnf->zones);
            cnf->hokan_info = reinterpret_cast<CameraHokanInfo *>((int)cnf + (int)cnf->hokan_info);
            n = ((int)(cnf + 1) + cnf->zone_count * sizeof(CameraAreaZone));
            n += 0x10 * sizeof(CameraHokanInfo);
        } else {
            cnf->zones = reinterpret_cast<CameraAreaZone *>((int)cnf + (int)cnf->zones);
            n = ((int)(cnf + 1) + cnf->zone_count * sizeof(CameraAreaZone));
        }
    }
    n = 0;
    if (data->index != NULL) {
        cell = reinterpret_cast<CameraAreaList *>((int)data + (int)data->index);
        data->index = cell;
        for (i = data->rows * data->columns; i > 0; --i, ++cell) {
            if (cell->length != 0) {
                cell->entries = reinterpret_cast<CameraAreaCnf **>((int)data + (int)cell->entries);
                n += cell->length;
            }
        }
    } else {
        data->index = NULL;
    }
    if (data->cnfs != NULL) {
        cnfs = reinterpret_cast<CameraAreaCnf **>((int)data + (int)data->cnfs);
        data->cnfs = cnfs;
        for (i = n; i > 0; --i, ++cnfs) {
            *cnfs = reinterpret_cast<CameraAreaCnf *>((int)data + (int)*cnfs);
        }
    } else {
        data->cnfs = NULL;
    }
}

u8 pad_089310F4[12] = {};
ScePspFVector4 D_eboot_08931100 = {0, 0, 320, 0};
ScePspFVector4 D_eboot_08931110 = {0, 0, -1000, 0};

CameraFollowDefinition cam_cnf_chs = {
    .fov = DEGREES_TO_RADIANS(50),
    .roll = 0.0f,
    .entries = {
        {
            .y_offset = 300.0f,
            .z_offset = 160.0f,
            .height = 184.0f,
            .min_height = 80.0f,
        },
        {
            .y_offset = 270.0f,
            .z_offset = 490.0f,
            .height = 170.0f,
            .min_height = 80.0f,
        },
        {
            .y_offset = 140.0f,
            .z_offset = 490.0f,
            .height = 170.0f,
            .min_height = 80.0f,
        },
        {
            .y_offset = 60.0f,
            .z_offset = 450.0f,
            .height = 210.0f,
            .min_height = 60.0f,
        },
        {
            .y_offset = 400.0f,
            .z_offset = 220.0f,
            .height = 195.0f,
            .min_height = 80.0f,
        },
    },
};

inline void Defautl_chs_init(CameraAreaCnf *c) {
    c->magic[0] = 0;
    c->id = 0;
    c->move_type = CameraAreaMoveType::STD;
    c->axes = CameraAreaAxes::INVALID_AXES;
    c->target_type = CameraAreaTargetType::FIXED;
    c->zone_count = 0;
    c->near_distance = 400.0f;
    c->far_distance = 2400.0f;
    c->near_fov = 1.0f;
    c->far_fov = 0.75f;
    c->zones = NULL;
    c->hokan_info = NULL;
    for (int i = 0; i < 5; ++i) {
        c->height.entries[i].y_offset = cam_cnf_chs.entries[i].y_offset;
        c->height.entries[i].z_offset = cam_cnf_chs.entries[i].z_offset;
        c->height.entries[i].height = cam_cnf_chs.entries[i].height;
        c->height.entries[i].min_height = cam_cnf_chs.entries[i].min_height;
    }
    c->height.fov = DEGREES_TO_RADIANS(50);
    c->height.roll = 0;
    c->height.pad_0x8 = 0;
    c->height.pad_0xE = 0;
    c->height.pad_0xC = 0;
}

void Camera::default_area_data() {
    CameraAreaHeader *d = area_data;
    CameraAreaCnf *c = reinterpret_cast<CameraAreaCnf *>(d + 1);
    d->version = 0x102;
    d->count = 0;
    d->rows = 1;
    d->columns = 1;
    d->unknown_0x8 = 20000;
    d->unknown_0xA = 20000;
    d->unknown_0xC = 0;
    d->unknown_0x10 = 0;
    d->unknown_0x14 = 20000;
    d->unknown_0x18 = 20000;
    d->index = NULL;
    d->cnfs = NULL;
    d->unknown_0x24 = 0;
    d->first = c;
    Defautl_chs_init(c);
    area_data = d;
}

void Camera::cameraUpdateInput() {
    if (GameSys::objectPtr->Game_clear_ck(1) == true || player->dialog_timer != 0) {
        falling_edge = 0;
        rising_edge = 0;
        buttons = 0;
        analog_magnitude = 0;
        analog_angle = 0;
        return;
    }
    buttons = Pad::BUTTONS;
    rising_edge = Pad::RISING_EDGE;
    analog_angle = Pad::ANALOG_ANGLE;
    analog_magnitude = Pad::ANALOG_MAGNITUDE;
}

void Camera::cam2view() {
    for (int i = 0; i < (int)(sizeof(quakes) / sizeof(quakes[0])); ++i) {
        if (quakes[i].is_active && --quakes[i].timer < 1) {
            quakes[i].is_active = false;
        }
    }

    SubCamera *sc = &subCameras[area_sub_camera_id];
    flvecCopy(&last_position, &sc->current_position);
    flvecCopy(&last_target, &sc->current_target);
    last_roll = sc->current_roll;
    last_fov = sc->current_fov;

    sub_camera_id = SubCameraId::INVALID_ID;
    if (subCameras[SubCameraId::DEMO].isActive) {
        sub_camera_id = SubCameraId::DEMO;
    } else if (subCameras[SubCameraId::PCHNGR].isActive) {
        sub_camera_id = SubCameraId::PCHNGR;
    } else if (subCameras[SubCameraId::PLAYER_EX].isActive) {
        sub_camera_id = SubCameraId::PLAYER_EX;
    }

    if (sub_camera_id > 0) {
        SubCamera *sc = &subCameras[sub_camera_id];
        flvecCopy(&position, &sc->current_position);
        flvecCopy(&target, &sc->current_target);
        if (sub_camera_id == SubCameraId::DEMO) {
            quake_sub(&quakes[2]);
        } else if (sub_camera_id == SubCameraId::PCHNGR) {
            quake_sub(&quakes[1]);
        } else if (sub_camera_id == SubCameraId::PLAYER_EX) {
            quake_sub(&quakes[0]);
        }
        RollView(sc->current_roll);
        SetAngleOfView(sc->current_fov);
        is_std_cam = false;
    } else {
        flvecCopy(&position, &last_position);
        flvecCopy(&target, &last_target);
        quake_sub(&quakes[0]);
        RollView(last_roll);
        SetAngleOfView(last_fov);
    }
    set_to_std_cam(sub_camera_id);
}

void Camera::set_to_std_cam(int sub_camera_id) {
    SubCamera *sc = &subCameras[SubCameraId::STD];
    if (sub_camera_id == SubCameraId::DEMO) {
        sub_camera_id = -1;
    }
    int value;
    if (sub_camera_id >= 0) {
        value = sub_camera_id;
    } else {
        value = 0xFF;
    }
    sc->current_sub_camera_id = value;
}

s8 Camera::SetAreaData() {
    if (area_data == NULL) {
        area_cnf = NULL;
        return -1;
    }
    area_cnf = area_data->first;
    if (is_areas_disabled != 0) {
        return 0;
    }
    CameraAreaList *index = area_data->index;
    if (index == NULL) {
        return 0;
    }
    CameraAreaList *cell = &index[area_row + area_row_count * area_col];
    CameraAreaCnf **cnf;
    int n = cell->length;
    if (n == 0) {
        return 0;
    }
    cnf = cell->entries;
    while (n > 0) {
        if (CameraAreaCheck(*cnf, CameraAreaZoneFlag::ENTER) == 0) {
            area_cnf = *cnf;
            return 1;
        }
        --n;
        ++cnf;
    }
    return 0;
}

u8 Camera::CameraAreaCheck(CameraAreaCnf *cnf, u8 flags) {
    if ((cnf->attributes & 0x80) != 0 && player->Pl_ClimbChk(player) == false) {
        return 2;
    }
    CameraAreaZone *zone;
    int n;
    for (zone = cnf->zones, n = cnf->zone_count; n > 0; --n, ++zone) {
        if ((zone->flags & flags) == 0) {
            ScePspFVector3 delta;
            delta.x = player->position.x - zone->position.x;
            delta.y = player->position.y - zone->position.y;
            delta.z = player->position.z - zone->position.z;
            float prod = flvecInnerProduct3(&zone->normal, &delta);
            if (prod < 0.0f || prod > zone->height || Area_XZ_Check(zone, &player->position) != 0) {
                continue;
            }
            return 0;
        }
    }
    return 2;
}

u8 Camera::Area_XZ_Check(CameraAreaZone *zone, ScePspFVector4 *position) {
    float *v = zone->vertices;
    float b = position->x - v[0];
    float c = position->z - v[2];
    // TODO: find an inline function that doesn't break this
    float d = v[0] - v[4];
    float a = v[2] - v[6];
    a *= b;
    float area = c * d;
    area = a - area;
    if (area < 0.0f) {
        return 2;
    }
    d = v[5] - v[0];
    a = v[7] - v[2];
    a *= b;
    area = c * d;
    area = a - area;
    if (area < 0.0f) {
        return 2;
    }
    b = position->x - v[1];
    c = position->z - v[3];
    d = v[4] - v[1];
    a = v[6] - v[3];
    a *= b;
    area = c * d;
    area = a - area;
    if (area < 0.0f) {
        return 2;
    }
    d = v[1] - v[5];
    a = v[3] - v[7];
    a *= b;
    area = c * d;
    area = a - area;
    if (area < 0.0f) {
        return 2;
    }
    return 0;
}

void Camera::set_aov() {
    set_viewproj(fov, aspect_ratio, near_z, far_z);
    fov_cache = fov;
}

inline int calc_mat_angY(ScePspFMatrix4 *m) {
    ScePspFVector4 forward;
    forward.x = forward.y = 0.0f;
    forward.z = forward.w = 1.0f;
    ScePspFVector4 tar;
    nlCalcPoint(&tar, &forward, m);
    return flArcTan2(-(tar.z - m->w.z), tar.x - m->w.x) * 65536.0f / (float)(2 * PI) + 0.5f;
}

void Camera::View_move() {
    if (fov_cache != fov) {
        set_aov();
    }

    Roll2Upvec(&up, &last_position, &last_target, roll);
    flmatMakeLookAt(&position, &target, &up);

    u16 ang = calc_mat_angY(&viewToWorld);
    // TODO: cleanup
    float pan = 360.0f;
    ang += 0x4000;
    pan *= (int)ang;
    pan /= 65536.0f;
    pan = pan / 360.0f * (float)(2 * PI);
    flmatSetXYZ33(&rview_matY, 0, pan, 0);
}

void flmatMakeLookAt(ScePspFMatrix4 *out, ScePspFVector4 *position, ScePspFVector4 *target, ScePspFVector4 *up) {
    ScePspFVector4 e3;
    ScePspFVector4 e4;
    ScePspFVector4 e1;
    ScePspFVector4 e2;
    vsub_q(&e1, position, target);
    copy_q(&e2, up);
    normalize3_2(&e1);
    normalize3_2(&e2);
    flvecOuterProduct(&e3, &e2, &e1);
    normalize3_2(&e3);
    flvecOuterProduct(&e4, &e1, &e3);
    copy_q(&out->x, &e3);
    copy_q(&out->y, &e4);
    copy_q(&out->z, &e1);
    sv_q(&out->w, 0, 0, 0, 0);
    transpose_q(out);
    sv_q(&out->w,
        -flvecInnerProduct(position, &e3),
        -flvecInnerProduct(position, &e4),
        -flvecInnerProduct(position, &e1),
        1.0f);
}

void Camera::view_set(ScePspFVector4 *pos, ScePspFVector4 *tar, float f) {
    sv_q(&up, 0.0f, 1.0f, 0.0f, 0.0f);
    copy_q(&last_position, pos);
    copy_q(&last_target, tar);
    copy_q(&position, pos);
    copy_q(&target, tar);
    last_fov = f;
    fov = f;
    view_set_move();
}

void Camera::view_set_move() {
    target.x += (last_target.x - target.x) / 9.0f;
    target.y += (last_target.y - target.y) / 9.0f;
    target.z += (last_target.z - target.z) / 9.0f;
    position.x += (last_position.x - position.x) / 9.0f;
    position.y += (last_position.y - position.y) / 9.0f;
    position.z += (last_position.z - position.z) / 9.0f;
    fov += (last_fov - fov) / 9.0f;

    set_aov();

    flmatMakeLookAt(&position, &target, &up);

    u16 ang = calc_mat_angY(&viewToWorld);
    // TODO: cleanup
    float pan = 360.0f;
    ang += 0x4000;
    pan *= (int)ang;
    pan /= 65536.0f;
    pan = pan / 360.0f * (float)(2 * PI);
    flmatSetXYZ33(&rview_matY, 0, pan, 0);
}

void Camera::last_view_set(ScePspFVector4 *pos, ScePspFVector4 *tar, float f) {
    copy_q(&last_position, pos);
    copy_q(&last_target, tar);
    last_fov = f;
}

void Roll2Upvec(ScePspFVector4 *out, ScePspFVector4 *position, ScePspFVector4 *target, float roll) {
    ScePspFVector4 tmp;
    ScePspFVector4 q;
    ScePspFMatrix4 m;
    tmp.x = target->x - position->x;
    tmp.y = target->y - position->y;
    tmp.z = target->z - position->z;
    tmp.w = 1.0f;
    sceVfpuQuaternionFromRotate(&q, &tmp, roll);
    sceVfpuQuaternionToMatrix(&m, &q);
    tmp.y = 1.0f;
    tmp.z = tmp.x = 0.0f;
    flvecApplyMat33(out, &tmp, &m);
}

void Camera::plMakePlaneFromPoints(ScePspFVector4 *out, ScePspFVector4 *a, ScePspFVector4 *b, ScePspFVector4 *c) {
    ScePspFVector4 p, q, r;
    vsub_q(&p, b, a);
    vsub_q(&q, c, a);
    flvecOuterProduct(&r, &p, &q);
    normalize3(out, &r);
}

void Camera::plSetupFOVClipPlanes() {
    ScePspFVector4 a, b, c;
    vzero_q(&a);
    float far = far_z;
    c.z = -far * 1.5f;
    b.z = c.z;
    float cos_fov = vcos_s(0.5f * fov);
    float sin_fov = vsin_s(0.5f * fov);
    float r = cos_fov / sin_fov;
    b.x = -far_z * r;
    b.y = far_z * r / aspect_ratio;
    c.x = b.x;
    c.y = -b.y;
    plMakePlaneFromPoints(&left_clip_plane, &a, &b, &c);
    r = cos_fov / sin_fov;
    b.x = far_z * r;
    b.y = -far_z * r / aspect_ratio;
    c.x = b.x;
    c.y = -b.y;
    plMakePlaneFromPoints(&right_clip_plane, &a, &b, &c);
    b.x = far_z * r;
    b.y = far_z * r / aspect_ratio;
    c.x = -b.x;
    c.y = b.y;
    plMakePlaneFromPoints(&top_clip_plane, &a, &b, &c);
    b.x = -far_z * r;
    b.y = -far_z * r / aspect_ratio;
    c.x = -b.x;
    c.y = b.y;
    plMakePlaneFromPoints(&bottom_clip_plane, &a, &b, &c);
    clip_min_z = -far_z;
    clip_max_z = -near_z;
}

bool Camera::z_clip_check(ScePspFVector4 *p, float delta) {
    if (zClipping == false) {
        return true;
    }
    float z = worldToView.x.z * p->x
            + worldToView.y.z * p->y
            + worldToView.z.z * p->z
            + worldToView.w.z;
    if (z - delta > clip_max_z) {
        return false;
    }
    if (z + delta < clip_min_z) {
        return false;
    }
    return true;
}

bool Camera::clip_check(ScePspFVector4 *p, float tolerance) {
    float f = clip_min_z;
    return clip_check(p, tolerance, f);
}

// a few regswaps
bool Camera::clip_check(ScePspFVector4 *p, float delta, float tolerance) {
    if (zClipping == false) {
        return true;
    }

    float z = worldToView.x.z * p->x
        + worldToView.y.z * p->y
        + worldToView.z.z * p->z
        + worldToView.w.z;
    if (z - delta > clip_max_z) {
        return false;
    }
    if (z + delta < tolerance) {
        return false;
    }

    float x = worldToView.x.x * p->x
        + worldToView.y.x * p->y
        + worldToView.z.x * p->z
        + worldToView.w.x;
    if (x * left_clip_plane.x + z * left_clip_plane.z > delta) {
        return false;
    }
    if (x * right_clip_plane.x + z * right_clip_plane.z > delta) {
        return false;
    }

    float y = worldToView.x.y * p->x
        + worldToView.y.y * p->y
        + worldToView.z.y * p->z
        + worldToView.w.y;
    if (y * top_clip_plane.y + z * top_clip_plane.z > delta) {
        return false;
    }
    if (y * bottom_clip_plane.y + z * bottom_clip_plane.z > delta) {
        return false;
    }

    return true;
}

void Camera::SetViewport() {
    float far = 0.5f * far_z;
    viewport_scale.fv.x = 240.0f;
    viewport_scale.fv.y = -136.0f;
    viewport_scale.fv.z = far;
    viewport_center.fv.x = 2048.0f;
    viewport_center.fv.y = 2048.0f;
    viewport_center.fv.z = far;
}

UnalignedCameraFollowHeightEntry cam_cnf_chs_kabegiwa = {
    .y_offset = 280.0f,
    .z_offset = 500.0f,
    .height = 170.0f,
    .min_height = 80.0f,
};

UnalignedCameraFollowHeightEntry cam_cnf_chs_falldown = {
    .y_offset = 250.0f,
    .z_offset = 100.0f,
    .min_height = 80.0f,
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
u8 quest_clear_camera_tbl[90] = {
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
    u16 demo_id;
    u16 pac_offset;
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
