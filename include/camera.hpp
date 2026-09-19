#pragma once

#include "common.h"
#include "enemy.hpp"
#include "npc.hpp"
#include "psptypes.h"
#include "singleton.hpp"
#include "player.hpp"

union SubCameraState {
    u32 word;
    u8 byte;
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
};

struct CameraHokanInfo {
    u8 area_id;
    u8 steps;
};

struct CameraDataEntry {
    u8 pad_0x0;
    u8 index;
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
    void *zones;
    CameraHokanInfo *hokan_info;
    union {
        CameraPanDefinition pan;
        CameraRailDefinition rail;
    };
};

struct StdCameraData {
    ScePspFVector4 position;
    ScePspFVector4 target;
    ScePspFVector4 previous_position;
    float ground_y;
    s8 ground_hit;
    u8 is_falldown;
    s16 falldown_timer;
    u8 pad_0x38[0x84 - 0x38];
    u16 buttons;
    u16 rising_edge;
};

struct PchngrCameraData {
    u8 pad_0x0[0x5C];
    float unknown_0x5C;
    float unknown_0x60;
};

struct FishingCameraData {
    SubCameraState state;
    u32 checkResult;
    void *stage_unique;
};

struct ZoomCameraData {
    SubCameraState state;
    Npc *npcs[3];
    ScePspFVector4 position;
    ScePspFVector4 direction;
    u8 animationState;
    u8 targetTypes[3];
    s16 animationTotalFrames;
    s16 operationFrame;
    float yCenters[3];
    float zSpacings[3];
    float fieldOfView;
    int stackSize;
    float fieldsOfView[3];
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
};

union SubCameraData {
    StdCameraData std;
    PchngrCameraData pchngr;
    PlayerEXCameraData playerEX;
    DemoCameraData demo;
};

struct SubCameraType {
    enum {
        STD,
        GUNNER,
        STG,
        PCHNGR,
        PLAYER_EX,
        DEMO,
    };
private:
    SubCameraType();
};

struct CameraRailPoint {
    float coord;
    float t;
    u8 spline;
};

struct SubCamera {
    typedef void (SubCamera::*mem_fn)();

    SubCamera() {
        unknown_0x90 = 0;
        unknown_0x92 = 0;
    }
    ~SubCamera() {}

    ScePspFVector4 current_position;
    ScePspFVector4 current_target;
    ScePspFVector4 last_camera_position;
    u8 pad_0x30[0x80 - 0x30];
    float current_roll;
    u32 pad_0x84;
    float current_fov;
    u32 pad_0x8C;
    u8 unknown_0x90;
    bool isActive;
    u8 unknown_0x92;
    u8 pad_0x93[0x94 - 0x93];
    s16 timer;
    s16 timer_total;
    SubCameraState cam_sub_mode;
    SubCameraState cam_sub_state;
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
    void cam_plEX_fishing(FishingCameraData&);
    bool fish_cam_sub(FishingCameraData &);
    void cam_plEX_zoom(ZoomCameraData&);
    float zoom_cam_rate(s16 timer, s16 total_timer, u8 state);\
    int point_camera();
    int point_cam_sub();
    void CamRailPoint(ScePspFVector4 *out, ScePspFMatrix4 *coeff, float t);
    void GetRailTarget(ScePspFVector4 *out, CameraDataEntry *data, ScePspFVector4 *in);
    void GetRailCamPos(ScePspFVector4 *out, CameraDataEntry *data);
    int GetNearSection(CameraRailDefinition *definition, ScePspFVector4 *position);
    int GetNearPoint(CameraRailPoint *point, CameraRailDefinition *rail, ScePspFVector4 *position);
    int get_near_point_sub(CameraRailPoint *point, ScePspFVector4 *section, float *idk, int n);
    int GetOrthogonalPoint(float *idk, ScePspFMatrix4 *spline_section, ScePspFVector4 *postion, int enable_y);
    float ZoomRateCalc(CameraDataEntry *d, float distance);
    float ZoomBaseAngleRail(CameraRailDefinition *definition, int spline, float t);
    float RollAngleRail(CameraRailDefinition *definition, int spline, float t);
    void Spline(ScePspFVector4 *points, int num_points);
    void tri_diag(float *out, float *subdiag, float *diag, float *superdiag, float *in, int equations);
    int ex_ev_camera();

    void std_cam_sw_set_sub();
    void GetPanTarget(ScePspFVector4 *out, CameraDataEntry *data);

    bool Manual_cam_chk();
    bool Fishing_cam_chk();
    bool pl_falldown_status();
    u8 PachiTypeCheck();
    void pachinger_mat(ScePspFMatrix4 *out, s16 alpha, s16 beta, ScePspFVector4 *position);
    void DKAS(float *out, float *coeffs);
    int Cardano(float *out, float *coeffs);
    float vInnerProductXYZ(ScePspFVector4 *a, ScePspFVector4 *b);
    float vInnerProductXZ(ScePspFVector4 *a, ScePspFVector4 *b);
    ScePspFMatrix4 *get_em_local();
    void get_angle(CameraAngle *out);
    void Camera_hokan_start(int steps);
    void Camera_hokan_chk(CameraDataEntry *area);
    float Camera_hokan_sub();
    float cmGetGroundHit(ScePspFVector4 *camera_position, Player *player);
    int SenkaiChousei(int angle, float, float, float);
    void cmd_set_pos(ScePspFVector4 *out, CameraCommand *pc);
    void cmd_set_tar(ScePspFVector4 *out, CameraCommand *pc);
    void cmd_copy(int flags);
    void cmd_cam_move(CameraCommand *pc);

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
    float unknown_0x8;
    float unknown_0xC;
    float unknown_0x10;
    u8 unknown_0x14;
    u8 padding_0x15[0xB0 - 0x15];
    SubCamera subCameras[6];
    u16 buttons;
    u16 rising_edge;
    u8 padding_0xA74[0xA7C - 0xA74];
    Enemy *demo_enemy;
    u8 next_demo_id;
    s8 unknown_0xA81;
    Player *player;
    s8 unknown_0xA88;
    u8 unknown_0xA89;
    s8 unknown_0xA8A;
    bool changeStageCamera;
    u8 padding_0xA8C[0xA8D - 0xA8C];
    u8 base_sub_type;
    CameraDataEntry *areas;
    u8 area_id;
    CameraRailPoint rail_point;
    u8 padding_0xAA4[0xAA9 - 0xAA4];
    bool enableCameraControls;
    bool zClipping;
    u8 padding_0xAAB[0xAC0 - 0xAAB];
    s8 unknown_0xAC0;
    u8 padding_0xAC1[0xAE0 - 0xAC1];
    s8 unknown_0xAE0;
    u8 padding_0xAE1[0xB00 - 0xAE1];
    s8 unknown_0xB00;
    u8 padding_0xB01[0xB10 - 0xB01];
    u32 unknown_0xB10;
    u8 padding_0xB14[0xB34 - 0xB14];
    float unknown_0xB34;
    u8 padding_0xB38[0xB3C - 0xB38];
    s8 unknown_0xB3C;
    u8 padding_0xB3D[0xB70 - 0xB3D];
    float unknown_0xB70;
    float unknown_0xB74;
    u8 padding_0xB78[0xB80 - 0xB78];
    ScePspFMatrix4 perspective; // unsure
    ScePspFMatrix4 world;
    ScePspFMatrix4 projection;
    u8 padding_0xC40[0xCA0 - 0xC40];
    ScePspFVector4 position;
    u8 padding_0xCB0[0xDA0 - 0xCB0];
    ScePspVector3 viewport_scale;
    ScePspVector3 viewport_center;
    u8 padding_0xDB8[8];
    float unknown_0xDC0;
    u8 padding_0xDC4[0xDD0 - 0xDC4];

    Camera();
    ~Camera() {};

    void method_088137C8();

    void method_088138DC();
    void method_0881395C();
    void method_08813990();
    void method_08813B78();
    void method_08813CD8();
    void method_08813D24();
    void method_08813D50();
    void method_08813DE4();
    void method_08813E84();
    void method_08813F18();
    void method_08813F48();
    void method_08813F78();
    void method_08813FBC();
    void method_088140E0();
    void method_08814148();
    void method_08814258();
    void method_08814280();
    void method_088142A8();
    void method_08814318();
    void method_08814320();
    void method_08814334();
    void method_08814354();
    void method_088148F4();
    void method_088148FC();
    void method_08814A4C();
    void method_08814A88();
    void method_08814AB8();
    void method_08814B00();
    void method_08814B5C();
    void method_08814B6C();
    void method_08814B98();
    void method_08814BD0();
    void method_08814BD8();
    void method_08814C30();
    void method_08814CC4();
    void method_08814E08();
    void method_08814E40();
    void method_08814E84();
    void method_08814EA4();
    void method_08814EC4();
    void method_08814ED4();
    void method_08815028(ScePspFVector4 *, ScePspFVector4 *, ScePspFVector4 *);
    void method_08815274();
    void method_08815384();
    void method_08815434();
    void method_08815588();
    void method_08815744();
    void method_088157D4();
    void method_088159A0();
    void method_08815B44();
    void method_08815BD8();
    void method_08815DC4();
    void method_08815DE4();
    void method_08815EC8();
    void method_08816020();
    void method_08816108();
    void method_08816144();
    void method_088164C0();
    void method_08816698();
    void method_08816724();
    void method_08816B24();
    void method_08816B44();
    void method_08816BE8();
    void method_08816C5C();
    void method_08816E20();
    void method_08816EA8();
    void method_08816EB0();
    void method_08817024();
};

extern "C" {
    // clipping test; objects are clipped when false
    int func_eboot_08816EA8(Camera *, ScePspFVector4 *position, float clipping_distance);
    int func_eboot_08816E20(Camera *, ScePspFVector4 *, float);
    void func_eboot_08814E84(Camera *, ScePspFVector4 *);
    void func_eboot_088157D4(Camera *, void *);
}
