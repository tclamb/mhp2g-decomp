#include "common.h"

#include "base_stage.hpp"
#include "vfpu.h"
#include "drawable_manager.hpp"
#include "stage_manager.hpp"
#include "immediate_ge.hpp"

using namespace immediate_ge;

#pragma opt_unroll_loops on

base_stage::base_stage() {

}

base_stage::~base_stage() {
    // empty
}

extern struct global_089C7508 {
    u8 padding_0x0[0x1C];
    u16 short_0x1C;
    u8 padding_0x1E[0x2E - 0x1E];
    u8 byte_0x2E;
    u8 padding_0x2F[0x422-0x2F];
    bool allow_hidden_flag;
    u8 padding_0x423[0x6AF0E - 0x423];
    u16 stage_id;
    u8 padding_0x6AF10[0x6AF14 - 0x6AF10];
    u32 flags_0x6AF14;
} *D_eboot_089C7508;

void base_stage::execute_model_draw_commands() {
    pmo *pmo = &model_pmo;
    stage_draw_command *command = vtable_0x48()->model_commands;
    emit_world_model(&transform, &pmo->scale);
    for (int i = 0; i < vtable_0x48()->model_commands_length; ++i, ++command) {
        ge::atest(0xFF, command->alpha_threshold, GE_OP_AT_LEAST);
        if ((command->flags & 1) == 0 || !D_eboot_089C7508->allow_hidden_flag) {
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_ALWAYS);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(true);
            }
            switch (command->opcode) {
            case 0:
                break;
            case 1:
                pmo->draw_mesh(0, &model_tmh, command->mesh_index);
                break;
            case 2:
                vtable_0x54(pmo, command->data, command->mesh_index);
                break;
            case 3:
                vtable_0x58(pmo, command->data, command->mesh_index);
                break;
            case 4:
                vtable_0x5C(pmo, command->data, command->mesh_index);
                break;
            case 6:
                vtable_0x64(pmo, command->data, command->mesh_index);
                break;
            case 7:
                vtable_0x68(pmo, command->data, command->mesh_index);
                break;
            case 8:
                vtable_0x6C(pmo, command->data, command->mesh_index);
                break;
            case 9:
                vtable_0x70(pmo, command->data, command->mesh_index);
                break;
            case 11:
                vtable_0x78(pmo, command->data, command->mesh_index);
                break;
            case 15:
                vtable_0x88(pmo, command->data);
                break;
            case 18:
                vtable_0x94(pmo, command->data, command->mesh_index);
                // fallthrough
            case 19:
                // stage-defined opcode (see stages 13, 17, 39, 46, 66, 80, 99, 124, 192)
                vtable_0x98(pmo, command->data);
            default:
                break;
            }
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_AT_MOST);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(false);
            }
        }
    }
}

void base_stage::execute_prop_draw_commands() {
    pmo *pmo = &prop_pmo;
    stage_draw_command *command = vtable_0x48()->prop_commands;
    emit_world_model(&transform, &pmo->scale);
    for (int i = 0; i < vtable_0x48()->prop_commands_length; ++i, ++command) {
        ge::atest(0xFF, command->alpha_threshold, GE_OP_AT_LEAST);
        if ((command->flags & 1) == 0 || !D_eboot_089C7508->allow_hidden_flag) {
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_ALWAYS);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(true);
            }
            switch (command->opcode) {
            case 1:
                pmo->draw_mesh(0, &model_tmh, command->mesh_index);
                break;
            case 2:
                vtable_0x54(pmo, command->data, command->mesh_index);
                break;
            case 3:
                vtable_0x58(pmo, command->data, command->mesh_index);
                break;
            case 4:
                vtable_0x5C(pmo, command->data, command->mesh_index);
                break;
            case 5:
                vtable_0x60(pmo, command->data, command->mesh_index);
                break;
            case 6:
                vtable_0x64(pmo, command->data, command->mesh_index);
                break;
            case 7:
                vtable_0x68(pmo, command->data, command->mesh_index);
                break;
            case 8:
                vtable_0x6C(pmo, command->data, command->mesh_index);
                break;
            case 9:
                vtable_0x70(pmo, command->data, command->mesh_index);
                break;
            case 10:
                vtable_0x74(pmo, command->data, command->mesh_index);
                break;
            case 12:
                vtable_0x7C(pmo, command->data, command->mesh_index);
                break;
            case 13:
                vtable_0x80(pmo, command->data, command->mesh_index);
                break;
            case 14:
                vtable_0x84(pmo, command->data, command->mesh_index);
                break;
            case 15:
                vtable_0x88(pmo, command->data);
                break;
            case 16:
                vtable_0x8C(pmo, command->data, command->mesh_index);
                break;
            case 17:
                vtable_0x90(pmo, command->data, command->mesh_index);
                break;
            case 19:
                vtable_0x98(pmo, command->data);
                break;
            default:
                break;
            }
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_AT_MOST);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(false);
            }
        }
    }
}

extern "C" {

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x54__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x58__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x5C__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x60__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x64__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x68__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x6C__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x70__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x74__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x78__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x7C__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x80__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x84__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x88__10base_stageFP3pmoPv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x8C__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x90__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x94__10base_stageFP3pmoPvUc);
}

void base_stage::vtable_0x98(pmo *, void *) {
    // empty
}

void base_stage::clear() {
    memset(&prop_pmo, 0, sizeof(prop_pmo));
    memset(&prop_skeleton, 0, sizeof(prop_skeleton));
    reset_transform();
    unknown_0x330 = 0; // environment_params->type
    flag_0x3D4 = false;
    flags &= ~drawable::VISIBLE;
    unknown_0x1C0 = 0; // animation timer?
    unknown_0x1C2 = 0;
    method_088CDCAC();
    set_ptmf_0x3D8(&base_stage::vtable_0x4C);
}

extern "C" {
    extern void *D_eboot_08A5DE5C;
    int func_eboot_08885198(void *, u32, u8, u32, u8);
    void func_eboot_08883858(void *, u32, u32, u32, u32, u32, ScePspFVector4*, u32, u32, u32, bool);
    int func_eboot_088852C8(void *, u32, u32, u32);
    void func_eboot_0888444C(void *, u32, u32, u32, u32, ScePspFVector4 *, u32, u32, u32);

    bool func_eboot_0884F9A0(void *, int);

    extern void *D_eboot_09A4ADAC;
    bool func_eboot_088D0824(void *, u32, u8);
}

void base_stage::vtable_0x1C() {
    int i;
    stage_sound *sound = vtable_0xB0();
    for (i = 0; i < vtable_0xAC(); ++i, ++sound) {
        if ((bool)(D_eboot_089C7508->flags_0x6AF14 & 1) == false && func_eboot_0884F9A0(D_eboot_089C7508, 0) == true) {
            func_eboot_08885198(D_eboot_08A5DE5C, sound->unknown_0x4, 0xC0, i + 1, 0);
        } else {
            if (func_eboot_088D0824(D_eboot_09A4ADAC, sound->unknown_0x8, 0) != false) {
                continue;
            }
            if (sound->unknown_0x0 == 0) {
                stage_manager::get()->register_sound(sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC);
            } else {
                func_eboot_08883858(D_eboot_08A5DE5C, sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC, 0, 0, false);
            }
        }
    }
}

int base_stage::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *base_stage::vtable_0xB0() {
    return definitions()->sounds;
}

void base_stage::vtable_0x20() {
    if ((bool)(D_eboot_089C7508->flags_0x6AF14 & 1) != true) {
        int i;
        u16 *shorts = stage_manager::get()->stage->definitions()->unknown_0x10;
        for (i = 0; i < (int)stage_manager::get()->stage->definitions()->unknown_0x3E; ++i, shorts += 12) {
            int global_sound = 0;
            if (shorts[1] == 0x17 && shorts[11] == 0) {
                ScePspFVector4 *position = vtable_0x44();
                if (position != NULL) {
                    if ((0 < (D_eboot_089C7508->flags_0x6AF14 & 1)) == 0) {
                        u8 byte_0x2E = D_eboot_089C7508->byte_0x2E;
                        switch (byte_0x2E) {
                        default:
                            break;
                        case 6:
                        case 5:
                        case 7:
                        case 8:
                            if (func_eboot_088852C8(D_eboot_08A5DE5C, 6, 0xC0, 0) != false) {
                                func_eboot_08885198(D_eboot_08A5DE5C, 6, 0xC0, 0, 0);
                            }
                            global_sound = 1;
                            break;
                        }
                    }
                    if (global_sound == 0) {
                        stage_manager::get()->register_sound(6, 0x11, 0, 0xC0, 0, position, 9);
                    }
                }
                break;
            }
        }
    }
    stage_sound *sound = vtable_0xB0();
    for (int i = 0; i < vtable_0xAC(); ++i, ++sound) {
        if ((0 < (D_eboot_089C7508->flags_0x6AF14 & 1)) == 0 &&
            func_eboot_0884F9A0(D_eboot_089C7508, 0) == 1) {
            if (func_eboot_088852C8(D_eboot_08A5DE5C, sound->unknown_0x4, 0xC0, i + 1) != 0) {
                func_eboot_08885198(D_eboot_08A5DE5C, sound->unknown_0x4, 0xC0, i + 1, 0);
            }
        } else if (func_eboot_088D0824(D_eboot_09A4ADAC, sound->unknown_0x8, 1) == 0) {
            if (sound->unknown_0x0 == 0) {
                stage_manager::get()->register_sound(sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC);
            } else {
                if (D_eboot_089C7508->short_0x1C % sound->unknown_0x0 == 0) {
                    func_eboot_08883858(D_eboot_08A5DE5C, sound->unknown_0x4, sound->unknown_0x8, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC, 0, 0, false);
                }
            }
            if (sound->unknown_0x0 != 0 && sound->unknown_0x8 != 0x50) {
                func_eboot_0888444C(D_eboot_08A5DE5C, sound->unknown_0x4, 0, 0xC0, i + 1, &sound->position, sound->unknown_0xC, 0, 0);
            }
        }
    }
}

void base_stage::call_ptmf_0x3D8() {
    if (ptmf_0x3D8 != 0) {
        (this->*ptmf_0x3D8)();
    }
}

void base_stage::destroy() {
    func_eboot_08885198(D_eboot_08A5DE5C, 6, 0xC0, 0, 0);
    int i;
    stage_sound *sound = vtable_0xB0();
    for (i = 0; i < vtable_0xAC(); ++i, ++sound) {
        func_eboot_08885198(D_eboot_08A5DE5C, sound->unknown_0x4, 0xC0, i + 1, 0);
    }
}

extern "C" {
    extern void *D_eboot_08A5DD28;

    void func_eboot_08860C4C(void *);
    void func_eboot_0886117C(void *, u8 i);
}

void base_stage::draw() {
    if (D_eboot_089C7508->stage_id != 0) {
        if (vtable_0x48() != 0) {
            vmidt_q(&transform);
            if (vtable_0x48()->model_commands != 0) {
                execute_model_draw_commands();
            }
            if (vtable_0x48()->prop_commands != 0) {
                execute_prop_draw_commands();
            }
        }

        ge::texoffsetu();
        ge::texoffsetv();
        ge::atest(0xFF, 0, GE_OP_GREATER_THAN);
        func_eboot_08860C4C(D_eboot_08A5DD28);
        func_eboot_0886117C(D_eboot_08A5DD28, 1);
    }
}

stage_definitions *base_stage::definitions() {
    return 0;
}

stage_exit *base_stage::exits(u32 ignored_map_id) {
    stage_definitions *d = definitions();
    if (d != 0) {
        return d->exits;
    }
    return 0;
}

s8 base_stage::exit_count(u32 ignored_map_id) {
    stage_definitions *d = definitions();
    u8 count;
    if (d != 0) {
        count = d->exit_count;
    } else {
        count = 0;
    }
    return count;
}

#ifdef BUILD_NONMATCHING
// score 45: 9 regswaps on the args passed to ge::fog; when is ft0 used??
void base_stage::emit_fog() {
    stage_fog *fog = &this->fog;
    if (drawable_manager::get()->start_fragment(render_group::RESET)) {
        float begin = fog->begin;
        float end = fog->end;
        float norm = ge_manager::get()->norm;
        begin *= norm;
        end *= norm;
        ge::fog(fog->color, begin, end);
        drawable_manager::get()->end_fragment();
    }
}
#else
extern "C"
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", emit_fog__10base_stageFv);
#endif

void base_stage::vtable_0x24() {
    method_088CEA2C();
}

void base_stage::method_088CD61C() {
    if (flag_0x3D4 != 0) {
        ge::ditherenable(true);
        drawable_manager::get()->dither_matrix(0);

        ge::alphatestenable(false);
        ge::lightingenable(false);

        ge::alphablendenable(true);
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);

        ge::ztestenable(false);
        ge::ztest(GE_OP_NEVER);

        ge::texturemapenable(false);

        ge::shademode(GE_SHADE_GOURAUD);

        draw_sky_gradient();
        draw_flash();

        ge::ztest(GE_OP_AT_MOST);
        ge::ztestenable(true);

        ge::alphatestenable(true);

        ge::ditherenable(false);

        flag_0x3D4 = false;
    }
}

void base_stage::compile_environment_params(stage_environment_params *environment) {
    unknown_0x330 = environment->type;

    float *in;
    switch (unknown_0x330) {
    case 1:
        in = (float *)environment->data;
        in = compile_fog_params(in);
        compile_unk1_params(in);
        break;
    case 2:
        in = (float *)environment->data;
        in = compile_fog_params(in);
        in = compile_unk1_params(in);
        compile_sky_gradient(in);
        break;
    default:
        unknown_0x330 = 0;
        break;
    }
}

void base_stage::operator delete(void *) {
    // empty
}

void base_stage::vtable_0x4C() {
    sky_gradient_vdata[2].x = 0;
    sky_gradient_vdata[0].x = 0;
    sky_gradient_vdata[3].x = 480;
    sky_gradient_vdata[1].x = 480;
    sky_gradient_vdata[3].z = 0;
    sky_gradient_vdata[1].z = 0;
    sky_gradient_vdata[2].z = 0;
    sky_gradient_vdata[0].z = 0;
    flags |= drawable::VISIBLE;
    vtable_0x1C();
    method_088CDCAC();
    set_ptmf_0x3D8(&base_stage::vtable_0x50);
}

extern "C" {
    struct resource_type {
        enum {
            PICKAXE = 3,
            BUG_NET = 4,
        };
    private: resource_type();
    };

    struct resource_node {
        ScePspFVector3 position;
        float unknown_0xC; // interaction distance?
        u16 unknown_0x10; // loot table?
        u16 remaining;
        u16 type;
        u16 unknown_0x16; // high tier rolls?
    };

    extern void *D_eboot_08A5DD4C;
    resource_node *func_eboot_08869568(void *, int);

    struct bug_flag {
        enum {
            SCALE_WITH_ALPHA = 1,
            PERSISTENT = 2,
            RANDOM_SPAWN_POSITION = 4,
        };
    private: bug_flag();
    };
}


void base_stage::vtable_0x50() {
    if (flash_state != 0) {
        if (flash_state == 1) {
            if (--flash_frames <= 0) {
                flash_state = 2;
                flash_frames = 2;
            }
        } else if (flash_state == 2) {
            if (--flash_frames <= 0) {
                flash_state = 3;
                flash_frames = 0x5A;
            }
        } else {
            if (--flash_frames <= 0) {
                flash_state = 0;
                flash_frames = 0;
            }
        }
    }
    resource_node *node = NULL;
    if ((bool)(D_eboot_089C7508->flags_0x6AF14 & 1) != true) {
        node = func_eboot_08869568(D_eboot_08A5DD4C, D_eboot_089C7508->stage_id);
    }
    if (node) {
        while (node->position.x != -1.0f) {
            if (node->remaining > 0 && node->type == resource_type::BUG_NET) {
                stage_definitions *assets = stage_manager::get()->stage->definitions();
                if (assets->bug_mesh_index != -1 && (unknown_0x1C0 & 0x7F) == 0) {
                    ScePspFVector4 spawn_center, spawn_box;
                    sv_q(&spawn_center, node->position.x, node->position.y + 65.0f, node->position.z, 0.0f);
                    sv_q(&spawn_box, 64.0f, 64.0f, 64.0f, 0.0f);
                    switch (D_eboot_089C7508->stage_id) {
                        case stages::TOWER_3:
                        case stages::GREAT_FOREST_N_2: {
                            stage_definitions *assets = stage_manager::get()->stage->definitions();
                            u32 flags = bug_flag::RANDOM_SPAWN_POSITION;
                            stage_manager::get()->push_prop_089B969C(flags, assets->bug_mesh_index, &spawn_center, &spawn_box);
                            break;
                        }
                        default: {
                            stage_definitions *assets = stage_manager::get()->stage->definitions();
                            u32 flags = bug_flag::RANDOM_SPAWN_POSITION | bug_flag::SCALE_WITH_ALPHA;
                            stage_manager::get()->push_prop_089B969C(flags, assets->bug_mesh_index, &spawn_center, &spawn_box);
                            break;
                        }
                    }
                }
            }
            ++node;
        }
    }
    method_088CDCAC();
    vtable_0x20();
    ++unknown_0x1C0;
    unknown_0x1C2 += 0x2E;
}

extern "C" {
    extern void *D_game_task_09C0D430;
    u8 func_game_task_09AAEE58(void *, u8, bool, bool);
}


bool base_stage::method_088CDC74() {
    return func_game_task_09AAEE58(D_game_task_09C0D430, 54, false, false) == true;
}

extern "C" {
    bool func_eboot_0886A304(void *, bool);
    int func_eboot_0886A354(void *, bool);
}

void base_stage::method_088CDCAC() {
    if (vtable_0xA4() == false) {
        unknown_0x444 = 0;
        return;
    }

    if (func_eboot_0886A304(D_eboot_08A5DD4C, true)) {
        return;
    }

    if (func_game_task_09AAEE58(D_game_task_09C0D430, 54, true, true) == true) {
        unknown_0x444 = 2;
        return;
    }

    if (func_eboot_0886A354(D_eboot_08A5DD4C, true) - func_eboot_0886A354(D_eboot_08A5DD4C, false) < 900) {
        unknown_0x444 = 0;
        return;
    }

    if ((func_eboot_0886A354(D_eboot_08A5DD4C, false) / 7200 & 1) != 0) {
        unknown_0x444 = 0;
        return;
    }

    unknown_0x444 = 1;
}

bool base_stage::vtable_0xA4() {
    return false;
}

void lerp_bgra8888(float *out, void *ignored, u8 *a, u8 *b, float t);

inline u32 lerp_haze(void *ignored, u8 *a, u8 *b, float t) {
    ScePspUnion32 result;
    lerp_bgra8888(&result.f, ignored, a, b, t);
    return result.ui;
}

inline u16 atan2s16(float y, float x) {
    return (int)((65536.0f * atan2f_s(y, x)) / 6.2831855f + 0.5f);
}

void base_stage::draw_sky_gradient() {
    u16 camera_angle = atan2s16(-D_eboot_089C6CB4->position.x, -D_eboot_089C6CB4->position.z);
    u16 sun_angle = atan2s16(sky_gradient_origin.x, sky_gradient_origin.z);
    u16 dtheta = sun_angle - camera_angle;
    u16 top = sky_gradient_top;
    sky_gradient_vdata[1].y = top;
    sky_gradient_vdata[0].y = top;
    float height = sky_gradient_height / 448.0f;
    u16 bottom = height * 272.0f;
    sky_gradient_vdata[3].y = bottom;
    sky_gradient_vdata[2].y = bottom;
    if (dtheta < 16384) {
        float t = dtheta / 16384.0f;
        sky_gradient_vdata[0].color.ui = lerp_haze(this, sky_gradient_colors[0].uc, sky_gradient_colors[2].uc, t);
        sky_gradient_vdata[1].color.ui = lerp_haze(this, sky_gradient_colors[0].uc, sky_gradient_colors[3].uc, t);
        sky_gradient_vdata[2].color.ui = lerp_haze(this, sky_gradient_colors[4].uc, sky_gradient_colors[6].uc, t);
        sky_gradient_vdata[3].color.ui = lerp_haze(this, sky_gradient_colors[4].uc, sky_gradient_colors[7].uc, t);
    } else if (dtheta < 2 * 16384) {
        float t = (dtheta - 0x4000) / 16384.0f;
        sky_gradient_vdata[0].color.ui = lerp_haze(this, sky_gradient_colors[2].uc, sky_gradient_colors[1].uc, t);
        sky_gradient_vdata[1].color.ui = lerp_haze(this, sky_gradient_colors[3].uc, sky_gradient_colors[1].uc, t);
        sky_gradient_vdata[2].color.ui = lerp_haze(this, sky_gradient_colors[6].uc, sky_gradient_colors[5].uc, t);
        sky_gradient_vdata[3].color.ui = lerp_haze(this, sky_gradient_colors[7].uc, sky_gradient_colors[5].uc, t);
    } else if (dtheta < 3 * 16384) {
        float t = (dtheta - 0x8000) / 16384.0f;
        sky_gradient_vdata[0].color.ui = lerp_haze(this, sky_gradient_colors[1].uc, sky_gradient_colors[3].uc, t);
        sky_gradient_vdata[1].color.ui = lerp_haze(this, sky_gradient_colors[1].uc, sky_gradient_colors[2].uc, t);
        sky_gradient_vdata[2].color.ui = lerp_haze(this, sky_gradient_colors[5].uc, sky_gradient_colors[7].uc, t);
        sky_gradient_vdata[3].color.ui = lerp_haze(this, sky_gradient_colors[5].uc, sky_gradient_colors[6].uc, t);
    } else {
        float t = (dtheta - 0xC000) / 16384.0f;
        sky_gradient_vdata[0].color.ui = lerp_haze(this, sky_gradient_colors[3].uc, sky_gradient_colors[0].uc, t);
        sky_gradient_vdata[1].color.ui = lerp_haze(this, sky_gradient_colors[2].uc, sky_gradient_colors[0].uc, t);
        sky_gradient_vdata[2].color.ui = lerp_haze(this, sky_gradient_colors[7].uc, sky_gradient_colors[4].uc, t);
        sky_gradient_vdata[3].color.ui = lerp_haze(this, sky_gradient_colors[6].uc, sky_gradient_colors[4].uc, t);
    }
    ge::vertextype(
        GE_VTYPE_TC_NONE,
        GE_VTYPE_COL_8888,
        GE_VTYPE_NRM_NONE,
        GE_VTYPE_POS_S16,
        GE_VTYPE_WEIGHT_NONE,
        GE_VTYPE_IDX_NONE,
        0,
        0,
        true);
    ge::vaddr(sky_gradient_vdata);
    ge::prim(GE_PRIM_TRIANGLE_STRIP, 4);
}

template<typename T, typename U>
inline T lerp(T a, T b, U t) {
    return a + (T) (t * (b - a));
}

void lerp_bgra8888(float *out, void *ignored, u8 *a, u8 *b, float t) {
    ScePspUnion32 result;
    result.uc[0] = lerp(a[2], b[2], t);
    result.uc[1] = lerp(a[1], b[1], t);
    result.uc[2] = lerp(a[0], b[0], t);
    result.uc[3] = lerp(a[3], b[3], t);
    *out = result.f;
}

void base_stage::draw_flash() {
    if (flash_state != 0) {
        ge::vertextype(
            GE_VTYPE_TC_NONE,
            GE_VTYPE_COL_8888,
            GE_VTYPE_NRM_NONE,
            GE_VTYPE_POS_S16,
            GE_VTYPE_WEIGHT_NONE,
            GE_VTYPE_IDX_NONE,
            0,
            0,
            true);
        u8 blend_mask;
        if (flash_state == 1) {
            blend_mask = 0xFF;
            ge::blendfixeda(0xFF, 0xFF, 0xFF);
            ge::blendfixedb(0xFF, 0xFF, 0xFF);
            ge::blendmode(GE_BLENDMODE_MUL_AND_SUBTRACT, GE_SRCBLEND_FIXA, GE_DSTBLEND_FIXB);
        } else if (flash_state == 2) {
            blend_mask = 0xFF - (u8)(flash_frames * 127.5f);
            flash_blank_vdata[1].color.rgba8888 = (blend_mask << 24) | 0x000000;
            flash_blank_vdata[0].color.rgba8888 = (blend_mask << 24) | 0x000000;
            ge::vaddr(flash_blank_vdata);
            ge::prim(GE_PRIM_RECTANGLES, 2);
            ge::blendfixeda(0xFF, 0xFF, 0xFF);
            ge::blendfixedb(0xFF, 0xFF, 0xFF);
            ge::blendmode(GE_BLENDMODE_MUL_AND_SUBTRACT, GE_SRCBLEND_FIXA, GE_DSTBLEND_FIXB);
        } else {
            float x = vsin_s(6.2831855f * (((360.0f * (float) ((flash_frames * 0xB6) + 0x7FFF + 0x4001)) / 65536.0f) / 360.0f));
            blend_mask = (x + 1.0f) * 255.0f;
            ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);
        }

        if (flash_state - 1U < 2) {
            flash_blend_vdata[1].color.rgba8888 = 0xFFFFFFFF;
            flash_blend_vdata[0].color.rgba8888 = 0xFFFFFFFF;
        } else {
            u32 color = (blend_mask << 24) | 0xFFFFFF;
            flash_blend_vdata[1].color.rgba8888 = color;
            flash_blend_vdata[0].color.rgba8888 = color;
        }
        ge::vaddr(flash_blend_vdata);
        ge::prim(GE_PRIM_RECTANGLES, 2);
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);
    }
}

void base_stage::method_088CEA2C() {
    flash_state = 0;
    flash_frames = 0;

    flash_blend_vdata[0].z = 0;
    flash_blend_vdata[0].y = 0;
    flash_blend_vdata[0].x = 0;
    flash_blend_vdata[1].x = 480;
    flash_blend_vdata[1].y = 272;
    flash_blend_vdata[1].z = 0;

    // oops! fortunately this object is allocated in the bss & the missed values are always 0
    flash_blend_vdata[0].z = 0;
    flash_blend_vdata[0].y = 0;
    flash_blank_vdata[0].x = 0;
    flash_blank_vdata[1].x = 480;
    flash_blank_vdata[1].y = 272;
    flash_blank_vdata[1].z = 0;
}

extern "C" {
    void *memcpy(void *, void *, int);
}


float *base_stage::compile_fog_params(float *in) {
    memcpy(&fog.color, in++, sizeof(fog.color));
    memcpy(&fog.begin, in++, sizeof(fog.begin));
    memcpy(&fog.end, in++, sizeof(fog.end));
    return in;
}

float *base_stage::compile_unk1_params(float *in) {
    u32 size = sizeof(unknown_0x334[0]);
    u32 stride = sizeof(unknown_0x334[0]) / sizeof(*in);
    memcpy(&unknown_0x334[0], in + 0 * stride, size);
    memcpy(&unknown_0x334[3], in + 1 * stride, size);
    memcpy(&unknown_0x334[6], in + 2 * stride, size);
    memcpy(&unknown_0x334[1], in + 3 * stride, size);
    memcpy(&unknown_0x334[4], in + 4 * stride, size);
    memcpy(&unknown_0x334[7], in + 5 * stride, size);
    memcpy(&unknown_0x334[2], in + 6 * stride, size);
    memcpy(&unknown_0x334[5], in + 7 * stride, size);
    memcpy(&unknown_0x334[8], in + 8 * stride, size);
    return in + 9 * stride;
}

inline u32 from_bgra8888(ScePspUnion32 &x) {
    return (x.uc[3] << 24) | (x.uc[0] << 16) |  (x.uc[1] << 8) | x.uc[2];
}

float *base_stage::compile_sky_gradient(float *in) {
    memcpy(&sky_gradient_origin, in, 0xC);
    memcpy(&sky_gradient_top, in + 3, 4);
    ScePspUnion32 x;
    memcpy(&x, in +  4, 4); sky_gradient_colors[0].ui = from_bgra8888(x);
    memcpy(&x, in +  5, 4); sky_gradient_colors[1].ui = from_bgra8888(x);
    memcpy(&x, in +  6, 4); sky_gradient_colors[2].ui = from_bgra8888(x);
    memcpy(&x, in +  7, 4); sky_gradient_colors[3].ui = from_bgra8888(x);
    memcpy(&x, in +  8, 4); sky_gradient_colors[4].ui = from_bgra8888(x);
    memcpy(&x, in +  9, 4); sky_gradient_colors[5].ui = from_bgra8888(x);
    memcpy(&x, in + 10, 4); sky_gradient_colors[6].ui = from_bgra8888(x);
    memcpy(&x, in + 11, 4); sky_gradient_colors[7].ui = from_bgra8888(x);
    return in + 12;
}

stage_definitions_0x28_t *base_stage::method_088CEDC0() {
    stage_definitions *d = definitions();
    stage_definitions_0x28_t *result;
    if (d != 0) {
        result = d->unknown_0x28;
    } else {
        result = NULL;
    }
    return result;
}

stage_definitions_0x2C_t *base_stage::method_088CEDF0() {
    stage_definitions *d = definitions();
    stage_definitions_0x2C_t *result;
    if (d != 0) {
        result = d->unknown_0x2C;
    } else {
        result = NULL;
    }
    return result;
}

stage_definitions_0x28_t *base_stage::vtable_0x34() {
    return NULL;
}

void *base_stage::vtable_0x38() {
    return NULL;
}

stage_draw_commands *base_stage::vtable_0x48() {
    return NULL;
}

void *base_stage::vtable_0x3C() {
    return NULL;
}

void *base_stage::vtable_0x40() {
    return NULL;
}

ScePspFVector4 *base_stage::vtable_0x44() {
    return NULL;
}

u8 base_stage::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *base_stage::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
