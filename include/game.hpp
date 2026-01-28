#ifndef MHP2G_DECOMP__GAME_HPP
#define MHP2G_DECOMP__GAME_HPP

typedef struct game game;

struct game {
    static game *instance;

    u16 next_index(u32 type);

    u8 padding_0x0[0xf7a3ac];
    u16 unknown_indexes[3];
    u8 padding_0xf7a3b2[0x4abe];
    SceUID sha1_thread_id;
    u8 unknown_flag;
};

#endif