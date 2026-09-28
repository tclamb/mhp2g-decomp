// system.hpp declares func_eboot_0888FEE8 as returning SceBool, which is what the matched caller
// ContTask::load (cont_task.cpp, "== 0" without a mask) needs. The function itself returns bool
// (tail call to FileSys::is_loading, no andi), so its declaration is renamed away while system.hpp
// is included. Proper fix: bool in system.hpp and "!func_eboot_0888FEE8(...)" in cont_task.cpp.
#define func_eboot_0888FEE8 func_eboot_0888FEE8_header_decl
#include "system.hpp"
#undef func_eboot_0888FEE8

#include <psploadexec.h>
#include <pspintrman.h>

#include <psputility_modules.h>

extern "C" {
    SceUID sceKernelGetThreadId(void);
    int sceKernelChangeCurrentThreadAttr(int unknown, SceUInt attr);
    SceUID sceKernelCreateThread(const char *name, int (*entry)(SceSize, void *), int initPriority, int stackSize, SceUInt attr, void *option);
    int sceKernelStartThread(SceUID thid, SceSize arglen, void *argp);
    int sceKernelDeleteThread(SceUID thid);
    int sceKernelCreateCallback(const char *name, int (*func)(int, int, void *), void *arg);
    int sceKernelDelayThreadCB(SceUInt delay);
    int sceKernelSleepThread(void);
    int sceKernelWakeupThread(SceUID thid);
    int sceKernelChangeThreadPriority(SceUID thid, int priority);
    int sceKernelSuspendDispatchThread(void);
    int sceKernelResumeDispatchThread(int state);
    SceUID sceKernelCreateVTimer(const char *name, void *opt);
    int sceKernelStartVTimer(SceUID uid);
    int sceKernelSetVTimerHandlerWide(SceUID uid, long long time, SceUInt (*handler)(SceUID, void *, void *, void *), void *common);
    void func_eboot_08858FD0(Ge *);
    void func_eboot_08859094(Ge *);
    void func_eboot_0888040C(Sound *);
    void func_eboot_088B0AD4(Net *);
    void func_eboot_088BD038(MemoryStick *);
    void func_eboot_08899004(int);
    void func_eboot_0890B844(void *, u32);
    TaskBase *func_eboot_0885A98C(TaskBase *);
    void func_eboot_0889501C(TaskManager *);
    void func_eboot_0888FB00(System *);
    void func_eboot_08890130(System *);
    void func_eboot_0888FFB0(System *);
    int func_eboot_0888FD28(SceSize, void *);
    int D_eboot_0888FD68(int, int, void *);
    int func_eboot_08890014(SceSize, void *);
    void func_eboot_088900C4(System *);
    SceUInt func_eboot_088901CC(SceUID, void *, void *, System *);
    void *_overlay_group_addresses[];
    void sceKernelDcacheWritebackAll(void);
    void sceKernelIcacheInvalidateAll(void);
}

System *System::objectPtr;
SceUID D_eboot_08A5E274;

char D_eboot_089AA230[16] = "CheckExitGame";
char D_eboot_089AA240[8] = "exit";
char D_eboot_089AA248[16] = "priorityChanger";
char D_eboot_089AA258[20] = "ChangeThreadVTimer";

void System::run() {
    func_eboot_0888FB00(this);
    while (true) {
        func_eboot_08858FD0(Ge::objectPtr);
        Pad::objectPtr->update();
        SystemFont::objectPtr->clear();
        FileSys::objectPtr->draw_loading_screen();
        DataManager::objectPtr->update();
        DrawManager::objectPtr->initialize();
        func_eboot_0889501C(TaskManager::objectPtr);
        DrawManager::objectPtr->draw();
        DrawManager::objectPtr->clear();
        func_eboot_0888040C(Sound::objectPtr);
        SystemFont::objectPtr->draw();
        loopCount++;
    }
}

extern "C" void func_eboot_0888FB00(System *this_) {
    this_->userMainThreadId = sceKernelGetThreadId();
    this_->sha1ThreadStarted = false;
    this_->loopCount = 0;
    this_->languageId = 0;
    this_->unknown_0xF7A3A8 = 0;
    sceKernelChangeCurrentThreadAttr(0, 0x4000);
    func_eboot_08890130(this_);
    func_eboot_08899004(0);
    func_eboot_0888FFB0(this_);
    this_->checkExitGameThreadId = sceKernelCreateThread(D_eboot_089AA230, func_eboot_0888FD28, 0x6F, 0x1000, 0, 0);
    if (sceKernelStartThread(this_->checkExitGameThreadId, 0, 0) < 0) {
        sceKernelDeleteThread(this_->checkExitGameThreadId);
    }
    VramManager::objectPtr->method_08812A44();
    Ge::objectPtr->initialize();
    Pad::objectPtr->initialize();
    FileSys::objectPtr->initialize();
    ResourceManager::objectPtr->reset(0x83C000);
    DataManager::objectPtr->reset();
    func_eboot_08859094(Ge::objectPtr);
    FileSys::objectPtr->load_libfont(0x16, 2);
    sceUtilityLoadModule(0x100);
    sceUtilityLoadModule(0x101);
    DrawManager::objectPtr->reset();
    SystemFont::objectPtr->initialize();
    Camera::objectPtr->method_088137C8();
    func_eboot_088B0AD4(Net::objectPtr);
    TaskManager *taskManager = TaskManager::objectPtr;
    void *p = taskManager->cache.alloc(0x20, 0x10);
    if (p) {
        TaskBase *task = (TaskBase *)TaskBase::operator new(0x20, p);
        if (task) {
            task = func_eboot_0885A98C(task);
        }
        taskManager->active[0] = task;
        if (task) {
            task->overlay_group = 0;
            taskManager->active[0]->id = 1;
            TaskBase *active = taskManager->active[0];
            active->load_status = 2;
            active->load_delay = 0;
        }
    }
    func_eboot_088BD038(MemoryStick::objectPtr);
    DataManager::objectPtr->register_power_callbacks();
}

extern "C" int func_eboot_0888FD28(SceSize, void *) {
    D_eboot_08A5E274 = sceKernelCreateCallback(D_eboot_089AA240, D_eboot_0888FD68, 0);
    sceKernelRegisterExitCallback(D_eboot_08A5E274);
    while (true) {
        sceKernelDelayThreadCB(10000);
    }
}

extern "C" int D_eboot_0888FD68(int, int, void *) {
    sceKernelExitGame();
    return 0;
}

extern "C" void func_eboot_0888FD88(System *this_, u8 seed) {
    this_->rngState[0] = seed;
    this_->rngState[1] = seed;
}

extern "C" void func_eboot_0888FDA8() {
    System::objectPtr->rngState[2] = GameSys::objectPtr->userData.rngSeed;
}

u16 System::next_index(u32 type) {
    u32 v = rngState[type];
    if (v == 0) {
        v = 1;
    }
    rngState[type] = (v * 176) % 0xFF53;
    return rngState[type];
}

extern "C" u16 func_eboot_0888FE1C(System *this_) {
    u16 r = this_->next_index(2);
    GameSys::objectPtr->userData.rngSeed = r;
    return r;
}

// The original passes the unmasked int file_id to both find_pac and the virtual
// load_file_async(s32) (which does not mask it either). With the header's
// find_pac(u16) mwcc adds an andi at one of the two calls, so find_pac is called
// here through its exact symbol with an int parameter (the upstream "FUs" name,
// or the int/u16 split, is a guess; its body masks the value itself).
extern "C" s32 find_pac__11DataManagerFUs(DataManager *, int);

// Request an overlay load into _overlay_group_addresses[group]: copied from a
// cached pac when DataManager has one, else read from DATA.BIN asynchronously.
// func_eboot_0888FEE8 polls completion and func_eboot_0888FF00 links it.
extern "C" void func_eboot_0888FE4C(System *this_, int file_id, int group) {
    void *addr = _overlay_group_addresses[group];
    s32 slot = find_pac__11DataManagerFUs(DataManager::objectPtr, file_id);
    if (slot != -1) {
        DataManager::objectPtr->copy(addr, slot, -1);
        sceKernelDcacheWritebackAll();
    } else {
        FileSys::objectPtr->load_file_async(file_id, (u8 *)addr, -1, 0, 0, 1);
    }
}

extern "C" bool func_eboot_0888FEE8(System *) {
    return FileSys::objectPtr->is_loading();
}

extern "C" void func_eboot_0888FF00(System *this_, int file_id, int group) {
    void *addr = _overlay_group_addresses[group];
    func_eboot_0890B844(addr, FileSys::objectPtr->file_size(file_id));
    (&this_->activeOverlays.eboot)[group] = file_id;
    sceKernelIcacheInvalidateAll();
}

extern "C" bool func_eboot_0888FF8C(System *this_, int file_id, int group) {
    return (&this_->activeOverlays.eboot)[group] == (u16)file_id;
}

extern "C" void func_eboot_0888FFB0(System *this_) {
    this_->priorityChangerThreadId = sceKernelCreateThread(D_eboot_089AA248, func_eboot_08890014, 0x28, 0x200, 0, 0);
    sceKernelStartThread(this_->priorityChangerThreadId, sizeof(System *), &System::objectPtr);
}

extern "C" int func_eboot_08890014(SceSize, void *argp) {
    while (true) {
        if ((GameSys::objectPtr->flags_0x6AF14 & 0x200) != 0) {
            System *system = *(System **)argp;
            int dispatch = sceKernelSuspendDispatchThread();
            int intr = sceKernelCpuSuspendIntr();
            sceKernelChangeThreadPriority(system->userMainThreadId, 0x26);
            if (system->sha1ThreadStarted) {
                sceKernelChangeThreadPriority(system->sha1ThreadId, 0x27);
            }
            sceKernelCpuResumeIntr(intr);
            sceKernelResumeDispatchThread(dispatch);
        }
        sceKernelSleepThread();
    }
}

extern "C" void func_eboot_088900C4(System *this_) {
    sceKernelChangeThreadPriority(this_->userMainThreadId, 0x30);
    if (this_->sha1ThreadStarted) {
        sceKernelChangeThreadPriority(this_->sha1ThreadId, 0x31);
    }
    sceKernelWakeupThread(this_->priorityChangerThreadId);
}

extern "C" void func_eboot_08890130(System *this_) {
    this_->changeThreadVtimerId = sceKernelCreateVTimer(D_eboot_089AA258, 0);
}

extern "C" void func_eboot_0889016C(System *this_) {
    sceKernelSetVTimerHandlerWide(this_->changeThreadVtimerId, 0, (SceUInt (*)(SceUID, void *, void *, void *))func_eboot_088901CC, this_);
    sceKernelStartVTimer(this_->changeThreadVtimerId);
    this_->vtimerTickCount = 0;
}

extern "C" SceUInt func_eboot_088901CC(SceUID, void *, void *, System *this_) {
    func_eboot_088900C4(System::objectPtr);
    if (++this_->vtimerTickCount < 2) {
        return 16666;
    }
    return 0;
}

extern "C" void func_eboot_08890228(System *this_) {
    this_->wirelessIsOn |= 1;
    FileSys::objectPtr->unknown_0x2fa04 = 0;
}

extern "C" void func_eboot_0889025C(System *this_) {
    this_->wirelessIsOn &= ~1;
    FileSys::objectPtr->unknown_0x2fa04 = 1;
}
