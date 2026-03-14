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
    u8 padding_0x0[0x422];
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
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x1C__10base_stageFv);
}

int base_stage::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *base_stage::vtable_0xB0() {
    return definitions()->sounds;
}

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x20__10base_stageFv);
}

void base_stage::call_ptmf_0x3D8() {
    if (ptmf_0x3D8 != 0) {
        (this->*ptmf_0x3D8)();
    }
}

extern "C" {
    extern void *D_eboot_08A5DE5C;

    int func_eboot_08885198(void *, u32, u8, s32, u8);
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

        vtable_0x9C();
        method_088CE668();

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
        compile_unk2_params(in);
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
    depth_buffer_params[2].unknown_0x4 = 0;
    depth_buffer_params[0].unknown_0x4 = 0;
    depth_buffer_params[3].unknown_0x4 = 480;
    depth_buffer_params[1].unknown_0x4 = 480;
    depth_buffer_params[3].unknown_0x8 = 0;
    depth_buffer_params[1].unknown_0x8 = 0;
    depth_buffer_params[2].unknown_0x8 = 0;
    depth_buffer_params[0].unknown_0x8 = 0;
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
    if (unknown_0x3A0 != 0) {
        if (unknown_0x3A0 == 1) {
            if (--unknown_0x3A2 <= 0) {
                unknown_0x3A0 = 2;
                unknown_0x3A2 = 2;
            }
        } else if (unknown_0x3A0 == 2) {
            if (--unknown_0x3A2 <= 0) {
                unknown_0x3A0 = 3;
                unknown_0x3A2 = 0x5A;
            }
        } else {
            if (--unknown_0x3A2 <= 0) {
                unknown_0x3A0 = 0;
                unknown_0x3A2 = 0;
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

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x9C__10base_stageFv);
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

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", method_088CE668__10base_stageFv);
}

void base_stage::method_088CEA2C() {
    unknown_0x3A0 = 0;
    unknown_0x3A2 = 0;
    unknown_0x41C = 0;
    unknown_0x41A = 0;
    unknown_0x418 = 0;
    unknown_0x424 = 480;
    unknown_0x426 = 272;
    unknown_0x428 = 0;
    unknown_0x41C = 0;
    unknown_0x41A = 0;
    unknown_0x430 = 0;
    unknown_0x43C = 480;
    unknown_0x43E = 272;
    unknown_0x440 = 0;
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

float *base_stage::compile_unk2_params(float *in) {
    memcpy(&unknown_0x3A4, in, 0xC);
    memcpy(&unknown_0x3B0, in + 3, 4);
    ScePspUnion32 x;
    memcpy(&x, in +  4, 4); unknown_0x3B4[0] = from_bgra8888(x);
    memcpy(&x, in +  5, 4); unknown_0x3B4[1] = from_bgra8888(x);
    memcpy(&x, in +  6, 4); unknown_0x3B4[2] = from_bgra8888(x);
    memcpy(&x, in +  7, 4); unknown_0x3B4[3] = from_bgra8888(x);
    memcpy(&x, in +  8, 4); unknown_0x3B4[4] = from_bgra8888(x);
    memcpy(&x, in +  9, 4); unknown_0x3B4[5] = from_bgra8888(x);
    memcpy(&x, in + 10, 4); unknown_0x3B4[6] = from_bgra8888(x);
    memcpy(&x, in + 11, 4); unknown_0x3B4[7] = from_bgra8888(x);
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

void *base_stage::vtable_0x44() {
    return NULL;
}

u8 base_stage::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *base_stage::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
