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

struct GlobalFileSys : InlineConstructorSingleton<FileSys>, FileSys {
    GlobalFileSys();
    virtual ~GlobalFileSys() {}
};

struct System : NoInlineConstructorSingleton<System> {
    static System *objectPtr;

    Ge ge;
    TaskManager taskManager;
    Pad pad;
    GlobalFileSys fileSys;
    VramManager vramManager;
    Sound sound;
    ObjManager objManager;
    DrawManager drawManager;
    ResourceManager resourceManager;
    SystemFont systemFont;
    CCC ccc;
    Osk osk;
    Net net;
    LobbyNet lobbyNet;
    QuestNet questNet;
    Camera camera;
    LightManager lightManager;
    GameSys gameSys;
    Cockpit cockpit;
    Quest quest;
    ResultCheck resultCheck;
    ItemManager itemManager;
    EquipManager equipManager;
    Evdemo evdemo;
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

    MemoryStick memoryStick;

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
