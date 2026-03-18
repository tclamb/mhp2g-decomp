#pragma once

#include "singleton.hpp"

typedef u32 ge_command;
typedef ge_command *display_list;

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
};

extern volatile bool GE_END_REACHED;
