#pragma once

#include "singleton.hpp"
#include "drawable.hpp"

extern u32 *DRAWABLE_WRITE_HEAD;


struct render_group {
    enum {
        GROUP_0,
        RESET,
        GROUP_2,
        STAGE,
        GROUP_4,
        GROUP_5,
        GROUP_6,
        GROUP_7,
        GROUP_8,
        GROUP_9,
        GROUP_10,
        GROUP_11,
        GROUP_12,
        GROUP_13,
        GROUP_14,
        GROUP_15,
        GROUP_16,
        GROUP_17,
        GROUP_18,
        GROUP_19,
        GROUP_COUNT
    } value;
private:
    render_group();
};

struct drawable_manager : singleton<drawable_manager> {
    drawable *z_index[111]; // 111 == sum(z_index_bucket_length)
    drawable **z_index_buckets[render_group::GROUP_COUNT];
    u8 writing;
    u8 fragment_group;
    u32 *fragment_start;
    bool flag_0x214;
    u8 unknown_0x215;
    void *unknown_0x218;
    u8 padding_0x21C[12];


    drawable_manager();
    ~drawable_manager();
    void reset();
    void clear();
    bool start_fragment(u8 group);
    void end_fragment();
    void initialize();
    void dither_matrix(u8);
    int add(u8 group, drawable *object, ScePspFVector4 *position, bool no_culling);
    int add(u8 group, model *model, bool no_culling);
    bool method_0884CCC0(void *unknown_data, u8 unknown_index);
    void world_model(ScePspFMatrix4 *transform);

private:
    inline drawable **head(u8 group, int index);
};
