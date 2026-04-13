#pragma once

#define INCLUDE_ASM(path, function)
#define INCLUDE_RODATA(path, symbol)

#define ext(x, pos, size) (((x) >> (pos)) & ((1 << (size)) - 1))

#define DEGREES_TO_VFPU(degrees) ((degrees) / 90.0f)
#define PI 3.141592653589793238462643383279502884

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long int64_t;
typedef unsigned long uint64_t;

#include <psptypes.h> // IWYU pragma: export
#include <pspkerneltypes.h> // IWYU pragma: export

extern "C" {
    int memcmp(const void*, const void*, long unsigned int);
    void *memset(void*, int,  long unsigned int);
    void sceKernelDcacheWritebackRange(const void *p, unsigned int size);
    void sceKernelDcacheWritebackInvalidateRange(const void *p, unsigned int size);
    void sceKernelDcacheInvalidateRange(const void *p, unsigned int size);
    int sprintf(char*, const char*, ...);
}

struct render_group {
    enum {
        GROUP_0,
        RESET,
        GROUP_2,
        STAGE,
        GROUP_4,
        GROUP_5,
        GROUP_6,
        GROUP_7,
        GROUP_8,
        GROUP_9,
        GROUP_10,
        GROUP_11,
        GROUP_12,
        GROUP_13,
        GROUP_14,
        GROUP_15,
        GROUP_16,
        GROUP_17,
        GROUP_18,
        GROUP_19,
        GROUP_COUNT
    } value;
private:
    render_group();
};

struct stages {
    enum {
        GREAT_FOREST_N_2 = 259,
        GREAT_FOREST_N_7 = 264,
        TOWER_3 = 130,
    };

private:
    stages();
};

struct SkillType {
    enum {
        NONE,
        TORSO_INCREASE,
        PARALYSIS,
        SLEEP,
        FAINT,
        POISON,
        ANTISEPTIC,
        SNOW_RESISTANCE,
        SNEAK,
        HEALTH,
        RECOVERY_SPEED,
        SHARPNESS,
        ARTISAN,
        FENCING,
        EXPERT,
        SWORD_SHARPENER,
        GUARD,
        GUARD_UP,
        AUTO_GUARD,
        THROW,
        RELOAD,
        RECOIL,
        NORMAL_SHOT_UP,
        PIERCE_SHOT_UP,
        PELLET_SHOT_UP,
        NORMAL_SHOT_ADD,
        PIERCE_SHOT_ADD,
        PELLET_SHOT_ADD,
        CRAG_SHOT_ADD,
        CLUSTER_SHOT_ADD,
        SPECIAL_ATTACK,
        ELEMENTAL_ATTACK,
        BOMB_STRENGTH_UP,
        HUNGER,
        GLUTTONY,
        ATTACK,
        DEFENSE,
        PROTECTION,
        HEARING_PROTECTION,
        ANTI_THEFT,
        WIDE_AREA,
        BACKPACKING,
        ALL_RESISTANCE,
        FIRE_RESISTANCE,
        WATER_RESISTANCE,
        ICE_RESISTANCE,
        THUNDER_RESISTANCE,
        DRAGON_RESISTANCE,
        HEAT_RESISTANCE,
        COLD_RESISTANCE,
        WIND_PRESSURE,
        MAP,
        GATHERING,
        HI_SPEED_GATHERING,
        WHIM,
        FATE,
        FISHING,
        PSYCHIC_VISION,
        RECOVERY,
        MIX_SUCCESS_RATE,
        SHOT_MIX,
        ALCHEMY,
        EVADE,
        POTENTIAL,
        EVERLASTING,
        STAMINA,
        CAPACITY,
        PRECISION,
        COOKING,
        CARVING,
        TERRAIN,
        QUAKE_RESISTANCE,
        BBQ,
        GUNNERY,
        HORN,
        FATIGUE,
        EVADE_DISTANCE,
        SWORD_DRAW,
        COMRADE_GUIDE,
        COMRADE_ATTACK,
        COMRADE_DEFENSE,
        SPEED_SETUP,
        POISON_COATING_ADD,
        PARALYSIS_COATING_ADD,
        SLEEP_COATING_ADD,
        POWER_COATING_ADD,
        CLOSE_RANGE_COATING_ADD,
        GUTS,
        CONSTITUTION,
        TRANQUILIZER,
        PERCEIVE,
        RESIST_STATUS,
        EDGEMASTER,
        STEADY_HAND,
        FURY,
        ANTI_FIRE_DRAGON,
        ANTI_DAORA,
        ANTI_CHAMELEOS,
        SHORT_CHARGE,
        AUTO_RELOAD,
        SKILL_TYPE_COUNT,
    };
private:
    SkillType();
};

struct EnemyDetectionData {
    ScePspFVector4 unknown_0x0;
    u32 unknown_0x10;
    ScePspFVector3 unknown_0x14;
};

struct EnemyToleranceData {
    u16 initial;
    u16 recoveryFrames;
    u16 recovery;
    u16 durationFrames;
    u16 toleranceIncrement;
};

struct EnemyPoisonToleranceData {
    u16 initial;
    u16 recoveryFrames;
    u16 recoveryIncrement;
    u16 poisonDamage;
    u16 durationFrames;
    u16 perSecPoison; // ???
    u16 toleranceIncrement;
};

extern struct EnemyData {
    ScePspFVector4 unknown_0x0[4];
    float scale;
    ScePspFVector2 shadowScale;
    float unknown_0x4C;
    void *collisionPtr;
    u16 unknown_0x54;
    u16 unknown_0x56;
    u32 unknown_0x58;
    u32 flags_0x5C;
    EnemyDetectionData unknown_0x60;
    EnemyDetectionData unknown_0x80;
    ScePspFVector4 unknown_0xA0[6];
    u32 unknown_0x100;
    u32 unknown_0x104;
    float unknown_0x108;
    void *hitPtr;
    EnemyToleranceData paralysis;
    EnemyToleranceData knockout;
    EnemyToleranceData sleep;
    EnemyToleranceData unknown_0x12E;
    EnemyPoisonToleranceData poison;
    u16 playerReactionTimer;
    u16 unknown_0x148[7];
    void *navwayPtr;
    void *unknown_0x15C[27];

} D_game_task_09BB3C20[90];
