#include "audio_cycle.hpp"

extern "C" u8 func_game_task_09A5E9F0(GameTaskAudioState *task) {
    switch (task->sound_index) {
    case 0:
        func_eboot_088805E0(Sound::objectPtr, 1, 0x1506, 0x1505, 0x1507);
        ++task->sound_index;
        break;
    case 1:
        if (func_eboot_08881674(Sound::objectPtr) == 1) break;
        if (*(u8 *)((u8 *)GameSys::objectPtr + 0x41A) == 1) {
            ++task->sound_index;
        } else {
            task->sound_index = 0;
            return 1;
        }
    case 2:
        func_eboot_088805E0(Sound::objectPtr, 3, 0x1515, 0x1514, 0x1516);
        ++task->sound_index;
        break;
    case 3:
        if (func_eboot_08881674(Sound::objectPtr) != 1) {
            task->sound_index = 0;
            return 1;
        }
        break;
    }
    return 0;
}
