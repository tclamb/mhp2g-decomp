#pragma once

#include "common.h"
#include "draw.hpp"
#include "obj_base.hpp"
#include "singleton.hpp"

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

struct DrawManager : Singleton<DrawManager> {
    Draw *z_index[111]; // 111 == sum(z_index_bucket_length)
    Draw **z_index_buckets[render_group::GROUP_COUNT];
    u8 writing;
    u8 fragment_group;
    u32 *fragment_start;
    bool vram_transfer_queued;
    u8 vram_transfer_fragment_index;
    void *vram_transfer_dst;
    u8 padding_0x21C[12];

    DrawManager();
    ~DrawManager();
    void reset();
    void clear();
    void draw();
    void initialize();
    void dither_matrix(u8);
    bool start_fragment(u8 group);
    void end_fragment();
    int add(u8 group, ObjBase *character, bool no_culling);
    int add(u8 group, ModelBase *model, bool no_culling);
    int add(u8 group, Draw *object, ScePspFVector4 *position, bool no_culling);
    bool queue_vram_transfer(void *unknown_data, u8 unknown_index);
    void world_model(ScePspFMatrix4 *transform);

private:
    bool vram_transfer();

    inline Draw **head(u8 group, int index);
};
