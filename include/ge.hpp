#pragma once

#include "common.h"
#include "model.hpp" // FIXME: move tmh structs into their own header

typedef u32 ge_command;
typedef ge_command *display_list;

struct Ge {
    float norm;
    u32 active_buffer;
    ge_command root_display_lists[2][2*20];
    display_list active_branches[20];
    ge_command slab[2][65536];
    ge_command *active_write_head;
    SceUID ge_callback_id;
    bool waiting_for_ge;

    void initialize();
    void swap_buffers();
    void clear_display_list();
    void render();
    void spinlock_until_ge_end();
    ge_command *write_head();
    void set_write_head(ge_command *);
};

extern volatile bool GE_END_REACHED;


extern "C" {
    struct GeTexture {
        tmh_image_header *data;
        u32 format;
        u16 width;
        u16 height;
        float *palette_data;
        u32 palette_width;
        u32 palette_height;
    };

    // copies a display list fragment to the write head and calls that copy at the specified index
    bool func_eboot_088593A0(Ge *, u32 *display_list, s32 length, s32 fragment_index);
    int func_eboot_08859768(Ge *, tmh_header *, s32, u32, u32, GeTexture *);
    u32 func_eboot_0885973C(Ge *, u32);
}
