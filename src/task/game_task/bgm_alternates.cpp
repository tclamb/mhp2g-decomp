#include "bgm_alternates.hpp"

extern "C" void func_game_task_09A60318(BgmServer *self, u16 stage) {
    self->alternates_1C[1] = 0xFFFF;
    u8 *record = (u8 *)D_game_sub_09CDFBE8;
    self->alternates_1C[0] = 0xFFFF;
    u16 record_stage = *(u16 *)record;
    if (record_stage == 0xFFFF) goto after_loop;
    {
        u8 *slot = (u8 *)self;
        do {
            if (record_stage == stage) {
                *(u16 *)(slot + 0x1C) = *(u16 *)(record + 4);
                slot += 2;
            }
            record += 8;
            record_stage = *(u16 *)record;
        } while (record_stage != 0xFFFF);
    }
after_loop:
    if ((self->flags_14 & 1) != 0) return;
    if (stage == 0x97) goto check_quest;
    switch (stage) {
    case 0x10:
        break;
    default:
        return;
    }
check_quest:
    {
    Quest *quest = Quest::objectPtr;
    GameSys *game = GameSys::objectPtr;
    s32 status;
    switch (*(u8 *)((u8 *)game + 0x480)) {
    case 0:
        status = *(s8 *)(*(u8 **)((u8 *)quest + 0x4C) + 0x45);
        break;
    default:
        status = 0;
        break;
    }
    if (status == 0) self->alternates_1C[1] = 0xFFFF;
    }
}
