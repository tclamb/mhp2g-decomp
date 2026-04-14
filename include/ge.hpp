#pragma once

#include "common.h"
#include "model_base.hpp" // FIXME: move tmh structs into their own header
#include "singleton.hpp"

typedef u32 ge_command;
typedef ge_command *display_list;

struct GeTexture {
    tmh_image_header *data;
    u32 format;
    u16 width;
    u16 height;
    float *palette_data;
    u32 palette_width;
    u32 palette_height;
};

struct Ge : Singleton<Ge> {
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

    // copies a display list fragment to the write head and calls that copy at the specified index
    bool method_088593A0(u32 *display_list, s32 length, s32 fragment_index);
    int method_08859768(tmh_header *, s32, u32, u32, GeTexture *);
    u32 method_0885973C(u32);

    Ge();
};

extern volatile bool GE_END_REACHED;
