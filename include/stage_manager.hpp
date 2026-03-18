#pragma once

#include "common.h"

#include "cache.hpp"
#include "drawable.hpp"
#include "resource_manager.hpp"
#include "stage_base.hpp"
#include "pac.hpp"

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

struct prop_089B94DC_data {
    u16 mesh_index;
    u16 period;
    ScePspFVector4 position;
};

struct prop_089B975C_data {
    u8 mesh_index;
    ScePspFVector4 min_position;
    ScePspFVector4 spawn_box;
};

struct StageManager : Singleton<StageManager> {
    StageBase *stage;
    u8 unknown_0x4[640];
    u32 map_id;
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
    pac_header *stage_pac;
    u8 unknown_0xA2E8[0x100];
    u8 flag_0xA3E8;
    u8 unknown_0xA3E9;
    u8 unknown_0xA3EA[6];

    StageManager();
    ~StageManager();
    void reset();
    void unload();
    void call_stage_ptmf_0x3D8();
    void register_drawable();

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
    void push_prop_089B94DC(prop_089B94DC_data *data);
    void push_prop_089B94FC(u32, ScePspFVector4 *, u32, u32, u8, u16, int);
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
    void push_prop_089B969C(u32 bug_flags, s16 bug_mesh_id, ScePspFVector4 *spawn_center, ScePspFVector4 *spawn_box);
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
    void push_prop_089B975C(prop_089B975C_data *data);

    void stage_clear();
    void stage_destroy();
    u32 stage_unknown_0x444();
    u32 stage_unknown_0x444_thunk();
    bool stage_vtable_0xA8();
    u16 farm_stage_file_id();
    void unknown_0xA2E8_clear();
    u32 register_sound(u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, ScePspFVector4 *arg7, u32 arg8);
    void register_lobby_sounds();
    void compile_pac(pac_header *pac, bool load_all);
    void vram_clear();
    u8 *vram_alloc(s32 size);
    void push(base_prop *prop);
    void free(base_prop *prop);
    u16 find_map_stage_index(int map_id, u16 stage_id);
    stage_exit *stage_exits();
    s8 stage_exit_count();
    u8 get_flag_0xA3E8();
    u8 find_map_stage_index(u16 stage_id);
    u16 map_stage_id(u8 map_stage_index);
    stage_exit *intersecting_exit(ScePspFVector4 *position);
    u16 nearest_exit_destination_stage_id(u16 /* ignored */ stage_id, ScePspFVector4 *position);

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
            StageBase *st = stage;
            prop->prop_pmo = &st->model_pmo;
            prop->prop_tmh = &st->model_tmh;
        } else if (pmo_index == 1) {
            StageBase *st = stage;
            prop->prop_pmo = &st->prop_pmo;
            prop->prop_tmh = &st->model_tmh;
        }
    }

    template<pmo StageBase::*P>
    inline void compile_stage_pmo(pmo_header *header);

    // distance of point P to rectangle in the XZ plane with center (A+B)/2, width |AB|, and height |2t|
    float distance_point_rectangle(ScePspFVector4 *P, ScePspFVector4 *A, ScePspFVector4 *B, float t);
};
