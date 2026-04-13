#include "item_manager.hpp"
#include "system.hpp"

#include <pspthreadman.h>

extern "C" {
    void mwInit();
    void mwExit();
}

static System system;

int main() {
    sceKernelChangeThreadPriority(0, 0x30);
    mwInit();
    Singleton<System>::objectPtr->run();
    mwExit();
}

inline TaskManager::~TaskManager() {
    int i = 0;
    for (; i < 4; ++i) {
        if (active[i]) {
            delete active[i];
            cache.free(active[i]);
            active[i] = 0;
        }
    }
}

void forceEmitGlobalFileSysDestructor() {
    GlobalFileSys unused;
}

System::~System() {}
GlobalMemoryStick::GlobalMemoryStick() {}
GlobalEvdemo::GlobalEvdemo() {}
GlobalEquipManager::GlobalEquipManager() {}
ItemManager::ItemManager() {}
GlobalResultCheck::GlobalResultCheck() {}
GlobalQuest::GlobalQuest() {}
GlobalQuestNet::GlobalQuestNet() {}
GlobalLobbyNet::GlobalLobbyNet() {}
GlobalNet::GlobalNet() {}
GlobalOsk::GlobalOsk() {}
GlobalCCC::GlobalCCC() {}
SystemFont::SystemFont() {}
GlobalVramManager::GlobalVramManager() {}
GlobalFileSys::GlobalFileSys() {}
GlobalPad::GlobalPad() {}
TaskManager::TaskManager() {
    active[0] = NULL;
    active[1] = NULL;
    active[2] = NULL;
    active[3] = NULL;
    cache.reset(slab, sizeof(slab));
}
GlobalGe::GlobalGe() {}
