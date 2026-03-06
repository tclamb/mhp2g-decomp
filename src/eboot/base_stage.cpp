#include "common.h"

#include "base_stage.hpp"

#include "immediate_ge.hpp"

using namespace immediate_ge;

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CA15C);
}

base_stage::~base_stage() {
    // empty
}

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CA25C);
}

extern struct global_089C7508 {
    u8 padding_0x0[0x422];
    bool allow_hidden_props;
} *D_eboot_089C7508;

extern "C" {
    // emit world matrix for model
    void func_eboot_0886234C(ScePspFMatrix4 *transform, ScePspFVector4 *scale);
    // draw mesh
    void func_eboot_08861714(pmo *pmo, skeleton *skeleton, tmh *, int mesh);
}

void base_stage::method_088CA624() {
    pmo *pmo = &prop_pmo;
    stage_draw_command *command = vtable_0x48()->draw_commands;
    func_eboot_0886234C(&transform, &pmo->scale);
    for (int i = 0; i < vtable_0x48()->draw_commands_length; ++i, ++command) {
        ge::atest(0xFF, command->alpha_threshold, GE_OP_AT_LEAST);
        if ((command->flags & 1) == 0 || !D_eboot_089C7508->allow_hidden_props) {
            if ((command->flags & 4) != 0) {
                ge::ztest(GE_OP_ALWAYS);
            }
            if ((command->flags & 2) != 0) {
                ge::zwritedisable(true);
            }
            switch (command->opcode) {
            case 1:
                func_eboot_08861714(pmo, 0, &model_tmh, command->mesh_index);
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

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x78__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x7C__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x80__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x84__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x88__10base_stageFP3pmoPv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x8C__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x90__10base_stageFP3pmoPvUc);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x94__10base_stageFv);
}

void base_stage::vtable_0x98(pmo *, void *) {
    // empty
}

void base_stage::clear() {
    memset(&prop_pmo, 0, sizeof(prop_pmo));
    memset(&prop_skeleton, 0, sizeof(prop_skeleton));
    reset_transform();
    unknown_0x330 = 0;
    unknown_0x3D4 = 0;
    unknown_0x4 &= ~2;
    unknown_0x1C0 = 0;
    unknown_0x1C2 = 0;
    method_088CDCAC();
    set_ptmf_0x3D8(&base_stage::vtable_0x4C);
}

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x1C__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0xAC__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0xB0__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x20__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x14__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", destroy__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", draw__10base_stageFv);

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

extern "C"
{
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CD4E0);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x24__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CD61C);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", compile_environment_params__10base_stageFP24stage_environment_params);
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
    unknown_0x4 |= 2;
    vtable_0x1C();
    method_088CDCAC();
    set_ptmf_0x3D8(&base_stage::vtable_0x50);
}

extern "C" {
INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x50__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CDC74);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", method_088CDCAC__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0xA4__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x9C__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CE4F4);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CE668);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CEA2C);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CEA70);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CEAD8);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CEB94);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CEDC0);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", func_eboot_088CEDF0);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x34__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x38__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x48__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x3C__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x40__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0x44__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0xB4__10base_stageFv);

INCLUDE_ASM("asm/eboot/nonmatchings/base_stage", vtable_0xB8__10base_stageFv);
}
