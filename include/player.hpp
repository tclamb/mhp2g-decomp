#pragma once

#include "game_sys.hpp"
#include "obj_base.hpp"
#include "common.h"

struct YureBoundingBox {
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float zMin;
    float zMax;
};

struct HairYureDefinition {
    u16 boneRangeIndex;
    u16 unknown_0x2;

    s16 zOffsetIndex;
    s16 restOffsetIndex;
    s16 chestOffsetIndex;
    s16 chestVectorIndex;
    s16 boundingBoxIndex;

    s16 dampingIndex;
    s16 poseBlendIndex;
};

struct HairYureLookupEntry {
    u32 modelId;
    s16 *definitionIndices;
};

struct SkillThreshold {
    u8 type;
    u8 neg20;
    u8 neg15;
    u8 neg10;
    u8 pos10;
    u8 pos15;
    u8 pos20;
};

struct PlFlag {
    enum {
        TOOLBELT_OPEN = 1 << 0,
        SUPPLY_BOX_OPEN = 1 << 1,
        DELIVERY_BOX_OPEN = 1 << 9,
        SCOPE_CAM = 1 << 12,
        LIGHT_COLOR_OVERRIDE = 1 << 14,
        SHOULDER_CAM = 1 << 15,
    };
private:
    PlFlag();
};

struct PlActionType {
    enum {
        NORMAL,
        ATTACK,
        DAMAGE,
        DIE,
        DEMO,
        EGG,
        CHAT,
        KITCHEN,
        FARM,
    };
private:
    PlActionType();
};

struct Player : ObjBase {
    Player();

    virtual void init_sub();
    virtual void settleDynamics();
    virtual void vtable_0x48();
    virtual void vtable_0x4C();
    virtual void vtable_0x50();
    virtual void vtable_0x54();
    virtual void vtable_0x58();
    virtual void Pl_atck_adj_calc();
    virtual void Pl_def_adj_calc();
    virtual void Pl_reg_calc();
    virtual void Pl_vital_calc();
    virtual void Pl_act_set(int, int, int);

    s32 buttons;
    u8 padding_0x484[0x4E8 - 0x484];
    EquipmentSet equipment;
    u8 padding_0x530[0x552 - 0x530];
    u8 dialog_timer;
    void *stage_unique;
    u8 padding_0x558[0x564 - 0x558];
    u8 scope_aim_increment;
    u8 padding_0x565[0x614 - 0x565];
    u32 plFlags;
    u8 padding_0x618[0x67C - 0x618];
    s8 magazine_remaining;
    s8 magazine_capacity;
    s8 bowgun_pitch;
    s16 pchngr_pitch;

    u8 padding_0x682[0x10D0 - 0x682];
    EquipmentColor equipmentColors[5];
    u8 equipmentColorSeries[5];
    u8 padding_0x10E9[0x10F0 - 0x10E9];

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

    void applyHairColor();
    void player_modify();
    void yure_move_hair();
    void refreshEquipmentColorSeries();
    void sw_set_sub();
};

struct PlayerCont : Player {
    PlayerCont();
    virtual void vtable_0x38();
};

struct PlayerLobby : Player {
    PlayerLobby();
    virtual void vtable_0x38();

    u8 padding_0x10F0[0x1120 - sizeof(Player)];
};

struct PlayerQuest : Player {
    PlayerQuest();
    virtual void vtable_0x38();

    ScePspFMatrix4 *pch_aim_mat();
    bool pch_lock_chk();
    void pl_to_normal(int, int, int);

    u8 padding_0x10F0[0x11C0 - sizeof(Player)];
};

enum CatWeaponId {
    BONE_AXE = 5,
    CRYSTAL_AXE = 6,
    DRAGON_AXE = 7,
    PAW_HAMMER = 8,
    ACORN_HAMMER = 9,
    BARREL_HAMMER = 10,
    KATANA = 11,
    OTSUCHI = 12,
};

struct PlayerCat : PlayerQuest {
    PlayerCat();
    virtual void vtable_0x38();

    CatWeaponId catWeaponId();

    u8 padding_0x11C0[0x1610 - sizeof(PlayerQuest)];
};
