#include <pspgecmd.h>

#define emit(out, cmd) do { \
    *out++ = cmd; \
} while(0)

#define jump(out, target) do { \
    emit(out, GE_SET_BASE_BASE8(target)); \
    emit(out, GE_SET_JUMP_ADDR24(target)); \
} while(0)

typedef u32 ge_command;
typedef ge_command *display_list;

struct ge_manager {
    float norm;
    u32 active_buffer;
    ge_command root_display_lists[2][2*20];
    display_list active_branches[20];
    ge_command slab[2][65536];
    ge_command *write_head;
    SceUID ge_callback_id;
    bool waiting_for_ge;

    void initialize();
    void swap_buffers();
    void clear_display_list();
    void render();
    void spinlock_until_ge_end();
};

extern volatile bool GE_END_REACHED;