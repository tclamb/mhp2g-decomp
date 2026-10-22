#pragma once

#include "common.h"
#include "enemy.hpp"
#include "npc.hpp"
#include "psptypes.h"
#include "singleton.hpp"
#include "player.hpp"


struct DemoId {
    enum {
        NONE,
        PLAYER_COMEBACK,
        PLAYER_DIE,
        FORTRESS_PILEBUNKER,
        CASTLE_SCHRADE_PILEBUNKER,
        TOWN_PILEBUNKER,
        BOSS_QUEST_CLEAR,
        SMALL_MONSTER_QUEST_CLEAR,
        LARGE_MONSTER_QUEST_CLEAR,
        MEDIUM_MONSTER_QUEST_CLEAR,
        FISH_WYVERN,
        LAOSHAN_LUNG_ESCAPE,
        FATALIS_ESCAPE,
        FATALIS_FLYING_ESCAPE,
        SHEN_GAOREN_ESCAPE,
        KUSHALA_DAORA_ESCAPE,
        CHAMELEOS_ESCAPE,
        LUNASTRA_ESCAPE,
        AKANTOR_ESCAPE,
        LAOSHAN_LUNG_QUEST_CLEAR,
        FATALIS_QUEST_CLEAR,
        SHEN_GAOREN_QUEST_CLEAR,
        AKANTOR_QUEST_CLEAR,
        UKANLOS_QUEST_CLEAR,
        YAMA_TSUKAMI_QUEST_CLEAR_1,
        YAMA_TSUKAMI_QUEST_CLEAR_0,
        RECKLESS_BULLDROME_HUNTER_DEMO,
        HUNT_DOWN_THE_VELOCIDROME_DEMO,
        THE_CARNIVOROUS_LEADER_DEMO,
        JUNGLE_MENACE_DEMO,
        THE_LAND_SHARK_DEMO,
        GYPCEROS_VENOMOUS_TERROR_DEMO,
        THE_LURKING_DESERT_GIANT_DEMO,
        RAREST_OF_THE_RARE_BEASTS_DEMO,
        THE_ELDER_DRAGON_OF_WIND_DEMO,
        SHADOW_IN_THE_SNOW_DEMO,
        THE_RULER_OF_THE_SNOW_DEMO,
        WATER_WYVERN_IN_THE_DESERT_DEMO,
        COMMANDER_IN_THE_FLAMES_DEMO,
        THE_RUNAWAY_DIABLOS_DEMO,
        THE_FROZEN_DICTATOR_DEMO,
        THE_EMPRESS_S_BLAZING_THRONE_DEMO,
        EMPEROR_OF_FLAME_DEMO,
        TERROR_OF_THE_GRAVIOS_DEMO,
        OVERSEER_OF_THE_ANCIENTS_DEMO,
        THE_LEGENDARY_KIRIN_DEMO,
        BATTLE_OF_THE_BLOS_DEMO,
        THE_ELDER_DRAGON_OF_MIST_DEMO,
        THE_FINAL_INVITATION_DEMO,
        THE_LONE_BLACK_GARUGA_DEMO,
        LAOSHAN_LUNG_INTRO,
        ASHEN_LAOSHAN_LUNG_INTRO,
        SHEN_GAOREN_INTRO,
        SINKING_FEELING_DEMO,
        ABSOLUTE_POWER_DEMO,
        FATALIS_INTRO,
        CRIMSON_FATALIS_INTRO,
        WHITE_FATALIS_INTRO,
        AKANTOR_INTRO,
        YAMA_TSUKAMI_INTRO,
        UKANLOS_INTRO,
        LAVASIOTH_INTRO,
        HYPNOCATRICE_INTRO,
        NARGACUGA_INTRO,
        TRAINING_BASICS_DEMO,
        TRAINING_GATHERING_DEMO_1,
        TRAINING_GATHERING_DEMO_2,
        TRAINING_GATHERING_DEMO_3,
        TRAINING_GATHERING_DEMO_4,
        TRAINING_COMBINING_DEMO,
        TRAINING_FISHING_DEMO,
        TRAINING_TRANSPORT_DEMO,
        FOREST_AND_HILLS_INTRO,
        DESERT_INTRO,
        SWAMP_INTRO,
        VOLCANO_INTRO,
        JUNGLE_INTRO,
        SNOWY_MOUNTAINS_INTRO,
        TOWER_INTRO,
        GREAT_FOREST_INTRO,
        POKKE_VILLAGE_INTRO,
        POKKE_ROOM_INTRO,
        KITCHEN_DEMO_0x52,
        KITCHEN_DEMO_0x53,
        KITCHEN_DEMO_0x54,
        KITCHEN_DEMO_0x55,
        KITCHEN_DISMISS_CAT,
        FARM_BOMB,
        FARM_FISH,
        KITCHEN_DEMO_0x59,
        KITCHEN_NEUTRAL_OR_GOOD_MEAL,
        KITCHEN_BAD_MEAL,
        KITCHEN_DEMO_0x5C,
        TRENYA_DEMO_0x5D,
        FARM_DEMO_0x5E,
        KITCHEN_CHANGE_CAT_TYPE,
        FARM_DEMO_0x60,
        FARM_DEMO_0x61,
        FARM_DEMO_0x62,
        FARM_DEMO_0x63,
        FARM_DEMO_0x64,
        FARM_DEMO_0x65,
        FARM_DEMO_0x66,
        FARM_DEMO_0x67,
        FARM_DEMO_0x68,
        FARM_DEMO_0x69,
        CLEAR_DEMO_STATE,
        INVALID_ID = 0xFF,
    };
private:
    DemoId();
};

union CameraState {
    u32 word;
    u8 byte;
};

struct CameraQuake {
    ScePspFVector4 epicenter;
    bool is_active;
    u8 magnitude_id;
    s16 timer;
    u8 radius_id;
};

struct UnalignedCameraFollowHeightEntry {
    u32 pad_0x0;
    float y_offset;
    float z_offset;
    u32 pad_0xC;
    float height;
    u32 pad_0x14;
    float min_height;
};

typedef struct CameraFollowHeightEntry {
    u32 pad_0x0;
    float y_offset;
    float z_offset;
    u32 pad_0xC;
    float height;
    u32 pad_0x14;
    float min_height;
    u32 pad_0x1C;
} CameraFollowHeightEntry;

struct CameraFollowDefinition {
    float fov;
    float roll;
    u32 pad_0x8;
    u16 pad_0xC;
    u16 pad_0xE;
    ScePspFVector4 base_offset;
    CameraFollowHeightEntry entries[7];
};

struct CameraRailDefinition {
    ScePspFVector4 cam_points[16];
    ScePspFVector4 target_points[16];
    float fovs[16];
    s16 rolls[16];
    u8 count;
    u8 enable_y;
    u8 pad[14];
    ScePspFVector3 model_offset;
    ScePspFVector3 world_offset;
};

struct CameraPanDefinition {
    ScePspFVector3 camera_position;
    ScePspFVector3 target_position;
    ScePspFVector3 model_offset;
    ScePspFVector3 world_offset;
    float fov;
    float roll;
};

struct CameraHokanInfo {
    u8 area_id;
    u8 steps;
};

struct CameraAreaZoneFlag {
    enum {
        ENTER = 1 << 0,
        EXIT = 1 << 1,
    };
private:
    CameraAreaZoneFlag();
};

struct CameraAreaZone {
    float vertices[8];
    ScePspFVector3 position;
    u8 flags;
    ScePspFVector3 normal;
    float height;
};

struct CameraAreaMoveType {
    enum {
        STD,
        FIXED_PAN,
        RAIL,
        OFFSET_PAN,
    };
private:
    CameraAreaMoveType();
};

struct CameraAreaAxes {
    enum {
        Y,
        XZ,
        INVALID_AXES,
    };
private:
    CameraAreaAxes();
};


struct CameraAreaTargetType {
    enum {
        FIXED,
        PLAYER_MODEL_OFFSET,
        PLAYER_WORLD_OFFSET,
    };
private:
    CameraAreaTargetType();
};

struct CameraAreaCnf {
    u8 magic[1];
    u8 id;
    u8 move_type;
    u8 axes;
    u8 target_type;
    u8 zone_count;
    u8 attributes;
    u8 pad_0x7;
    float near_distance;
    float far_distance;
    float near_fov;
    float far_fov;
    CameraAreaZone *zones;
    CameraHokanInfo *hokan_info;
    union {
        CameraFollowDefinition height;
        CameraPanDefinition pan;
        CameraRailDefinition rail;
        u8 bytes[0x2E0];
    };
};

struct CameraAreaList {
    s32 length;
    CameraAreaCnf **entries;
};

struct CameraAreaHeader {
    u16 version;
    u16 count;
    u16 rows;
    u16 columns;
    u16 unknown_0x8;
    u16 unknown_0xA;
    u32 unknown_0xC;
    u32 unknown_0x10;
    u32 unknown_0x14;
    u32 unknown_0x18;
    CameraAreaList *index;
    CameraAreaCnf **cnfs;
    u32 unknown_0x24;
    CameraAreaCnf *first;
    s32 unknown_0x2C;
};

struct StdCameraData {
    ScePspFVector4 position;
    ScePspFVector4 target;
    ScePspFVector4 previous_position;
    float ground_y;
    s8 ground_hit;
    u8 is_falldown;
    s16 falldown_timer;
    float ground_y_adj;
    u8 pad_0x3C[0x40 - 0x3C];
    ScePspFVector4 goal_position;
    ScePspFVector4 goal_target;
    float fov;
    float goal_fov;
    float roll;
    CameraFollowDefinition *cnf_chs;
    CameraFollowHeightEntry *cnf_chs_entry;
    float wall_distance;
    u32 unk_0x78;
    u8 pad_0x7C[0x80 - 0x7C];
    s16 goal_rotation;
    s16 rotation;
    u16 buttons;
    u16 rising_edge;
    u8 unk_0x88;
    u8 is_view_blocked;
    u8 unk_0x8A;
    u8 kabegiwa_timer;
    bool is_kabegiwa;
    u8 inertia_timer;
    bool is_fast_rotate;
    bool is_shoulder_cam;
    bool is_ground_adj;
    s8 gun_targeting_state;
    s8 shoulder_cam_timer;
    u8 sdc_flag;
    u32 unk_0x94;
};

struct GunnerCameraData {
    ScePspFVector4 position;
    ScePspFVector4 target;
    ScePspFVector4 start_position;
    s16 aim_angle;
    s16 rotation_offset;
    s16 goal_rotation_offset;
};

struct StgCameraData {
    ScePspFVector4 position;
    ScePspFVector4 target;
    ScePspFVector4 last_position;
    ScePspFVector4 last_target;
    float fov;
    float roll;
    float last_fov;
    float last_roll;
    s16 pitch_adj;
    s16 rotation_adj;
    s32 pad_0x54;
    s16 pitch;
    s16 rotation;
};

struct PachiType {
    enum {
        BOWGUN,
        BINOCULARS,
        BALLISTA,
        NET_LAUNCHER,
        INVALID_TYPE = 0xFF,
    };
private:
    PachiType();
};

struct PchngrCameraData {
    ScePspFMatrix4 mat;
    ScePspFVector4 player_position;
    float min_fov;
    float max_fov;
    float inv_fov_range;
    float bowgun_fov;
    float binoculars_fov;
    u32 pad_0x64;
    s8 pachi_type;
    u8 crosshairs;
    u8 is_variable;
};

struct FishingCameraData {
    CameraState state;
    u32 checkResult;
    void *stage_unique;
};

struct ZoomCameraData {
    CameraState state;
    Npc *npcs[3];
    ScePspFVector4 position;
    ScePspFVector4 target;
    u8 operation;
    u8 target_types[3];
    s16 timer_total;
    s16 timer;
    float y_offsets[3];
    float z_offsets[3];
    float fov;
    int stack_size;
    float fovs[3];
    ScePspFVector4 positions[3];
    ScePspFVector4 targets[3];
};

struct PlayerEXCameraData {
    u8 pad_0x0[4];
    FishingCameraData fishing_cam;
    ZoomCameraData zoom_cam;
};

struct CameraAngle {
    s16 pitch;
    s16 yaw;
};

union CameraCommand {
    struct {
        s8 op;
        s8 words;
    } type0;

    struct {
        s8 op;
        s8 words;
        s8 arg0;
    } type1;

    struct {
        s8 op;
        s8 words;
        s8 arg0;
        s8 arg1;
    } type11;

    struct {
        s8 op;
        s8 words;
        s16 arg0;
    } type2;

    struct {
        s8 op;
        s8 words;
        s32 arg0;
    } type4;

    struct {
        s8 op;
        s8 words;
        s32 arg0;
        s32 arg1;
        s32 arg2;
    } type444;
};

struct DemoCameraData {
    ScePspFVector4 pos_start;
    ScePspFVector4 pos_end;
    ScePspFVector4 tar_start;
    ScePspFVector4 tar_end;
    ScePspUnion32 *pc;
    ScePspUnion32 *loop_pc;
    u8 move_type;
    u8 pos_offset_type;
    u8 tar_offset_type;
    u8 pos_offset_joint_id;
    u8 tar_offset_joint_id;
    u8 follow_target;
    u8 demo_id;
    s8 demo_state;
    Enemy *enemy;
    float offset_start;
    float offset_end;
    CameraAngle pitch_yaw_start;
    CameraAngle pitch_yaw_end;
    s16 roll_start;
    s16 roll_end;
    s16 fov_start;
    s16 fov_end;
    s16 truck_shake_phase;
    s16 truck_shake_rate;
    s16 truck_shake_magnitude_start;
    s16 truck_shake_magnitude_end;
    s16 jib_shake_phase;
    s16 jib_shake_rate;
    s16 jib_shake_magnitude_start;
    s16 jib_shake_magnitude_end;
    s16 roll_shake_phase;
    s16 roll_shake_rate;
    s16 roll_shake_magnitude_start;
    s16 roll_shake_magnitude_end;
    s16 fov_shake_phase;
    s16 fov_shake_rate;
    s16 fov_shake_magnitude_start;
    s16 fov_shake_magnitude_end;
    union {
        struct {
            u8 truck; // unimplemented
            u8 jib; // unimplemented
            u8 roll;
            u8 fov;
        } shake_rng;
        u8 shake_rngs[4];
    };
    ScePspUnion32 *jump_pc;
    float interpolation_exponent;
    u8 interpolation_type;
    u8 error;
    u8 is_quest_clear;
    u8 enable_stage_collision;
    u8 ex_ev_id;
};

union SubCameraData {
    StdCameraData std;
    GunnerCameraData gunner;
    StgCameraData stg;
    PchngrCameraData pchngr;
    PlayerEXCameraData playerEX;
    DemoCameraData demo;
};

struct SubCameraId {
    enum {
        STD,
        GUNNER,
        STG,
        PCHNGR,
        PLAYER_EX,
        DEMO,
        MAX_ID,
        INVALID_ID = 0xFF,
    };
private:
    SubCameraId();
};

struct CameraRailPoint {
    float coord;
    float t;
    u8 spline;
};

struct Complex {
    float re;
    float im;
};

struct Camera;

struct SubCamera {
    typedef void (SubCamera::*mem_fn)();

    SubCamera() {
        unknown_0x90 = 0;
        current_sub_camera_id = 0;
    }
    ~SubCamera() {}

    ScePspFVector4 current_position;
    ScePspFVector4 current_target;
    ScePspFVector4 current_up;
    ScePspFVector4 current_direction;
    ScePspFVector4 last_position;
    ScePspFVector4 last_target;
    ScePspFVector4 previous_up;
    ScePspFVector4 previous_direction;
    float current_roll;
    float last_roll;
    float current_fov;
    float last_fov;
    u8 unknown_0x90;
    bool isActive;
    u8 current_sub_camera_id;
    s16 timer;
    s16 timer_total;
    CameraState cam_sub_mode;
    CameraState cam_sub_state;
    SubCameraData data;
    mem_fn cam_sub_impl;
    u8 cam_type;
    float hokan_divisor;

    void cam_init(u8 type);
    void cam_sub();
    void cam_init_sub_std();
    void cam_sub_std();
    void cam_init_sub_gunner();
    void cam_sub_gunner();
    void cam_init_sub_stg();
    void cam_sub_stg();
    void cam_init_sub_pchngr();
    void cam_sub_pchngr();
    void cam_init_sub_playerEX();
    void cam_sub_playerEX();
    void cam_init_sub_demo();
    void cam_sub_demo();
    int CamRailMove(ScePspFVector4 *, bool);
    void cam_rail_move_sub(CameraRailPoint *point, CameraRailDefinition *rail, int spline, float t);
    int cam_rail_move_0(CameraRailPoint *point, CameraRailDefinition *rail, ScePspFVector4 *position);;
    int cam_rail_move(CameraRailPoint *point, CameraRailDefinition *rail, ScePspFVector4 *position);;
    void cam_plEX_fishing(FishingCameraData *);
    bool fish_cam_sub(FishingCameraData *);
    void cam_plEX_zoom(ZoomCameraData *);
    float zoom_cam_rate(s16 timer, s16 total_timer, u8 state);\
    int point_camera();
    int point_cam_sub();
    void CamRailPoint(ScePspFVector4 *out, ScePspFMatrix4 *coeff, float t);
    void GetRailTarget(ScePspFVector4 *out, CameraAreaCnf *data, ScePspFVector4 *in);
    void GetRailCamPos(ScePspFVector4 *out, CameraAreaCnf *data);
    int GetNearSection(CameraRailDefinition *definition, ScePspFVector4 *position);
    int GetNearPoint(CameraRailPoint *point, CameraRailDefinition *rail, ScePspFVector4 *position);
    int get_near_point_sub(CameraRailPoint *point, ScePspFVector4 *section, float *idk, int n);
    int GetOrthogonalPoint(float *idk, ScePspFMatrix4 *spline_section, ScePspFVector4 *postion, int enable_y);
    float ZoomRateCalc(CameraAreaCnf *d, float distance);
    float ZoomBaseAngleRail(CameraRailDefinition *definition, int spline, float t);
    float RollAngleRail(CameraRailDefinition *definition, int spline, float t);
    void Spline(ScePspFVector4 *points, int num_points);
    void tri_diag(float *out, float *subdiag, float *diag, float *superdiag, float *in, int equations);
    int ex_ev_camera();
    bool ex_ev_cam_sub();
    void std_cam_sw_set_sub();
    void GetPanTarget(ScePspFVector4 *out, CameraAreaCnf *data);
    float func_eboot_0888CEC0();
    bool func_eboot_0888CFB8();
    void kabegiwa_cam_chk();
    bool Manual_cam_chk();
    bool Fishing_cam_chk();
    bool pl_falldown_status();
    u8 PachiTypeCheck();
    void pachinger_mat(ScePspFMatrix4 *out, s32 alpha, s32 beta, ScePspFVector4 *position);
    void DKAS(Complex *out, float *coeffs);
    int Cardano(float *out, float *coeffs);
    float vInnerProductXYZ(ScePspFVector4 *a, ScePspFVector4 *b);
    float vInnerProductXZ(ScePspFVector4 *a, ScePspFVector4 *b);
    ScePspFMatrix4 *get_em_local();
    void get_angle(CameraAngle *out);
    void Camera_hokan_start(int steps);
    void Camera_hokan_chk(CameraAreaCnf *area);
    float Camera_hokan_sub();
    bool Cam_senkai_chk();
    float cmGetGroundHit(ScePspFVector4 *camera_position, Player *player);
    s16 SenkaiChousei(u32 angle, float min, float max, float rate);
    void cmd_set_pos(ScePspFVector4 *out, CameraCommand *pc);
    void cmd_set_tar(ScePspFVector4 *out, CameraCommand *pc);
    void cmd_copy(int flags);
    void cmd_cam_move(CameraCommand *pc);
    void Pl_OoS_Adj();
    bool posa_sphere_make(ScePspFVector4 *);
    void sdc_flag_set();
    bool pl_approaching_wall_chk();

private:
    inline void set_cam_sub(mem_fn fn) {
        if (fn) {
            cam_sub_impl = fn;
        }
    }

    inline float interpolant();
};

struct Camera : Singleton<Camera> {
    float near_z;
    float far_z;
    float aspect_ratio;
    float fov;
    float fov_cache;
    u8 height_id;
    u8 padding_0x15[0x20 - 0x15];
    ScePspFVector4 last_position;
    ScePspFVector4 last_target;
    u8 padding_0x40[0xA0 - 0x40];
    float last_roll;
    u32 padding_0xA4;
    float last_fov;
    u8 padding_0xAC[0xB0 - 0xAC];
    SubCamera subCameras[6];
    u16 buttons;
    u16 rising_edge;
    u16 falling_edge; // unused
    u16 analog_angle;
    u16 analog_magnitude;
    bool is_yama_tsukami_quest_clear_0;
    Enemy *demo_enemy;
    u8 next_demo_id;
    bool wyvern_find_player_flag;
    Player *player;
    s8 is_std_cam;
    u8 unused_0xA89;
    s8 unknown_0xA8A;
    bool is_changing_area;
    u8 last_sub_camera_id;
    u8 area_sub_camera_id;
    CameraAreaCnf *area_cnf;
    u8 area_id;
    CameraRailPoint rail_point;
    u8 padding_0xAA4[0xAA9 - 0xAA4];
    bool enableCameraControls;
    bool zClipping;
    u8 next_area_id;
    CameraAreaHeader *area_data;
    CameraQuake quakes[3];
    CameraState state;
    bool is_areas_disabled;
    u16 area_row;
    u16 area_col;
    u8 padding_0xB1A[0xB1C - 0xB1A];
    u16 area_row_count;
    u16 area_col_count;
    s16 unknown_0xB20;
    s16 unknown_0xB22;
    s32 unknown_0xB24;
    s32 unknown_0xB28;
    s32 unknown_0xB2C;
    s32 unknown_0xB30;
    float roll;
    u8 padding_0xB38[0xB3C - 0xB38];
    s8 unknown_0xB3C;
    u8 padding_0xB3D[0xB40 - 0xB3D];
    ScePspFVector4 position;
    ScePspFVector4 target;
    ScePspFVector4 up;
    ScePspFVector2 viewport_offset;
    ScePspFMatrix4 worldToView;
    ScePspFMatrix4 nworldToView;
    ScePspFMatrix4 viewToProjection;
    ScePspFMatrix4 viewToNworld;
    ScePspFMatrix4 viewToWorld;
    ScePspFMatrix4 rview_matY;
    ScePspFMatrix4 projectionToView;
    u8 pad_0xD40[16];
    ScePspFVector4 left_clip_plane;
    ScePspFVector4 right_clip_plane;
    ScePspFVector4 top_clip_plane;
    ScePspFVector4 bottom_clip_plane;
    float clip_max_z;
    float clip_min_z;
    u8 pad_0xD98[8];
    ScePspVector3 viewport_scale;
    ScePspVector3 viewport_center;
    float distance;
    u8 padding_0xDBC[4];
    float unknown_0xDC0;
    s8 sub_camera_id;
    u8 padding_0xDC5[0xDD0 - 0xDC5];

    Camera();
    ~Camera() {};

    void View_initialize();
    void CameraInit();
    void View_init();
    void CameraMove();
    void quake_sub(CameraQuake *);
    void set_quake_sub(int, ScePspFVector4 *);
    void set_quake_sub2(int);
    void Em_set_quake_sub(int, ObjBase *);
    void Em_set_quake_sub_W(int, ObjBase *, int);
    void Pachinger_set_quake_sub(int, ObjBase *);
    bool Camera_kyu_senkai_chk();
    void WyvernFindPlayer(Player *);
    void BBQcamera_set(Player *);
    void NPCZoomInCameraRequest(Npc *, bool);
    void BookEditCameraRequest(float, float);
    void NPCZoomInCameraCancel(bool);
    bool NPCZoomInCameraCheck();
    bool NPCZoomInCameraCancelCheck();
    void LobbyFixCameraRequest(ScePspFVector4 *, ScePspFVector4 *, float *, bool);
    void NPCZoomInCameraCancel2(bool);
    void DemoCameraRequest(int, Enemy *, bool);
    void DemoCameraRequest(int, Enemy *);
    void QuestClearCameraRequest(bool);
    void FB_DieCameraRequest();
    void EnemyEscapeCamera(Enemy *);
    bool check_clear_camera_disable();
    void PlayerDieCameraRequest();
    void PlComebackCameraRequest();
    void PilebunkerCameraRequest();
    void FishWyvernCameraRequest(Enemy *enemy);
    void DemoCameraCancel();
    int DemoCameraCheck();
    u8 GetDemoCameraID();
    u8 DemoCameraCheckQC();
    bool method_08814BE0();
    int GetDemoCameraTimer();
    bool method_08814C50();
    bool method_08814CC4();
    bool PachingerCamChk(Player *player);
    s8 GetPachingerInfo(u8 *crosshairsOut, u8 *isVariableOut, float *zoomCursorOut);
    void GetCameraEye(ScePspFVector4 *out);
    void GetCameraAt(ScePspFVector4 *out);
    void GetCameraUp(ScePspFVector4 *out);
    bool player_trnsl_chk(u8 *alphaOut, ObjBase *player);
    void flmatMakeLookAt(ScePspFVector4 *eye, ScePspFVector4 *at, ScePspFVector4 *up);
    void flmatrMakeProjection(ScePspFMatrix4 *out, float fov, float aspect, float near, float far);
    void set_viewproj(float fov, float aspect, float near, float far);
    void project(ScePspFVector4 *out, ScePspFVector4 *in);
    u16 Get_view_dir();
    void StageCamInit();
    void SetCameraData(CameraAreaHeader *);
    void default_area_data();
    void cam2view();
    void cameraUpdateInput();
    void set_to_std_cam(int);
    s8 SetAreaData();
    u8 CameraAreaCheck(CameraAreaCnf *, u8);
    u8 Area_XZ_Check(CameraAreaZone *, ScePspFVector4 *);
    void set_aov();
    void View_move();
    void view_set(ScePspFVector4 *, ScePspFVector4 *, float);
    void view_set_move();
    void last_view_set(ScePspFVector4 *, ScePspFVector4 *, float);
    void plMakePlaneFromPoints(ScePspFVector4 *, ScePspFVector4 *, ScePspFVector4 *, ScePspFVector4 *);
    void plSetupFOVClipPlanes();
    bool z_clip_check(ScePspFVector4 *, float);
    bool clip_check(ScePspFVector4 *, float);
    bool clip_check(ScePspFVector4 *, float, float);
    void SetViewport();

private:
    inline void DemoCameraRequest(int demo_id) {
        DemoCameraRequest(demo_id, NULL, demo_id == DemoId::YAMA_TSUKAMI_QUEST_CLEAR_0);
    }

    inline void RollView(float roll) {
        this->roll = roll;
    }

    inline void SetAngleOfView(float fov) {
        this->fov = fov;
    }

    inline int CamAreaAttribChk(CameraAreaCnf *);
};

void flmatMakeLookAt(ScePspFMatrix4 *out, ScePspFVector4 *position, ScePspFVector4 *target, ScePspFVector4 *up);
void Roll2Upvec(ScePspFVector4 *out, ScePspFVector4 *position, ScePspFVector4 *target, float roll);
