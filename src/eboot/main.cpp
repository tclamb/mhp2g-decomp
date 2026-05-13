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
    System::objectPtr->run();
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
MemoryStick::MemoryStick() {}
Evdemo::Evdemo() {}
EquipManager::EquipManager() {}
ItemManager::ItemManager() {}
ResultCheck::ResultCheck() {}
Quest::Quest() {}
QuestNet::QuestNet() {
    unknown_0x0 = 0;
}
LobbyNet::LobbyNet() {
    unknown_0x4 = 0;
}
Net::Net() {}
Osk::Osk() {
    visible = 0;
}
CCC::CCC() {}
SystemFont::SystemFont() {}
VramManager::VramManager() {}
GlobalFileSys::GlobalFileSys() {}
Pad::Pad() {}
TaskManager::TaskManager() {
    active[0] = NULL;
    active[1] = NULL;
    active[2] = NULL;
    active[3] = NULL;
    cache.reset(slab, sizeof(slab));
}
Ge::Ge() {}
