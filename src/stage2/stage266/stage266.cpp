#include "common.h"
#include "base_stage.hpp"
#include "stage_manager.hpp"

struct stage266 : base_stage {
    stage266();
    virtual ~stage266();

    virtual void vtable_0x24();
    virtual stage_definitions *definitions();
    virtual stage_draw_commands *vtable_0x48();
    virtual bool vtable_0xA0();
    virtual bool vtable_0xA4();
    virtual bool vtable_0xA8();
    virtual int vtable_0xAC();
    virtual stage_sound *vtable_0xB0();
    virtual u8 vtable_0xB4();
    virtual stage_definitions_0x38_t *vtable_0xB8();

    static void operator delete(void *);
};

u8 D_stage266_09D5E280[0x10] = {
    0, 0, 0, 0, 1, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0
};

stage_definitions_0x28_t D_stage266_09D5E290[2] = {
    {{0, 0, 0}, 0, 0, 0, 0},
    {{10000.0f, 0, 10000.0f}, 11900.0f, 12000.0f, 1, 2}
};

ScePspFVector2 D_stage266_09D5E2C0[3][4] = {
    {{1, 1}, {1, 0}, {0.3, 0.3}, {0.3, 0}},
    {{0.1, 0.1}, {0.1, 0}, {0.1, 0.1}, {0.1, 0}},
    {{0.1, 0.1}, {0.1, 0}, {0.1, 0.1}, {0.1, 0}}
};

stage_exit D_stage266_09D5E320[1] = {
    {stages::GREAT_FOREST_N_7, 0, {9600, -6400, 11200}, 800, 5900, {}, {10420, -1, 9150}, {}}
};

u8 padding_09D5E354[0xC] = {};

stage_sound D_stage266_09D5E360[1] = {
    {0, 6, 0x79, 0x18, {11000, 0, 11500}}
};

stage_definitions D_stage266_09D5E380 = {
    0, 0, 0,
    D_stage266_09D5E280,
    NULL,
    -1,
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    6,
    D_stage266_09D5E290,
    D_stage266_09D5E2C0,
    D_stage266_09D5E320,
    D_stage266_09D5E360,
    NULL,
    0,
    1,
    0,
    1,
    0,
    0,
    0
};

u8 padding_09D5E3C4[0x4] = {};

vtable_0x64_params D_stage266_09D5E3C8[2] = {
    {0, 0xBB8, 0},
    {0, 0xBB8, 0}
};

u8 padding_09D5E3D4[0xC] = {};

vtable_0x54_params D_stage266_09D5E3E0[1] = {
    {2, 0x2000, {12500, 0, 12500}}
};

stage_draw_command D_stage266_09D5E400[20] = {
    {1, 4, 0x80, 0},
    {1, 0, 0x80, 1},
    {1, 0, 0x80, 2},
    {2, 2, 0, 3, &D_stage266_09D5E3E0[0]},
    {6, 2, 0, 4, &D_stage266_09D5E3C8[0]},
    {6, 2, 0, 5, &D_stage266_09D5E3C8[1]},
    {1, 0, 0x80, 6},
    {1, 0, 0x80, 7},
    {1, 0, 0x80, 8},
    {1, 0, 0x80, 9},
    {1, 0, 0x80, 10},
    {1, 0, 0x80, 11},
    {1, 2, 0, 12},
    {1, 2, 0, 13},
    {1, 0, 0x80, 14},
    {1, 0, 0x80, 15},
    {1, 0, 0x80, 16},
    {1, 0, 0x80, 17},
    {1, 0, 0x80, 18},
    {1, 0, 0x80, 19}
};

vtable_0x64_params D_stage266_09D5E4A0[1] = {
    {0, 0x200, 0}
};

stage_draw_command D_stage266_09D5E4A8[2] = {
    {1, 0, 0x80, 7},
    {6, 0, 0x80, 8, &D_stage266_09D5E4A0[0]}
};

stage_draw_commands D_stage266_09D5E4B8 = {
    sizeof(D_stage266_09D5E400) / sizeof(D_stage266_09D5E400[0]),
    sizeof(D_stage266_09D5E4A8) / sizeof(D_stage266_09D5E4A8[0]),
    D_stage266_09D5E400,
    D_stage266_09D5E4A8
};

u8 padding_09D5E4C4[0xC] = {};

prop_089B94DC_data D_stage266_09D5E4D0 = {4, 10, {7379, 70, 11113}};

prop_089B975C_data D_stage266_09D5E4F0 = {
    5,
    {10300, 50, 8700},
    {2400, 1000, 1000}
};

prop_089B975C_data D_stage266_09D5E520 = {
    5,
    {7900, 50, 13000},
    {6300, 1000, 1000}
};

stage266 D_stage266_09D5E610;

ScePspFVector4 D_stage266_09D5EA70;

stage266::stage266() {
    // empty
}

stage266::~stage266() {
    // empty
}

void stage266::vtable_0x24() {
    stage_manager::get()->push_prop_089B94DC(&D_stage266_09D5E4D0);
    stage_manager::get()->push_prop_089B94FC(1, &D_stage266_09D5EA70, 0, 0x100, 0, 0, 1);
    stage_manager::get()->push_prop_089B975C(&D_stage266_09D5E4F0);
    stage_manager::get()->push_prop_089B975C(&D_stage266_09D5E520);
    base_stage::vtable_0x24();
}

stage_definitions *stage266::definitions() {
    return &D_stage266_09D5E380;
}

void stage266::operator delete(void *) {
    // empty
}

stage_draw_commands *stage266::vtable_0x48() {
    return &D_stage266_09D5E4B8;
}

bool stage266::vtable_0xA0() {
    return true;
}

bool stage266::vtable_0xA4() {
    return false;
}

bool stage266::vtable_0xA8() {
    return false;
}

int stage266::vtable_0xAC() {
    return definitions()->sound_count;
}

stage_sound *stage266::vtable_0xB0() {
    return definitions()->sounds;
}

u8 stage266::vtable_0xB4() {
    return definitions()->unknown_0x40;
}

stage_definitions_0x38_t *stage266::vtable_0xB8() {
    return definitions()->unknown_0x38;
}
