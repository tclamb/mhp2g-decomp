// Two matched audio-transition functions from game_task_ovl.
#include "evdemo.hpp"
#include "game_sys.hpp"
#include "sound.hpp"

struct BgmServer : Singleton<BgmServer> {};

struct GameTaskAudioState {
    u8 reserved[0x40];
    s32 sound_index;
};

extern "C" u8 func_eboot_08881674(Sound *);
extern "C" void func_eboot_08880FB8(Sound *, u16, u16, s32, s32);
extern "C" void func_game_task_09A5FAC0(BgmServer *);
extern "C" void func_game_task_09A5FBE0(BgmServer *, u16);
extern "C" void func_eboot_08885D9C(BgmServer *, s32);
extern "C" u16 func_eboot_088D05D4(Evdemo *);
extern "C" u16 D_game_task_09BB2640[];
extern "C" u16 D_game_task_09BB2642[];

extern "C" s32 func_game_task_09A5EBC8(GameTaskAudioState *task) {
    if (*(u8 *)((u8 *)GameSys::objectPtr + 0x480) != 0) {
        return 1;
    }
    if (task->sound_index >= 0) {
        if (func_eboot_08881674(Sound::objectPtr) == 0) {
            s32 index_bytes = task->sound_index << 2;
            u16 sound_id = *(u16 *)((u8 *)D_game_task_09BB2640 + index_bytes);
            if (sound_id != 0xFFFF) {
                u16 sound_id2 = *(u16 *)((u8 *)D_game_task_09BB2642 + index_bytes);
                func_eboot_08880FB8(Sound::objectPtr, sound_id, sound_id2, 0, 0);
                ++task->sound_index;
            } else {
                task->sound_index = 0;
                return 1;
            }
        }
    }
    return 0;
}

extern "C" void func_game_task_09A5EC88(GameTaskAudioState *) {
    Evdemo *evdemo = Evdemo::objectPtr;
    if (*(u32 *)((u8 *)evdemo + 0x88) == 0) {
        func_game_task_09A5FAC0(BgmServer::objectPtr);
        func_eboot_08885D9C(BgmServer::objectPtr, 0);
    } else {
        u16 id = func_eboot_088D05D4(evdemo);
        if (id != 0xFFFF) {
            func_game_task_09A5FBE0(BgmServer::objectPtr, id);
            func_eboot_08885D9C(BgmServer::objectPtr, 1);
        } else {
            func_game_task_09A5FAC0(BgmServer::objectPtr);
            func_eboot_08885D9C(BgmServer::objectPtr, 0);
        }
    }
}
