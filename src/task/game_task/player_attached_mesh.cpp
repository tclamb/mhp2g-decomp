

#include "obj_base.hpp"
#include "singleton.hpp"
#include "game_sys.hpp"
#include "immediate_ge.hpp"
#include "vfpu.h"
using namespace immediate_ge;

struct EffectManager;
struct AttachedEffectSelection {
    u8 unknown_00[2];
    u8 preset;
    u8 color;
    u16 flags;
};
struct AttachedEffectPreset {
    s16 mesh;
    u16 unknown_02;
    ScePspUnion32 texoffset_v;
    u8 unknown_08[8];
    ScePspFVector4 arg_10;
    ScePspFVector4 arg_20;
    ScePspFVector4 arg_30;
};
extern "C" AttachedEffectSelection *func_game_sub_09C2F3A0(EffectManager *, int);
extern "C" void func_eboot_0886503C(ObjBase *, ScePspFMatrix4 *, int, ScePspFVector4 *, ScePspFVector4 *, ScePspFVector4 *);
extern "C" AttachedEffectPreset D_game_sub_09CCE5A0[];
extern "C" float D_game_sub_09CCE570[];

extern "C" void func_game_task_09A93D10(ObjBase *player, int effect_id) {
    struct { u32 unused; ScePspUnion32 rgba; } color;
    ScePspFMatrix4 transform;
    if (player->pl_action_ck(5, 9) != false && *(u16 *)((u8 *)player + 0x324) == 0x33 && *(s8 *)((u8 *)player + 0xBE) != 0) {
        return;
    }
    AttachedEffectSelection *selection = func_game_sub_09C2F3A0(Singleton<EffectManager>::objectPtr, effect_id);
    AttachedEffectPreset *preset = &D_game_sub_09CCE5A0[selection->preset];
    pmo *model = (pmo *)((u8 *)Singleton<EffectManager>::objectPtr + 0x19040);
    tmh *textures = (tmh *)((u8 *)Singleton<EffectManager>::objectPtr + 0x19460);
    func_eboot_0886503C(player, &transform, 14, &preset->arg_20, &preset->arg_30, &preset->arg_10);
    emit_world_model(&transform, &model->scale);
    ge::impl::emit((GE_CMD_TEXOFFSETV << 24) | (preset->texoffset_v.ui >> 8));
    color.rgba.f = D_game_sub_09CCE570[selection->color];
    u8 material_count = model->header->mesh_material_count(preset->mesh);
    u8 *remap = model->header->material_remap(preset->mesh, 0);
    for (int i = 0; i < material_count; ++i) {
        if (i == 0) {
            model->material_data[remap[i]].color.ui = color.rgba.ui;
        }
    }
    if (selection->flags & 2) {
        model->set_mesh_alpha(preset->mesh, (u8)(255.0f * (0.875f + 0.075f * vsin_s(6.2831855f * ((360.0f * (int)(*(u16 *)((u8 *)GameSys::objectPtr + 0x1C) << 9) / 65536.0f) / 360.0f)))));
    } else {
        model->set_mesh_alpha(preset->mesh, 0xFF);
    }
    model->set_mesh_shadow_color(preset->mesh, 0x80, 0x80, 0x80);
    model->drawMesh(0, textures, preset->mesh);
    ge::impl::emit(GE_CMD_TEXOFFSETV << 24);
}
