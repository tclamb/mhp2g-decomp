#pragma once

#include "common.h"
#include "ge.hpp"
#include "singleton.hpp"
#include "task_manager.hpp"
#include "pad.hpp"
#include "file_sys.hpp"
#include "vram_manager.hpp"
#include "sound.hpp"
#include "obj_manager.hpp"
#include "draw_manager.hpp"
#include "resource_manager.hpp"
#include "system_font.hpp"
#include "ccc.hpp"
#include "osk.hpp"
#include "net.hpp"
#include "lobby_net.hpp"
#include "quest_net.hpp"
#include "camera.hpp"
#include "light_manager.hpp"
#include "game_sys.hpp"
#include "cockpit.hpp"
#include "quest.hpp"
#include "result_check.hpp"
#include "item_manager.hpp"
#include "equip_manager.hpp"
#include "data_manager.hpp"
#include "evdemo.hpp"
#include "memory_stick.hpp"

#define DECLARE_GLOBAL(T) \
    struct Global ## T  : Singleton< T >, T { \
        Global ## T (); \
        inline ~Global ## T () {}; \
    }

DECLARE_GLOBAL(Ge);
DECLARE_GLOBAL(Pad);
DECLARE_GLOBAL(VramManager);
DECLARE_GLOBAL(CCC);
DECLARE_GLOBAL(Osk);
DECLARE_GLOBAL(Net);
DECLARE_GLOBAL(LobbyNet);
DECLARE_GLOBAL(QuestNet);
DECLARE_GLOBAL(FileSys);
DECLARE_GLOBAL(Quest);
DECLARE_GLOBAL(ResultCheck);
DECLARE_GLOBAL(EquipManager);
DECLARE_GLOBAL(Evdemo);
DECLARE_GLOBAL(MemoryStick);

#undef DECLARE_GLOBAL

struct System : NoInlineConstructorSingleton<System> {
    GlobalGe ge;
    TaskManager taskManager;
    GlobalPad pad;
    GlobalFileSys fileSys;
    GlobalVramManager vramManager;
    Sound sound;
    ObjManager objManager;
    DrawManager drawManager;
    ResourceManager resourceManager;
    SystemFont systemFont;
    GlobalCCC ccc;
    GlobalOsk osk;
    GlobalNet net;
    GlobalLobbyNet lobbyNet;
    GlobalQuestNet questNet;
    Camera camera;
    LightManager lightManager;
    GameSys gameSys;
    Cockpit cockpit;
    GlobalQuest quest;
    GlobalResultCheck resultCheck;
    ItemManager itemManager;
    GlobalEquipManager equipManager;
    GlobalEvdemo evdemo;
    DataManager dataManager;

    u32 loopCount;
    u8 unknown_0xF7A398[12];
    SceUID checkExitGameThreadId;
    u32 unknown_0xF7A3A8;
    u16 rngState[3];
    int languageId;
    u32 unknown_0xF7A3B8;
    SceBool wirelessIsOn;
    struct {
        u16 eboot;
        u16 task;
        u16 sub;
        u16 em;
        u16 stage;
        u16 debug;
    } activeOverlays;

    GlobalMemoryStick memoryStick;

    SceUID priorityChangerThreadId;
    SceUID userMainThreadId;
    SceUID changeThreadVtimerId;
    u32  vtimerTickCount;
    SceUID sha1ThreadId;
    bool sha1ThreadStarted;

    System() {
        wirelessIsOn = false;
        activeOverlays.eboot = -1;
        activeOverlays.task = -1;
        activeOverlays.sub = -1;
        activeOverlays.em = -1;
        activeOverlays.stage = -1;
        activeOverlays.debug = -1;
    }
    ~System();

    u16 next_index(u32 type);
    void run();
};

extern "C" {
    void func_eboot_0888FF00(System*, int, int);
    void func_eboot_0888FE4C(System*, int, int);
    SceBool func_eboot_0888FEE8(System*);
    void func_eboot_0888FF00(System*, int, int);
    void func_eboot_0888FE4C(System*, int, int);
    SceBool func_eboot_0888FEE8(System*);
}
