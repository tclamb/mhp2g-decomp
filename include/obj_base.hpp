#pragma once

#include "model_base.hpp"

struct ObjFlag {
    enum {
        WALL_COLLISION_ENABLED = 1 << 0,
        NECK_LOOK_AT_OVERRIDE = 1 << 1,
        ACTION_UPDATE_PENDING = 1 << 2,
        WEAPON_DRAWN = 1 << 3,
        DASHING = 1 << 4,
        IN_WATER = 1 << 5,
        ON_SOFT_GROUND = 1 << 6,
        WALL_HIT_REQUEST = 1 << 7,
        LARGE_ENEMY_NEAR = 1 << 8,
        HAIR_YURE = 1 << 9,
    };
private:
    ObjFlag();
};

// stub for type with vtable at D_eboot_089B5C10
struct ObjBase : ModelBase {
    typedef void (ObjBase::*mem_fn)();

    ObjBase();
    virtual ~ObjBase();
    virtual void draw();
    virtual void vtable_0x10();
    virtual void vtable_0x14();
    virtual void vtable_0x18();
    virtual void vtable_0x1C();
    virtual void vtable_0x20();
    virtual void vtable_0x24();
    virtual void vtable_0x28();
    virtual void vtable_0x2C();
    virtual u8 vtable_0x30();
    virtual void vtable_0x34(float *, float *);
    virtual void vtable_0x38() = 0;
    virtual void vtable_0x3C();

    static void *operator new(u32, void *);
    static void operator delete(void *);

    void hit_stop_calc();
    void pl_flag_set(u32 flag);
    void pl_flag_clr(u32 flag);
    void method_08865AD4(u16, int, int);
    bool Pl_stg_ck();
    void World_calc();
    void World_calc2(ScePspFVector4 *out, u16 stageId, ScePspFVector4 *in);
    u32 pl_flag_ck(u32 flag);
    bool act_ck(u8 type, u8 id);
    void em_wall_bit_set();
    void method_08865D7C();
    int rate_g_calc(s16);

    u32 objListId;
    ObjBase *nextObj;
    ObjBase *prevObj;
    u8 padding_0x1C[8];
    u8 state_0x1D4;
    u8 unknown_0x1D5;
    u8 unknown_0x1D6;
    u8 unknown_0x1D7;
    u16 unknown_0x1D8;
    ScePspFVector4 (*diffuse_light_color_override)[3];
    u32 unknown_0x1E0;
    u8 index;
    u16 pl_id;
    u8 pl_type;
    u8 kind;
    ScePspIVector4 rotation __attribute__((aligned(16)));
    ScePspFVector4 position;
    ScePspFVector4 next_position;
    ScePspFVector4 scale;
    u8 alpha;
    ScePspFVector4 velocity;
    ScePspFVector4 acceleration;
    ScePspFVector4 world_position;
    float unknown_0x270;
    s16 unknown_0x274;
    u8 demo_on_timer;
    u8 padding_0x277[5];
    u16 wall_bit;
    u8 padding_0x27E[2];
    u8 posture;
    u8 padding_0x281[4];
    u8 unknown_0x285;
    u32 unknown_0x288;
    u32 unknown_0x28C;
    u8 unknown_0x290;
    u8 padding_0x291[7];
    u8 action_type;
    u8 action_id;
    u16 stageId;
    u8 em_ride_state;
    u8 padding_0x29D[0x2A4 - 0x29D];
    u8 stageTransitionType;
    u8 padding_0x2A5[0x2AC - 0x2A5];
    ScePspUnion32 diffuse_light_colors[3];
    u8 padding_0x2B8[2];
    u16 unknown_0x2BA;
    u16 unknown_0x2BC;
    u8 padding_0x2BE[0x2C0 - 0x2BE];
    u32 em_dir;
    u8 padding_0x2C4[0x2E4 - 0x2C4];
    s16 current_health;
    u8 padding_0x2E6[0x320 - 0x2E6];
    u16 next_rotation;
    u8 padding_0x322[0x324 - 0x322];
    ScePspSVector4 unknown_0x324;
    u8 padding_0x32C[0x33C - 0x32C];
    u8 unknown_0x33C;
    u8 padding_0x33D[0x396 - 0x33D];
    u8 unknown_0x396;
    u8 padding_0x397[0x3FC - 0x397];
    mem_fn memFn;
    u32 unknown_0x408;
    u32 unknown_0x40C;
    u32 objFlags;
    u32 unknown_0x414;
    u8 padding_0x418[0x480 - 0x418];

protected:
    inline void setMemFn(mem_fn f) {
        if (f != 0) {
            memFn = f;
        }
    }
};
