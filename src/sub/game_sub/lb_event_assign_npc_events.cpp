#include "common.h"
#include "lb_event.hpp"
#include "singleton.hpp"

struct NpcManager : Singleton<NpcManager> {};
extern "C" void *func_lobby_task_09AD73A0(NpcManager *, int);
extern "C" void func_lobby_task_09AB8350(void *, int, u16);

extern "C" void func_game_sub_09C17308(LbEvent *, u32 eventId) {
    int index = 0;
    do {
        void *npc = func_lobby_task_09AD73A0(NpcManager::objectPtr, index);
        if (npc != 0) {
            func_lobby_task_09AB8350(npc, 0, eventId);
        }
        ++index;
    } while (index < 5);
}
