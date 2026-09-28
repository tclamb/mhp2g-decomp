#include "common.h"

// Partial layout of the selection fields consumed by the player draw path.
// The equipment names of these modes are not established by this accessor.
struct PlayerPartSelection {
    u8 padding_0000[0x11E8];
    u8 variant;              // +0x11E8
    u8 padding_11E9[4];
    s8 part;                 // +0x11ED
    s8 mode;                 // +0x11EE
};

// Called when Player::lighting_flags has bit 0x400 set to select one extra mesh.
extern "C" s16 func_game_task_09A6E3E0(PlayerPartSelection *player) {
    s16 result;
    if (player->mode != 2) {
        s16 base;
        if (player->variant == 1) {
            base = 5;
        } else {
            base = 8;
        }
        result = (s16)(base + player->part);
    } else {
        if (player->variant == 1) {
            result = 11;
        } else {
            result = 12;
        }
    }
    return result;
}
