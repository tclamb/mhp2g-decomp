#include "audio_outro.hpp"

extern "C" s32 func_game_task_09A5ED28() {
    if (*(u32 *)((u8 *)Evdemo::objectPtr + 0x88) != 1) goto return_one;
    if ((u8)func_eboot_08885D94(BgmServer::objectPtr) != 1) goto check_evdemo;
    return 0;
check_evdemo:
    if (func_eboot_088D05F8(Evdemo::objectPtr) == 0) goto return_one;
    return 0;
return_one:
    return 1;
}
