#pragma once

#include "game_sys.hpp"
#include "obj_base.hpp"
#include "common.h"

// stub for type with vtable at D_eboot_089B5E78
struct Player : ObjBase {
    s32 buttons;
    u8 padding_0x484[0x4E8 - 0x484];
    EquipmentSet equipment;
    u8 padding_0x530[0x552 - 0x530];
    u8 dialog_timer;
    void *stage_unique;
    u8 padding_0x558[0x564 - 0x558];
    u8 scope_aim_increment;
    u8 padding_0x565[0x614 - 0x565];
    u32 attributes;
    u8 padding_0x618[0x67C - 0x618];
    s8 magazine_remaining;
    s8 magazine_capacity;
    s8 bowgun_pitch;
    s16 pchngr_pitch;

    inline bool Pl_master_ck() {
        return pl_id == GameSys::objectPtr->player_id;
    }

    bool activeSkill(u8 skillId);
    u8 Pl_Skill_type_ck(u8 skillType);
    bool Pl_PlateSkill_ck(u8 kitchenSkillId);
    void bagAdd(u16 itemId, s16 quantity, bool skipMask);
    bool Pl_bari_ck();
    s8 Gun_targeting_stat();
    bool Pl_senkai_chk();
    bool pch_lock_chk();
    bool pl_scope_chk();

    bool Pl_ClimbChk(Player *pl); // odd
};

// TODO: rename & move to player_quest.hpp
struct GamePlayer : Player {
    ScePspFMatrix4 *pch_aim_mat();
    bool pch_lock_chk();
};
