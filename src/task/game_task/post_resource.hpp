#pragma once

// Experimental GameTask helpers after the resource bundle.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef short s16;
typedef int s32;

template <typename T> struct Singleton { static T *objectPtr; };
struct GameTask;
struct PlayerManager : Singleton<PlayerManager> {};
struct GameSys : Singleton<GameSys> {};
struct StageManager : Singleton<StageManager> {};
struct Player {
    u8 reserved_000[0x298];
    u8 action_298;
    u8 reserved_299[0xB];
    u8 flag_2A4;
    u8 reserved_2A5[0x3F];
    s16 motion_2E4;
    u8 flag_2E6;
    u8 reserved_2E7[0x129];
    u32 flags;
};
struct Enemy {
    u8 reserved[0x410];
    u32 flags;
};
struct EnemyManager : Singleton<EnemyManager> {
    u8 reserved[0x1220];
    Enemy *enemies[20];
    inline Enemy *by_index(s32 index) {
        if (index < 0 || index >= 20) return 0;
        return enemies[index];
    }
};

extern "C" char *func_game_task_09A5E460(GameTask *, s32, u16);
extern "C" Player *method_088DF804__13PlayerManagerFi(PlayerManager *, s32);
extern "C" void func_game_sub_09C3C6D0(void *, s32);
extern "C" u8 func_eboot_0884F9A0(GameSys *, s32);
extern "C" void *intersecting_exit__12StageManagerFP14ScePspFVector4(StageManager *, void *);

