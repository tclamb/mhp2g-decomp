#pragma once

// Minimal declarations for the original PSP ABI.
typedef unsigned char u8;
typedef unsigned int u32;
typedef short s16;

template <typename T> struct Singleton { static T *objectPtr; };
struct GameTask { u8 reserved[0x54]; u32 flags_54; };
struct Quest : Singleton<Quest> {};
struct Camera : Singleton<Camera> {};
struct LightManager : Singleton<LightManager> {};
struct ShellManager : Singleton<ShellManager> {};
struct StageBase {
    virtual void f0();
    virtual void f1();
    virtual void f2();
    virtual void f3();
    virtual void f4();
    virtual void f5();
    virtual void f6();
    virtual void vtable_0x24();
};
struct StageManager : Singleton<StageManager> { StageBase *stage; };

extern "C" void func_eboot_088714F0(Quest *);
extern "C" void method_088138DC__6CameraFv(Camera *);
extern "C" void method_088609FC__12LightManagerFs(LightManager *, s16);
extern "C" void emit_fog__9StageBaseFv(StageBase *);
extern "C" void func_eboot_0886D160(Quest *);
extern "C" void func_game_task_09B61E28(ShellManager *);
extern "C" void func_eboot_088718EC(Quest *);
extern "C" void destroy_prop_list__12StageManagerFv(StageManager *);
extern "C" void vram_clear__12StageManagerFv(StageManager *);

