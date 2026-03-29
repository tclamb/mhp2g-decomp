#include "stage_manager.hpp"

#include "camera.hpp"
#include "game_sys.hpp"
#include "hit_manager.hpp"
#include "sound.hpp"
#include "singleton.hpp"
#include "draw_manager.hpp"
#include "resource_manager.hpp"
#include "vram_manager.hpp"
#include "pac.hpp"
#include "vfpu.h"
#include "lb_event.hpp"
#include "hit_manager.hpp"

#pragma opt_unroll_loops on

#include "stage_table.inc.cpp"

StageManager global_stage_manager;
template<>
StageManager *Singleton<StageManager>::objectPtr;

StageManager::StageManager() {
    vram_start = NULL;
    vram_transfer_size = 0;
    unknown_0xA2B4 = 0;
    unknown_0xA2B8 = 0;
    unknown_0x28A = -1;
}

StageManager::~StageManager() {
    // empty
}

void StageManager::reset() {
    cache.reset(slab, sizeof(slab));
    unknown_0xA2B8 = 0;
    Singleton<VramManager>::objectPtr->method_08812F04(6, 0x4e200, VramManager::INVALID_ADRS);
    vram_start = Singleton<VramManager>::objectPtr->method_088133D0(6);
    vram_transfer_size = 0;
    unknown_0x28A = -1;
    unknown_0xA2C8 = 0;
    prop_list = 0;
    flag_0xA3E8 = 0;
    unknown_0xA3E9 = 1;
}

void StageManager::unload() {
    Singleton<ResourceManager>::objectPtr->free_all(1);
    unknown_0xA2B8 = 0;
    Singleton<VramManager>::objectPtr->method_08813024(6);
    vram_transfer_size = 0;
}

void StageManager::call_stage_ptmf_0x3D8() {
    if (stage != 0 && stage->model_pmo.header != 0) {
        stage->call_ptmf_0x3D8();
    }
}

void StageManager::register_drawable() {
    if (stage != 0 && stage->model_pmo.header != 0) {
        DrawManager::objectPtr->add(render_group::STAGE, stage, true);
    }
}

void StageManager::call_prop_list_vtable_0x10() {
    base_prop *prop = prop_list;
    while (prop != 0) {
        prop->vtable_0x10();
        prop = prop->next;
    }
    if (stage != 0 && unknown_0xA3E9 == true) {
        stage->flag_0x3D4 = true;
    }
}


void base_prop::vtable_0x10() {
    // empty
}

void StageManager::call_prop_list_ptmf() {
    base_prop *prop = prop_list;
    while (prop != 0) {
        prop->call_ptmf();
        base_prop *next = prop->next;
        bool alive = prop->flags & Draw::ALIVE;
        if (alive == false) {
            free(prop);
        }
        prop = next;
    }
}

void base_prop::call_ptmf() {
    if (ptmf_0x1C != 0) {
        (this->*ptmf_0x1C)();
    }
}


void StageManager::destroy_prop_list() {
    base_prop *prop = prop_list;
    while (prop != 0) {
        base_prop *next = prop->next;
        delete prop;
        cache.free(prop);
        prop = next;
    }
    prop_list = 0;
}

base_prop::~base_prop() {
    // empty
}

#define DECLARE_DUMMY_PROP(name, size, id)      \
    struct name : base_prop {                   \
        u8 padding[size - sizeof(base_prop)];   \
        inline name() {                         \
            unknown_0x18 = id;                  \
        }                                       \
        virtual void draw();                    \
        virtual void vtable_0x10();             \
        virtual void setup();                   \
        virtual void update();                  \
        virtual void vtable_0x1C();             \
    }
#define DECLARE_DUMMY_PROP_INIT_CONSTRUCTOR(name, size, id) \
    struct name : base_prop {                       \
        u8 padding[size - sizeof(base_prop)];       \
        name();                                     \
        virtual void draw();                        \
        virtual void vtable_0x10();                 \
        virtual void setup();                       \
        virtual void update();                      \
        virtual void vtable_0x1C();                 \
    }

DECLARE_DUMMY_PROP(prop_089B927C, 0x3C, 0);
DECLARE_DUMMY_PROP(prop_089B92BC, 0x50, 1);
DECLARE_DUMMY_PROP(prop_089B92DC, 0x60, 2);
DECLARE_DUMMY_PROP(prop_089B92FC, 0x50, 3);
DECLARE_DUMMY_PROP_INIT_CONSTRUCTOR(prop_089B931C, 0x70, 4);
DECLARE_DUMMY_PROP(prop_089B933C, 0x60, 5);
DECLARE_DUMMY_PROP(prop_089B935C, 0x60, 6);
DECLARE_DUMMY_PROP(prop_089B937C, 0x54, 7);
DECLARE_DUMMY_PROP(prop_089B939C, 0x60, 8);
DECLARE_DUMMY_PROP(prop_089B93BC, 0x70, 9);
DECLARE_DUMMY_PROP(prop_089B93DC, 0x50, 10);
DECLARE_DUMMY_PROP(prop_089B93FC, 0x60, 11);
DECLARE_DUMMY_PROP(prop_089B941C, 0x70, 12);
DECLARE_DUMMY_PROP(prop_089B943C, 0xA0, 13);
DECLARE_DUMMY_PROP(prop_089B945C, 0x60, 14);
DECLARE_DUMMY_PROP(prop_089B947C, 0x60, 15);
DECLARE_DUMMY_PROP(prop_089B949C, 0x3C, 16);
DECLARE_DUMMY_PROP(prop_089B94BC, 0x50, 17);
DECLARE_DUMMY_PROP(prop_089B94DC, 0x60, 18);
DECLARE_DUMMY_PROP(prop_089B94FC, 0x70, 19);
DECLARE_DUMMY_PROP(prop_089B951C, 0x50, 20);
DECLARE_DUMMY_PROP(prop_089B953C, 0xC0, 21);
DECLARE_DUMMY_PROP(prop_089B955C, 0x50, 22);
DECLARE_DUMMY_PROP(prop_089B957C, 0x70, 23);
DECLARE_DUMMY_PROP(prop_089B959C, 0x3C, 24);
DECLARE_DUMMY_PROP(prop_089B95BC, 0x70, 25);
DECLARE_DUMMY_PROP(prop_089B95DC, 0x60, 26);
DECLARE_DUMMY_PROP(prop_089B95FC, 0x60, 27);
DECLARE_DUMMY_PROP(prop_089B961C, 0x50, 28);
DECLARE_DUMMY_PROP(prop_089B963C, 0x60, 29);
DECLARE_DUMMY_PROP(prop_089B965C, 0x40, 30);
DECLARE_DUMMY_PROP(prop_089B6208, 0x3C, 31);
DECLARE_DUMMY_PROP(prop_089B967C, 0x60, 32);
DECLARE_DUMMY_PROP(prop_089B969C, 0x80, 33);
DECLARE_DUMMY_PROP(prop_089B6258, 0x70, 34);
DECLARE_DUMMY_PROP(prop_089B96BC, 0x70, 35);
DECLARE_DUMMY_PROP(prop_089B6278, 0x5A0, 36);
DECLARE_DUMMY_PROP(prop_089B6298, 0xE0, 37);
DECLARE_DUMMY_PROP(prop_089B62B8, 0x50, 38);
DECLARE_DUMMY_PROP(prop_089B62D8, 0x70, 39);
DECLARE_DUMMY_PROP(prop_089B62F8, 0x90, 40);
DECLARE_DUMMY_PROP(prop_089B6318, 0x90, 41);
DECLARE_DUMMY_PROP(prop_089B6338, 0xA0, 42);
DECLARE_DUMMY_PROP(prop_089B6358, 0xA0, 43);
DECLARE_DUMMY_PROP(prop_089B96DC, 0x50, 44);
DECLARE_DUMMY_PROP(prop_089B96FC, 0x50, 45);
DECLARE_DUMMY_PROP(prop_089C41C8, 0x50, 46);
DECLARE_DUMMY_PROP(prop_089B971C, 0x50, 47); // oops
DECLARE_DUMMY_PROP(prop_089B973C, 0x70, 48);
DECLARE_DUMMY_PROP(prop_089B975C, 0xA0, 49);


extern "C" void func_game_sub_09CB54D8(prop_089B927C *prop, prop_params *params);

void StageManager::push_prop_089B927C(prop_params *params) {
    prop_089B927C *prop = alloc_and_push_prop<prop_089B927C>();
    link_model(prop, true);
    func_game_sub_09CB54D8(prop, params);
}


extern "C" void func_game_sub_09CB5C20(prop_089B92BC *prop, prop_params *params, u32 arg3);

void StageManager::push_prop_089B92BC(prop_params *params, u32 arg3) {
    prop_089B92BC *prop = alloc_and_push_prop<prop_089B92BC>();
    link_model(prop, true);
    func_game_sub_09CB5C20(prop, params, arg3);
}


extern "C" void func_game_sub_09CB6468(prop_089B92DC *prop, prop_params *params, u32 arg3);

void StageManager::push_prop_089B92DC(prop_params *params, u32 arg3) {
    prop_089B92DC *prop = alloc_and_push_prop<prop_089B92DC>();
    link_model(prop, true);
    func_game_sub_09CB6468(prop, params, arg3);
}


extern "C" void func_game_sub_09CB6968(prop_089B92FC *prop, prop_params *params);

void StageManager::push_prop_089B92FC(prop_params *params) {
    prop_089B92FC *prop = alloc_and_push_prop<prop_089B92FC>();
    link_model(prop, true);
    func_game_sub_09CB6968(prop, params);
}


extern "C" void func_game_sub_09CB7008(prop_089B931C *prop, prop_params *params, u32 arg3, u16 arg4, u16 arg5, u32 arg6, u16 arg7);

void StageManager::push_prop_089B931C(prop_params *params, u32 arg3, u16 arg4, u16 arg5, u32 arg6, u16 arg7) {
    prop_089B931C *prop = alloc_prop<prop_089B931C>();
    push(prop);
    link_model(prop, true);
    func_game_sub_09CB7008(prop, params, arg3, arg4, arg5, arg6, arg7);
}


extern "C" void func_game_sub_09CB76E0(prop_089B933C *prop, prop_params *params);

void StageManager::push_prop_089B933C(prop_params *params) {
    prop_089B933C *prop = alloc_and_push_prop<prop_089B933C>();
    link_model(prop, true);
    func_game_sub_09CB76E0(prop, params);
}


extern "C" void func_game_sub_09CB8110(prop_089B935C *prop, prop_params *params);

void StageManager::push_prop_089B935C(prop_params *params) {
    prop_089B935C *prop = alloc_and_push_prop<prop_089B935C>();
    link_model(prop, true);
    func_game_sub_09CB8110(prop, params);
}


extern "C" void func_game_sub_09CB8DF0(prop_089B937C *prop, prop_params *params, u32 arg3, u32 arg4, u8 arg5, u16 arg6, u16 arg7, u16 arg8, u32 arg9, u32 arg10);

void StageManager::push_prop_089B937C(int pmo_index, prop_params *params, u32 arg3, u32 arg4, u8 arg5, u16 arg6, u16 arg7, u16 arg8, u32 arg9, u32 arg10) {
    prop_089B937C *prop = alloc_and_push_prop<prop_089B937C>();
    link_model(prop, pmo_index);
    func_game_sub_09CB8DF0(prop, params, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
}


extern "C" void func_game_sub_09CB97E8(prop_089B939C *prop, prop_params *params);

void StageManager::push_prop_089B939C(prop_params *params, int pmo_index) {
    prop_089B939C *prop = alloc_and_push_prop<prop_089B939C>();
    link_model(prop, pmo_index);
    func_game_sub_09CB97E8(prop, params);
}


extern "C" void func_game_sub_09CB9FD8(prop_089B93BC *prop, prop_params *params);

void StageManager::push_prop_089B93BC(prop_params *params) {
    prop_089B93BC *prop = alloc_and_push_prop<prop_089B93BC>();
    link_model(prop, true);
    func_game_sub_09CB9FD8(prop, params);
}


extern "C" void func_game_sub_09CBA820(prop_089B93DC *prop, prop_params *params);

void StageManager::push_prop_089B93DC(prop_params *params) {
    prop_089B93DC *prop = alloc_and_push_prop<prop_089B93DC>();
    link_model(prop, true);
    func_game_sub_09CBA820(prop, params);
}


extern "C" void func_game_sub_09CBACF0(prop_089B93FC *prop, prop_params *params);

void StageManager::push_prop_089B93FC(prop_params *params) {
    prop_089B93FC *prop = alloc_and_push_prop<prop_089B93FC>();
    link_model(prop, true);
    func_game_sub_09CBACF0(prop, params);
}


extern "C" void func_game_sub_09CBB398(prop_089B941C *prop, prop_params *params, u32 arg3, u32 arg4, u32 arg5, u32 *arg6, u16 arg7, u32 arg8, u32 arg9, u32 arg10, u16 arg11);

void StageManager::push_prop_089B941C(prop_params *params, u32 arg3, u32 arg4, u32 arg5, u32 *arg6, u16 arg7, u32 arg8, u32 arg9, u32 arg10, u16 arg11) {
    prop_089B941C *prop = alloc_and_push_prop<prop_089B941C>();
    func_game_sub_09CBB398(prop, params, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
}


extern "C" void func_game_sub_09CBBB48(prop_089B943C *prop, prop_params *params);

void StageManager::push_prop_089B943C(prop_params *params) {
    prop_089B943C *prop = alloc_and_push_prop<prop_089B943C>();
    link_model(prop, true);
    func_game_sub_09CBBB48(prop, params);
}


extern "C" void func_game_sub_09CBD1F0(prop_089B945C *prop, prop_params *params);

void StageManager::push_prop_089B945C(prop_params *params) {
    prop_089B945C *prop = alloc_and_push_prop<prop_089B945C>();
    link_model(prop, true);
    func_game_sub_09CBD1F0(prop, params);
}


extern "C" void func_game_sub_09CBD950(prop_089B947C *prop, prop_params *params);

void StageManager::push_prop_089B947C(prop_params *params) {
    prop_089B947C *prop = alloc_and_push_prop<prop_089B947C>();
    link_model(prop, true);
    func_game_sub_09CBD950(prop, params);
}


extern "C" void func_game_sub_09CBE478(prop_089B949C *prop, prop_params *params);

void StageManager::push_prop_089B949C(prop_params *params) {
    prop_089B949C *prop = alloc_and_push_prop<prop_089B949C>();
    link_model(prop, false);
    func_game_sub_09CBE478(prop, params);
}


extern "C" void func_game_sub_09CBE7F8(prop_089B94BC *prop, prop_params *params, u32 arg3);

void StageManager::push_prop_089B94BC(prop_params *params, u32 arg3) {
    prop_089B94BC *prop = alloc_and_push_prop<prop_089B94BC>();
    link_model(prop, true);
    func_game_sub_09CBE7F8(prop, params, arg3);
}


extern "C" void func_game_sub_09CBECB8(prop_089B94DC *prop, prop_089B94DC_data *params);

void StageManager::push_prop_089B94DC(prop_089B94DC_data *data) {
    prop_089B94DC *prop = alloc_and_push_prop<prop_089B94DC>();
    link_model(prop, true);
    func_game_sub_09CBECB8(prop, data);
}


extern "C" void func_game_sub_09CBF188(prop_089B94FC *prop, u32 flags, ScePspFVector4 *position, u32 arg4, u32 arg5, u8 arg6, u16 arg7);

void StageManager::push_prop_089B94FC(u32 flags, ScePspFVector4 *position, u32 arg4, u32 arg5, u8 arg6, u16 arg7, int pmo_index) {
    prop_089B94FC *prop = alloc_and_push_prop<prop_089B94FC>();
    link_model(prop, pmo_index);
    func_game_sub_09CBF188(prop, flags, position, arg4, arg5, arg6, arg7);
}


extern "C" void func_game_sub_09CBFDF0(prop_089B951C *prop, u8 arg2);

void StageManager::push_prop_089B951C(u8 arg2) {
    prop_089B951C *prop = alloc_and_push_prop<prop_089B951C>();
    link_model(prop, true);
    func_game_sub_09CBFDF0(prop, arg2);
}


extern "C" void func_game_sub_09CC0890(prop_089B953C *prop, prop_params *params);

void StageManager::push_prop_089B953C(prop_params *params) {
    prop_089B953C *prop = alloc_and_push_prop<prop_089B953C>();
    link_model(prop, true);
    func_game_sub_09CC0890(prop, params);
}


extern "C" void func_game_sub_09CC11F0(prop_089B955C *prop);

void StageManager::push_prop_089B955C() {
    prop_089B955C *prop = alloc_and_push_prop<prop_089B955C>();
    link_model(prop, true);
    func_game_sub_09CC11F0(prop);
}


extern "C" void func_game_sub_09CC1A90(prop_089B957C *prop, prop_params *params);

void StageManager::push_prop_089B957C(prop_params *params) {
    prop_089B957C *prop = alloc_and_push_prop<prop_089B957C>();
    link_model(prop, true);
    func_game_sub_09CC1A90(prop, params);
}


extern "C" void func_game_sub_09CC2370(prop_089B959C *prop, prop_params *params);

void StageManager::push_prop_089B959C(prop_params *params) {
    prop_089B959C *prop = alloc_and_push_prop<prop_089B959C>();
    link_model(prop, true);
    func_game_sub_09CC2370(prop, params);
}


extern "C" void func_game_sub_09CC2B88(prop_089B95BC *prop, prop_params *params);

void StageManager::push_prop_089B95BC(prop_params *params) {
    prop_089B95BC *prop = alloc_and_push_prop<prop_089B95BC>();
    link_model(prop, true);
    func_game_sub_09CC2B88(prop, params);
}


extern "C" void func_game_sub_09CC33F8(prop_089B95DC *prop, prop_params *params);

void StageManager::push_prop_089B95DC(prop_params *params) {
    prop_089B95DC *prop = alloc_and_push_prop<prop_089B95DC>();
    link_model(prop, true);
    func_game_sub_09CC33F8(prop, params);
}


extern "C" void func_game_sub_09CC4130(prop_089B95FC *prop, prop_params *params);

void StageManager::push_prop_089B95FC(prop_params *params) {
    prop_089B95FC *prop = alloc_and_push_prop<prop_089B95FC>();
    link_model(prop, true);
    func_game_sub_09CC4130(prop, params);
}


extern "C" void func_game_sub_09CC4AA0(prop_089B961C *prop, prop_params *params);

void StageManager::push_prop_089B961C(prop_params *params) {
    prop_089B961C *prop = alloc_and_push_prop<prop_089B961C>();
    link_model(prop, true);
    func_game_sub_09CC4AA0(prop, params);
}


extern "C" void func_game_sub_09CC5088(prop_089B963C *prop, prop_params *params, u16 arg3, u16 arg4);

void StageManager::push_prop_089B963C(prop_params *params, u16 arg3, u16 arg4) {
    prop_089B963C *prop = alloc_and_push_prop<prop_089B963C>();
    link_model(prop, true);
    func_game_sub_09CC5088(prop, params, arg3, arg4);
}


extern "C" void func_game_sub_09CC5530(prop_089B965C *prop, prop_params *params);

void StageManager::push_prop_089B965C(prop_params *params) {
    prop_089B965C *prop = alloc_and_push_prop<prop_089B965C>();
    link_model(prop, true);
    func_game_sub_09CC5530(prop, params);
}


extern "C" void func_lobby_task_09AF2C38(prop_089B6208 *prop, prop_params *params);

void StageManager::push_prop_089B6208(prop_params *params) {
    prop_089B6208 *prop = alloc_and_push_prop<prop_089B6208>();
    link_model(prop, true);
    func_lobby_task_09AF2C38(prop, params);
}


extern "C" void func_game_sub_09CC5D10(prop_089B967C *prop, prop_params *params);

void StageManager::push_prop_089B967C(prop_params *params) {
    prop_089B967C *prop = alloc_and_push_prop<prop_089B967C>();
    link_model(prop, true);
    func_game_sub_09CC5D10(prop, params);
}


extern "C" void func_game_sub_09CC6950(prop_089B969C *, u32, s16, ScePspFVector4 *, ScePspFVector4 *);

void StageManager::push_prop_089B969C(u32 bug_flags, s16 bug_mesh_index, ScePspFVector4 *spawn_center, ScePspFVector4 *spawn_box) {
    prop_089B969C *prop = alloc_and_push_prop<prop_089B969C>();
    link_model(prop, true);
    func_game_sub_09CC6950(prop, bug_flags, bug_mesh_index, spawn_center, spawn_box);
}


extern "C" void func_lobby_task_09AF3160(prop_089B6258 *prop, prop_params *params, u32 arg3);

void StageManager::push_prop_089B6258(prop_params *params, u32 arg3) {
    prop_089B6258 *prop = alloc_and_push_prop<prop_089B6258>();
    link_model(prop, true);
    func_lobby_task_09AF3160(prop, params, arg3);
}


extern "C" void func_game_sub_09CC7180(prop_089B96BC *prop, prop_params *params);

void StageManager::push_prop_089B96BC(prop_params *params) {
    prop_089B96BC *prop = alloc_and_push_prop<prop_089B96BC>();
    link_model(prop, true);
    func_game_sub_09CC7180(prop, params);
}


extern "C" void func_lobby_task_09AF3F58(prop_089B6278 *prop);

void StageManager::push_prop_089B6278() {
    prop_089B6278 *prop = alloc_and_push_prop<prop_089B6278>();
    link_model(prop, true);
    func_lobby_task_09AF3F58(prop);
}


extern "C" void func_lobby_task_09AF5580(prop_089B6298 *prop, u8 arg2, prop_params *params, u16 arg4, u32 *arg5, u32 *arg6);

void StageManager::push_prop_089B6298(u8 arg2, prop_params *params, u16 arg4, u32 *arg5, u32 *arg6) {
    prop_089B6298 *prop = alloc_and_push_prop<prop_089B6298>();
    link_model(prop, true);
    func_lobby_task_09AF5580(prop, arg2, params, arg4, arg5, arg6);
}


extern "C" void func_lobby_task_09AF6B20(prop_089B62B8 *prop, prop_params *params);

void StageManager::push_prop_089B62B8(prop_params *params) {
    prop_089B62B8 *prop = alloc_and_push_prop<prop_089B62B8>();
    link_model(prop, true);
    func_lobby_task_09AF6B20(prop, params);
}


extern "C" void func_lobby_task_09AF7260(prop_089B62D8 *prop, prop_params *params);

void StageManager::push_prop_089B62D8(prop_params *params) {
    prop_089B62D8 *prop = alloc_and_push_prop<prop_089B62D8>();
    link_model(prop, true);
    func_lobby_task_09AF7260(prop, params);
}


extern "C" void func_lobby_task_09AF78B8(prop_089B62F8 *prop, u8 arg2, prop_params *params, u16 arg4, u16 arg5, u32 *arg6, u32 *arg7);

void StageManager::push_prop_089B62F8(u8 arg2, prop_params *params, u16 arg4, u16 arg5, u32 *arg6, u32 *arg7) {
    prop_089B62F8 *prop = alloc_and_push_prop<prop_089B62F8>();
    link_model(prop, true);
    func_lobby_task_09AF78B8(prop, arg2, params, arg4, arg5, arg6, arg7);
}


extern "C" void func_lobby_task_09AF8C18(prop_089B6318 *prop, u8 arg2, u32 arg3, u16 arg4);

void StageManager::push_prop_089B6318(u8 arg2, u32 arg3, u16 arg4) {
    prop_089B6318 *prop = alloc_and_push_prop<prop_089B6318>();
    link_model(prop, true);
    func_lobby_task_09AF8C18(prop, arg2, arg3, arg4);
}


extern "C" void func_lobby_task_09AF9B38(prop_089B6338 *prop, u8 arg2, u32 arg3, u8 arg4, u32 *arg5, u32 *arg6);

void StageManager::push_prop_089B6338(u8 arg2, u32 arg3, u8 arg4, u32 *arg5, u32 *arg6) {
    prop_089B6338 *prop = alloc_and_push_prop<prop_089B6338>();
    link_model(prop, true);
    func_lobby_task_09AF9B38(prop, arg2, arg3, arg4, arg5, arg6);
}


extern "C" void func_lobby_task_09AFB708(prop_089B6358 *prop, u32 arg2, u8 arg3, float arg4, u32 arg5);

void StageManager::push_prop_089B6358(u32 arg2, u8 arg3, float arg4, u32 arg5) {
    prop_089B6358 *prop = alloc_and_push_prop<prop_089B6358>();
    link_model(prop, true);
    func_lobby_task_09AFB708(prop, arg2, arg3, arg4, arg5);
}


extern "C" void func_lobby_task_09AFB7C8(prop_089B6358 *prop, u32 *arg2);

void StageManager::push_prop_089B6358(u32 *arg2) {
    prop_089B6358 *prop = alloc_and_push_prop<prop_089B6358>();
    link_model(prop, true);
    func_lobby_task_09AFB7C8(prop, arg2);
}


extern "C" void func_game_sub_09CC7DD8(prop_089B96DC *prop);

void StageManager::push_prop_089B96DC() {
    prop_089B96DC *prop = alloc_and_push_prop<prop_089B96DC>();
    link_model(prop, true);
    func_game_sub_09CC7DD8(prop);
}


extern "C" void func_game_sub_09CC8378(prop_089B96FC *prop, prop_params *params);

void StageManager::push_prop_089B96FC(prop_params *params) {
    prop_089B96FC *prop = alloc_and_push_prop<prop_089B96FC>();
    link_model(prop, true);
    func_game_sub_09CC8378(prop, params);
}


extern "C" void func_stage210_09D5E510(prop_089C41C8 *prop);

void StageManager::push_prop_089C41C8() {
    prop_089C41C8 *prop = alloc_and_push_prop<prop_089C41C8>();
    link_model(prop, true);
    func_stage210_09D5E510(prop);
}


extern "C" void func_game_sub_09CC8730(prop_089B971C *prop, prop_params *params);

void StageManager::push_prop_089B971C(prop_params *params, int pmo_index) {
    prop_089B971C *prop = alloc_and_push_prop<prop_089B971C>();
    link_model(prop, pmo_index);
    func_game_sub_09CC8730(prop, params);
}


extern "C" void func_game_sub_09CC8C60(prop_089B973C *prop, prop_params *params);

void StageManager::push_prop_089B973C(prop_params *params) {
    prop_089B973C *prop = alloc_and_push_prop<prop_089B973C>();
    link_model(prop, true);
    func_game_sub_09CC8C60(prop, params);
}


extern "C" void func_game_sub_09CC9850(prop_089B975C *prop, prop_089B975C_data *data);

void StageManager::push_prop_089B975C(prop_089B975C_data *data) {
    prop_089B975C *prop = alloc_and_push_prop<prop_089B975C>();
    link_model(prop, true);
    func_game_sub_09CC9850(prop, data);
}


void StageManager::stage_clear() {
    stage = STAGE_TABLE[stage_id];
    stage->clear();
    unknown_0xA2E8_clear();
}

void StageManager::stage_destroy() {
    stage->destroy();
    stage = 0;
}

u32 StageManager::stage_unknown_0x444() {
    return stage->unknown_0x444;
}

u32 StageManager::stage_unknown_0x444_thunk() {
    return stage_unknown_0x444();
}

bool StageManager::stage_vtable_0xA8() {
    return stage->vtable_0xA8();
}

// inferred from 0x1787 corresponding to the first farm variant stage pac
u16 StageManager::farm_stage_file_id() {
    FarmState &farm = Singleton<GameSys>::objectPtr->farm;
    u16 offset = farm.vars[1];
    if ((farm.vars[6] & 1) != 0) {
        offset = 3;
    }
    if (farm.vars[2] < 2) {
        offset += 4;
    }
    if (func_game_sub_09C14800(Singleton<LbEvent>::objectPtr, 0x5F) == 0) {
        offset += 8;
    }
    return offset + 0x1787;
}

void StageManager::unknown_0xA2E8_clear() {
    for (int i = 0; i < 0x100; ++i) {
        unknown_0xA2E8[i] = 0;
    }
}

u32 StageManager::register_sound(u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, ScePspFVector4 *arg7, u32 arg8) {
    u32 result = 0;
    if (func_eboot_08883CF4(Singleton<Sound>::objectPtr, arg7, arg8) == 0) {
        if (unknown_0xA2E8[arg6] != 0) {
            unknown_0xA2E8[arg6] = 0;
            func_eboot_08885198(Singleton<Sound>::objectPtr, arg2, arg5, arg6, false);
        } else {
            result = result;
        }
    } else {
        if (unknown_0xA2E8[arg6] == 0) {
            result = func_eboot_08883858(Singleton<Sound>::objectPtr, arg2, arg3, arg4, arg5, arg6, arg7, arg8, 0, 0, false);
            unknown_0xA2E8[arg6] = 1;
        } else {
            func_eboot_0888444C(Singleton<Sound>::objectPtr, arg2, arg4, arg5, arg6, arg7, arg8, 0, 0);
        }
    }
    return result;
}

void StageManager::register_lobby_sounds() {
    int index;
    stage_sound *sound = stage->definitions()->sounds;
    if (sound != 0) {
        for (index = 0; index < stage->definitions()->sound_count; ++index, ++sound) {
            if (sound->unknown_0x0 == 0) {
                StageManager::objectPtr->register_sound(sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, index + 1, &sound->position, sound->unknown_0xC);
            }
        }
    }
}

extern "C" {
    int sceDmacMemcpy(void *, const void *, u32);
    void sceKernelDcacheWritebackInvalidateAll();
}

inline u32 header_mesh_data_size(pmo_header *header) {
    return header->mesh_data_size();
}

void StageManager::compile_pac(pac_header *pac, bool load_all) {
    cache.reset(slab, sizeof(slab));
    vram_transfer_size = 0;

    stage_pac = pac;
    pmo_header *model_header = (pmo_header *)pac->data(0);
    {
        u32 mesh_count = model_header->mesh_count;
        u32 material_count = model_header->material_count();
        void *buffer = cache.alloc(material_count * sizeof(pmo_material_data) + mesh_count * sizeof(pmo_mesh_lighting), 0x10);

        u32 mesh_data_size = header_mesh_data_size(model_header);
        pmo_mesh_data *vram_block = (pmo_mesh_data *)vram_alloc(mesh_data_size);

        sceKernelDcacheWritebackInvalidateAll();
        pmo_mesh_data *mesh_data = model_header->mesh_data();
        sceDmacMemcpy(vram_block, mesh_data, mesh_data_size);

        stage->model_pmo.compile(buffer, model_header, vram_block);
    }

    tmh_header *tmh = (tmh_header *)pac->data(1);
    if (tmh != 0) {
        u32 *texture_buffer = (u32 *)cache.alloc(tmh->picture_count * 0x20, 0x10);
        stage->model_tmh.compile(texture_buffer, tmh, 0);
    }

    stage_environment_params *environment = (stage_environment_params *)pac->data(3);
    if (environment != 0) {
        stage->compile_environment_params(environment);
    }

    pmo_header *prop_header = (pmo_header *)pac->data(2);
    if (prop_header != 0) {
        u32 mesh_count = prop_header->mesh_count;
        u32 material_count = prop_header->material_count();
        void *buffer = cache.alloc(material_count * sizeof(pmo_material_data) + mesh_count * sizeof(pmo_mesh_lighting), 0x10);

        u32 mesh_data_size = prop_header->mesh_data_size();
        pmo_mesh_data *vram_block = (pmo_mesh_data *)vram_alloc(mesh_data_size);

        sceKernelDcacheWritebackInvalidateAll();
        pmo_mesh_data *mesh_data = prop_header->mesh_data();
        sceDmacMemcpy(vram_block, mesh_data, mesh_data_size);

        stage->prop_pmo.compile(buffer, prop_header, vram_block);
    }

    if (load_all == true) {
        void *unknown_data_4 = pac->data(4);
        func_eboot_088157D4(Singleton<Camera>::objectPtr, unknown_data_4);

        void *unknown_data_5 = pac->data(5);
        if (unknown_data_5 != 0) {
            func_game_sub_09C336D8(Singleton<HitManager>::objectPtr, unknown_data_5);
        }
    }

    flag_0xA3E8 = true;
}

void StageManager::vram_clear() {
    cache.reset(slab, sizeof(slab));
    vram_transfer_size = 0;
    unknown_0xA2B8 = 0;
    if (stage != 0) {
        stage->flags &= ~Draw::VISIBLE;
    }
    flag_0xA3E8 = false;
}

u8 *StageManager::vram_alloc(s32 size) {
    u32 misalignment = size & 0xf;
    if ((size < 0) && misalignment != 0) {
        misalignment -= 0x10;
    }
    if (misalignment != 0) {
        size += 0x10 - misalignment;
    }
    u8 *block = (u8 *)vram_start + vram_transfer_size;
    vram_transfer_size += size;
    return block;
}

void StageManager::push(base_prop *prop) {
    if (!prop_list) {
        prop_list = prop;
        prop->prev = 0;
        prop->next = 0;
    } else {
        prop_list->prev = prop;
        prop->next = prop_list;
        prop->prev = 0;
        prop_list = prop;
    }
}

void StageManager::free(base_prop *prop) {
    base_prop *next = prop->next;
    if (prop_list == prop) {
        prop_list = next;
        if (next != 0) {
            next->prev = 0;
        }
    } else {
        if (next == 0) {
            prop->prev->next = next;
        } else {
            prop->prev->next = next;
            prop->next->prev = prop->prev;
        }
    }
    delete prop;
    cache.free(prop);
}

extern "C" {
    struct MapStageIds {
        s32 count;
        u16 *stage_ids;
    };
    extern MapStageIds D_game_sub_09CDF678[0x20];
}

u16 StageManager::find_map_stage_index(int map_id, u16 stage_id) {
    for (int i = 0; i < D_game_sub_09CDF678[map_id].count; ++i) {
        if (stage_id == D_game_sub_09CDF678[map_id].stage_ids[i]) {
            return i;
        }
    }
    return -1;
}

stage_exit *StageManager::stage_exits() {
    return stage->exits(map_id);
}

s8 StageManager::stage_exit_count() {
    return stage->exit_count(map_id);
}

// test if position is in rectangular prism with center bottom left/right points a/b
extern "C"
int func_game_sub_09C31748(ScePspFVector4 *position,  ScePspFVector4 *a, ScePspFVector4 *b, float height, float half_thickness);

stage_exit *StageManager::intersecting_exit(ScePspFVector4 *position) {
    stage_exit *exit;
    if ((exit = stage_exits()) == 0) {
        return 0;
    }
    for (s8 i = 0; i < stage_exit_count(); ++i, ++exit) {
        if (position->y >= exit->p.y && position->y < exit->p.y + exit->height) {
            switch (exit->shape) {
            case 0: {
                float dx = position->x - exit->p.x;
                float dz = position->z - exit->p.z;
                if (dx * dx + dz * dz <= exit->size * exit->size) {
                    return exit;
                }
                break;
            }
            case 1:
                ScePspFVector4 p, q;
                p.x = exit->p.x;    p.y = exit->p.y;    p.z = exit->p.z;    p.w = 0;
                q.x = exit->q.x;    q.y = exit->q.y;    q.z = exit->q.z;    q.w = 0;
                if (func_game_sub_09C31748(position, &p, &q, exit->height, exit->size)) {
                    return exit;
                }
                break;
            }
        }
    }
    return 0;
}

u16 StageManager::nearest_exit_destination_stage_id(u16 ignored, ScePspFVector4 *position) {
    stage_exit *exit = stage_exits();
    if (exit == 0) {
        return D_game_sub_09CDF678[map_id].stage_ids[0];
    }
    float min = 1.0e10f;
    u16 result = 0;
    for (s8 i = 0; i < stage_exit_count(); ++i, ++exit) {
        float distance;
        if (exit->shape == 0) {
            float dx = exit->p.x;
            float dz = exit->p.z;
            dx -= position->x;
            dz -= position->z;
            float r2 = dx * dx + dz * dz;
            distance = vsqrt_s(r2) - exit->size;
        } else {
            ScePspFVector4 p, q, v;
            p.x = exit->p.x;    p.y = 0;    p.z = exit->p.z;    p.w = 1;
            q.x = exit->q.x;    q.y = 0;    q.z = exit->q.z;    q.w = 1;
            v.x = position->x;  v.y = 0;    v.z = position->z;  v.w = 1;
            distance = distance_point_rectangle(&v, &p, &q, exit->size);
        }
        if (min > distance) {
            result = exit->destination_stage_id;
            min = distance;
        }
    }
    return result;
}

// out <- closest point on line segment a-b to point p
// returns distance from p to out
extern "C"
float func_game_sub_09C30780(ScePspFVector4 *out, ScePspFVector4 *a, ScePspFVector4 *b, ScePspFVector4 *p);

// this is specifically the distance between a point and the boundary of a rectangle
float StageManager::distance_point_rectangle(ScePspFVector4 *v, ScePspFVector4 *p, ScePspFVector4 *q, float t) {
    ScePspFVector4 ignored;
    ScePspFVector4 direction;
    ScePspFVector4 perpendicular;
    ScePspFVector4 q_minus;
    ScePspFVector4 q_plus;
    ScePspFVector4 p_minus;
    ScePspFVector4 p_plus;

    vsub_q(&direction, p, q);
    normalize(&direction, &direction);

    perpendicular.x = -direction.z;
    perpendicular.y = direction.y;
    perpendicular.z = direction.x;
    perpendicular.w = 0;
    vscl_q(&perpendicular, &perpendicular, t);

    vadd_q(&p_plus,  p, &perpendicular);
    vsub_q(&p_minus, p, &perpendicular);
    vadd_q(&q_plus,  q, &perpendicular);
    vsub_q(&q_minus, q, &perpendicular);

    float distance[4];
    distance[0] = func_game_sub_09C30780(&ignored, &p_plus,  &p_minus, v);
    distance[1] = func_game_sub_09C30780(&ignored, &q_plus,  &q_minus, v);
    distance[2] = func_game_sub_09C30780(&ignored, &p_plus,  &q_plus,  v);
    distance[3] = func_game_sub_09C30780(&ignored, &p_minus, &q_minus, v);

    for (int i = 1; i <= 4 - 1; ++i) {
        if (distance[0] > distance[i]) {
            distance[0] = distance[i];
        }
    }
    return distance[0];
}

u8 StageManager::get_flag_0xA3E8() {
    return flag_0xA3E8;
}

u8 StageManager::find_map_stage_index(u16 stage_id) {
    int mapId = Singleton<GameSys>::objectPtr->mapId;
    MapStageIds &m = D_game_sub_09CDF678[mapId];
    for (int i = 0; i < m.count; ++i) {
        if (stage_id == m.stage_ids[i]) {
            return i;
        }
    }
    return 0;
}

u16 StageManager::map_stage_id(u8 map_stage_index) {
    int mapId = Singleton<GameSys>::objectPtr->mapId;
    MapStageIds &m = D_game_sub_09CDF678[mapId];
    return m.stage_ids[map_stage_index];
}
