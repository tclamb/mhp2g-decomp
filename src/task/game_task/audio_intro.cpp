#include "audio_intro.hpp"

extern "C" s32 func_game_task_09A5EAF8(GameTaskAudioState *task) {
    switch (task->sound_index) {
    case 0: {
        GameSys *game = GameSys::objectPtr;
        u16 stage_id = *(u16 *)((u8 *)game + 0x6AF0E);
        u16 *pair = D_eboot_089A6470 + stage_id * 2;
        func_eboot_088805E0(Sound::objectPtr, 6, pair[0], pair[1], 0x1519);
        *(u16 *)((u8 *)Sound::objectPtr + 0x562468) = pair[0];
        ++task->sound_index;
    }
    case 1:
        if (func_eboot_08881674(Sound::objectPtr) != 1) {
            task->sound_index = 0;
            return 1;
        }
        break;
    }
    return 0;
}
