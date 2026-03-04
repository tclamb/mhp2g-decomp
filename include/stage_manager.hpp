#pragma once

#include "common.h"

#include "cache.hpp"
#include "drawable.hpp"
#include "tagged_cache.hpp"
#include "base_stage.hpp"

struct pac_header;

struct prop_params {
    u16 words[4];
    float floats[4];
};

struct base_prop : drawable {
    typedef void (base_prop::*ptmf)(void);

    base_prop() {
        next = 0;
        prev = 0;
        prop_pmo = 0;
        prop_tmh = 0;
    }
    virtual ~base_prop();
    virtual void draw() = 0;
    virtual void vtable_0x10();
    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void call_ptmf();

    u32 unknown_0x4;
    u32 unknown_0x8;
    u32 unknown_0xC;
    base_prop *prev;
    base_prop *next;
    u8 unknown_0x18;
    u8 undefined_0x19[3];
    ptmf ptmf_0x1C;
    u8 undefined_0x28[4];
    pmo *prop_pmo;
    tmh *prop_tmh;
    prop_params *params;

    static void * operator new(u32 size, void *memory) {
        return memory;
    }

    static void operator delete(void *memory) {
        // do nothing
    }
};

struct stage_manager
{
    base_stage *stage;
    u8 unknown_0x4[640];
    u32 entrance_id;
    u16 stage_id;
    u16 unknown_0x28A;
    u8 slab[0xA000];
    cache cache;
    u8 unknown_0xA2A8[12];
    u32 unknown_0xA2B4;
    u32 unknown_0xA2B8;
    u8 *vram_start;
    u32 vram_transfer_size;
    u8 unknown_0xA2C4[4];
    u8 unknown_0xA2C8;
    u8 unknown_0xA2C9[7];
    ScePspFVector4 unknown_0xA2D0;
    base_prop *prop_list;
    pac_header *pac;
    u8 unknown_0xA2E8[0x100];
    u8 flag_0xA3E8;
    u8 unknown_0xA3E9;
    u8 unknown_0xA3EA[6];

    static stage_manager *INSTANCE;

    stage_manager();
    ~stage_manager();
    void reset();
    void unload();
    void call_stage_vtable_0x14();
    void call_0884ca28_with_stage();

    void call_prop_list_vtable_0x10();
    void call_prop_list_ptmf();
    void destroy_prop_list();

    void push_prop_089B927C(prop_params *params);
    void push_prop_089B92BC(prop_params *params, u32 arg3);
    void push_prop_089B92DC(prop_params *params, u32 arg3);
    void push_prop_089B92FC(prop_params *params);
    void push_prop_089B931C(prop_params *params, u32 arg3, u16 arg4, u16 arg5, u32 arg6, u16 arg7);
    void push_prop_089B933C(prop_params *params);
    void push_prop_089B935C(prop_params *params);
    void push_prop_089B937C(int pmo_index, prop_params *params, u32 arg3, u32 arg4, u8 arg5, u16 arg6, u16 arg7, u16 arg8, u32 arg9, u32 arg10);
    void push_prop_089B939C(prop_params *params, int pmo_index);
    void push_prop_089B93BC(prop_params *params);
    void push_prop_089B93DC(prop_params *params);
    void push_prop_089B93FC(prop_params *params);
    void push_prop_089B941C(prop_params *params, u32 arg3, u32 arg4, u32 arg5, u32 *arg6, u16 arg7, u32 arg8, u32 arg9, u32 arg10, u16 arg11);
    void push_prop_089B943C(prop_params *params);
    void push_prop_089B945C(prop_params *params);
    void push_prop_089B947C(prop_params *params);
    void push_prop_089B949C(prop_params *params);
    void push_prop_089B94BC(prop_params *params, u32 arg3);
    void push_prop_089B94DC(prop_params *params);
    void push_prop_089B94FC(prop_params *params, u32 *arg3, u32 arg4, u32 arg5, u8 arg6, u16 arg7, int pmo_index);
    void push_prop_089B951C(u8 arg2);
    void push_prop_089B953C(prop_params *params);
    void push_prop_089B955C();
    void push_prop_089B957C(prop_params *params);
    void push_prop_089B959C(prop_params *params);
    void push_prop_089B95BC(prop_params *params);
    void push_prop_089B95DC(prop_params *params);
    void push_prop_089B95FC(prop_params *params);
    void push_prop_089B961C(prop_params *params);
    void push_prop_089B963C(prop_params *params, u16 arg3, u16 arg4);
    void push_prop_089B965C(prop_params *params);
    void push_prop_089B6208(prop_params *params);
    void push_prop_089B967C(prop_params *params);
    void push_prop_089B969C(prop_params *params, u16 arg3, u32 *arg4, u32 *arg5);
    void push_prop_089B6258(prop_params *params, u32 arg3);
    void push_prop_089B96BC(prop_params *params);
    void push_prop_089B6278();
    void push_prop_089B6298(u8 arg2, prop_params *params, u16 arg4, u32 *arg5, u32 *arg6);
    void push_prop_089B62B8(prop_params *params);
    void push_prop_089B62D8(prop_params *params);
    void push_prop_089B62F8(u8 arg2, prop_params *params, u16 arg4, u16 arg5, u32 *arg6, u32 *arg7);
    void push_prop_089B6318(u8 arg2, u32 arg3, u16 arg4);
    void push_prop_089B6338(u8 arg2, u32 arg3, u8 arg4, u32 *arg5, u32 *arg6);
    void push_prop_089B6358(u32 arg2, u8 arg3, float arg4, u32 arg5);
    void push_prop_089B6358(u32 *arg2); // Matrix4?
    void push_prop_089B96DC();
    void push_prop_089B96FC(prop_params *params);
    void push_prop_089C41C8();
    void push_prop_089B971C(prop_params *params, int pmo_index);
    void push_prop_089B973C(prop_params *params);
    void push_prop_089B975C(prop_params *params);

    void stage_clear();
    void stage_destroy();
    u32 stage_unknown_0x444();
    u32 stage_unknown_0x444_thunk();
    u32 stage_vtable_0xA8();
    u16 farm_stage_file_id();
    void unknown_0xA2E8_clear();
    u32 register_sound(u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, ScePspFVector4 *arg7, u32 arg8);
    void register_lobby_sounds();
    void compile_pac(pac_header *pac, bool follow_camera);
    void vram_clear();
    u8 *vram_alloc(s32 size);
    void push(base_prop *prop);
    void free(base_prop *prop);
    u16 find_in_D_game_sub_09CDF678(int index, u16 key);
    void stage_vtable_0x2C();
    void stage_vtable_0x30();
    u8 get_flag_0xA3E8();
    u8 find_in_D_game_sub_09CDF678(u16 key);
    u16 get_in_D_game_sub_09CDF678(u8 i);

private:
    template<typename T>
    inline T *alloc_prop() {
        void *memory = cache.alloc(sizeof(T), 0x10);
        if (memory != 0) {
            memset(memory, 0, sizeof(T));
            void *prop = new (memory) T();
            if (!prop) {
                cache.free(memory);
            }
            return (T *)prop;
        }
        return 0;
    }

    template<typename T>
    inline T *alloc_and_push_prop() {
        T *prop = alloc_prop<T>();
        push(prop);
        return prop;
    }

    inline void link_model(base_prop *prop, int pmo_index) {
        if (pmo_index == 0) {
            base_stage *st = stage;
            prop->prop_pmo = &st->model_pmo;
            prop->prop_tmh = &st->model_tmh;
        } else if (pmo_index == 1) {
            base_stage *st = stage;
            prop->prop_pmo = &st->prop_pmo;
            prop->prop_tmh = &st->model_tmh;
        }
    }
};
