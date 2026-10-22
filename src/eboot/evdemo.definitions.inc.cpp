#include "evdemo.hpp"
#include "camera.hpp"
#include "game_sys.hpp"

// The definitions of the demo events.  The ptmf members of every
// definition are filled in by the static initializer of this file.
static EvdemoDefinition RECKLESS_BULLDROME_HUNTER_DEMO = {
    .flag = EventFlagId::RECKLESS_BULLDROME_HUNTER_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_7,
    .demo_id = DemoId::RECKLESS_BULLDROME_HUNTER_DEMO,
    .em_type = EmType::BULLDROME,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO61_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition HUNT_DOWN_THE_VELOCIDROME_DEMO = {
    .flag = EventFlagId::HUNT_DOWN_THE_VELOCIDROME_DEMO_SEEN,
    .day_stage_id = StageId::JUNGLE_D_AREA_8,
    .demo_id = DemoId::HUNT_DOWN_THE_VELOCIDROME_DEMO,
    .em_type = EmType::VELOCIDROME,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO30_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_CARNIVOROUS_LEADER_DEMO = {
    .flag = EventFlagId::THE_CARNIVOROUS_LEADER_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_6,
    .demo_id = DemoId::THE_CARNIVOROUS_LEADER_DEMO,
    .em_type = EmType::GIADROME,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO62_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition JUNGLE_MENACE_DEMO = {
    .flag = EventFlagId::JUNGLE_MENACE_DEMO_SEEN,
    .day_stage_id = StageId::JUNGLE_D_AREA_6,
    .demo_id = DemoId::JUNGLE_MENACE_DEMO,
    .em_type = EmType::YIAN_KUTKU,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO44_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_LAND_SHARK_DEMO = {
    .flag = EventFlagId::THE_LAND_SHARK_DEMO_SEEN,
    .day_stage_id = StageId::DESERT_D_AREA_5,
    .demo_id = DemoId::THE_LAND_SHARK_DEMO,
    .em_type = EmType::CEPHADROME,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO11_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition GYPCEROS_VENOMOUS_TERROR_DEMO = {
    .flag = EventFlagId::GYPCEROS_VENOMOUS_TERROR_DEMO_SEEN,
    .day_stage_id = StageId::SWAMP_D_AREA_2,
    .demo_id = DemoId::GYPCEROS_VENOMOUS_TERROR_DEMO,
    .em_type = EmType::GYPCEROS,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO25_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_LURKING_DESERT_GIANT_DEMO = {
    .flag = EventFlagId::THE_LURKING_DESERT_GIANT_DEMO_SEEN,
    .day_stage_id = StageId::DESERT_D_AREA_9,
    .demo_id = DemoId::THE_LURKING_DESERT_GIANT_DEMO,
    .em_type = EmType::DAIMYO_HERMITAUR,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO32_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition RAREST_OF_THE_RARE_BEASTS_DEMO = {
    .flag = EventFlagId::RAREST_OF_THE_RARE_BEASTS_DEMO_SEEN,
    .day_stage_id = StageId::JUNGLE_D_AREA_3,
    .demo_id = DemoId::RAREST_OF_THE_RARE_BEASTS_DEMO,
    .em_type = EmType::CONGALALA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO33_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_ELDER_DRAGON_OF_WIND_DEMO = {
    .flag = EventFlagId::THE_ELDER_DRAGON_OF_WIND_DEMO_SEEN,
    .day_stage_id = StageId::JUNGLE_D_AREA_3,
    .demo_id = DemoId::THE_ELDER_DRAGON_OF_WIND_DEMO,
    .em_type = EmType::KUSHALA_DAORA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO35N_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition SHADOW_IN_THE_SNOW_DEMO = {
    .flag = EventFlagId::SHADOW_IN_THE_SNOW_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_3,
    .demo_id = DemoId::SHADOW_IN_THE_SNOW_DEMO,
    .em_type = EmType::KHEZU,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO45_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_RULER_OF_THE_SNOW_DEMO = {
    .flag = EventFlagId::THE_RULER_OF_THE_SNOW_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_8,
    .demo_id = DemoId::THE_RULER_OF_THE_SNOW_DEMO,
    .em_type = EmType::BLANGONGA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO42_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition WATER_WYVERN_IN_THE_DESERT_DEMO = {
    .flag = EventFlagId::WATER_WYVERN_IN_THE_DESERT_DEMO_SEEN,
    .day_stage_id = StageId::DESERT_D_AREA_7,
    .demo_id = DemoId::WATER_WYVERN_IN_THE_DESERT_DEMO,
    .em_type = EmType::PLESIOTH,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO26_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition COMMANDER_IN_THE_FLAMES_DEMO = {
    .flag = EventFlagId::COMMANDER_IN_THE_FLAMES_DEMO_SEEN,
    .day_stage_id = StageId::VOLCANO_D_AREA_3,
    .demo_id = DemoId::COMMANDER_IN_THE_FLAMES_DEMO,
    .em_type = EmType::SHOGUN_CEANATAUR,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO43_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_RUNAWAY_DIABLOS_DEMO = {
    .flag = EventFlagId::THE_RUNAWAY_DIABLOS_DEMO_SEEN,
    .day_stage_id = StageId::DESERT_D_AREA_7,
    .demo_id = DemoId::THE_RUNAWAY_DIABLOS_DEMO,
    .em_type = EmType::DIABLOS,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO46_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_FROZEN_DICTATOR_DEMO = {
    .flag = EventFlagId::THE_FROZEN_DICTATOR_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_8,
    .demo_id = DemoId::THE_FROZEN_DICTATOR_DEMO,
    .em_type = EmType::KUSHALA_DAORA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO36_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_EMPRESS_S_BLAZING_THRONE_DEMO = {
    .flag = EventFlagId::THE_EMPRESS_S_BLAZING_THRONE_DEMO_SEEN,
    .day_stage_id = StageId::VOLCANO_D_AREA_8,
    .demo_id = DemoId::THE_EMPRESS_S_BLAZING_THRONE_DEMO,
    .em_type = EmType::LUNASTRA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO37_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition EMPEROR_OF_FLAME_DEMO = {
    .flag = EventFlagId::EMPEROR_OF_FLAME_DEMO_SEEN,
    .day_stage_id = StageId::VOLCANO_D_AREA_6,
    .demo_id = DemoId::EMPEROR_OF_FLAME_DEMO,
    .em_type = EmType::TEOSTRA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO41_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TERROR_OF_THE_GRAVIOS_DEMO = {
    .flag = EventFlagId::TERROR_OF_THE_GRAVIOS_DEMO_SEEN,
    .day_stage_id = StageId::VOLCANO_D_AREA_7,
    .demo_id = DemoId::TERROR_OF_THE_GRAVIOS_DEMO,
    .em_type = EmType::GRAVIOS,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO47_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition OVERSEER_OF_THE_ANCIENTS_DEMO = {
    .flag = EventFlagId::OVERSEER_OF_THE_ANCIENTS_DEMO_SEEN,
    .day_stage_id = StageId::TOWER_FATALIS_AREA_10,
    .demo_id = DemoId::OVERSEER_OF_THE_ANCIENTS_DEMO,
    .em_type = EmType::LUNASTRA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO38_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_LEGENDARY_KIRIN_DEMO = {
    .flag = EventFlagId::THE_LEGENDARY_KIRIN_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_6,
    .demo_id = DemoId::THE_LEGENDARY_KIRIN_DEMO,
    .em_type = EmType::KIRIN,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO50_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition BATTLE_OF_THE_BLOS_DEMO = {
    .flag = EventFlagId::BATTLE_OF_THE_BLOS_DEMO_SEEN,
    .day_stage_id = StageId::DESERT_D_AREA_7,
    .demo_id = DemoId::BATTLE_OF_THE_BLOS_DEMO,
    .em_type = EmType::MONOBLOS,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO48_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_ELDER_DRAGON_OF_MIST_DEMO = {
    .flag = EventFlagId::THE_ELDER_DRAGON_OF_MIST_DEMO_SEEN,
    .day_stage_id = StageId::FOREST_AND_HILLS_D_AREA_9,
    .demo_id = DemoId::THE_ELDER_DRAGON_OF_MIST_DEMO,
    .em_type = EmType::CHAMELEOS,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO40_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_FINAL_INVITATION_DEMO = {
    .flag = EventFlagId::THE_FINAL_INVITATION_DEMO_SEEN,
    .day_stage_id = StageId::ARENA_LARGE,
    .demo_id = DemoId::THE_FINAL_INVITATION_DEMO,
    .em_type = EmType::RAJANG,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO63_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition THE_LONE_BLACK_GARUGA_DEMO = {
    .flag = EventFlagId::THE_LONE_BLACK_GARUGA_DEMO_SEEN,
    .day_stage_id = StageId::JUNGLE_D_AREA_6,
    .demo_id = DemoId::THE_LONE_BLACK_GARUGA_DEMO,
    .em_type = EmType::ONE_EAR_YIAN_GARUGA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO39_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition LAOSHAN_LUNG_INTRO = {
    .flag = EventFlagId::LAOSHAN_LUNG_INTRO_SEEN,
    .day_stage_id = StageId::FORTRESS_AREA_1,
    .demo_id = DemoId::LAOSHAN_LUNG_INTRO,
    .em_type = EmType::LAOSHAN_LUNG,
    .bgm_type = EvdemoBgmType::LAOSHAN_LUNG,
    .bgm_id = FileId::EFDEMO17_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition ASHEN_LAOSHAN_LUNG_INTRO = {
    .flag = EventFlagId::ASHEN_LAOSHAN_LUNG_INTRO_SEEN,
    .day_stage_id = StageId::FORTRESS_AREA_1,
    .demo_id = DemoId::ASHEN_LAOSHAN_LUNG_INTRO,
    .em_type = EmType::LAOSHAN_LUNG,
    .bgm_type = EvdemoBgmType::LAOSHAN_LUNG,
    .bgm_id = FileId::EFDEMO34_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition SHEN_GAOREN_INTRO = {
    .flag = EventFlagId::SHEN_GAOREN_INTRO_SEEN,
    .day_stage_id = StageId::FORTRESS_AREA_1,
    .demo_id = DemoId::SHEN_GAOREN_INTRO,
    .em_type = EmType::SHEN_GAOREN,
    .bgm_type = EvdemoBgmType::SHEN_GAOREN,
    .bgm_id = FileId::EFDEMO51_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition SINKING_FEELING_DEMO = {
    .flag = EventFlagId::SINKING_FEELING_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_6,
    .demo_id = DemoId::SINKING_FEELING_DEMO,
    .em_type = EmType::TIGREX,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO65_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition ABSOLUTE_POWER_DEMO = {
    .flag = EventFlagId::ABSOLUTE_POWER_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_8,
    .demo_id = DemoId::ABSOLUTE_POWER_DEMO,
    .em_type = EmType::TIGREX,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO66_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition FATALIS_INTRO = {
    .flag = EventFlagId::FATALIS_INTRO_SEEN,
    .day_stage_id = StageId::CASTLE_SCHRADE_AREA_2,
    .demo_id = DemoId::FATALIS_INTRO,
    .em_type = EmType::FATALIS,
    .bgm_type = EvdemoBgmType::FATALIS,
    .bgm_id = FileId::EFDEMO20_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition CRIMSON_FATALIS_INTRO = {
    .flag = EventFlagId::CRIMSON_FATALIS_INTRO_SEEN,
    .day_stage_id = StageId::BATTLEGROUND_AREA_1,
    .demo_id = DemoId::CRIMSON_FATALIS_INTRO,
    .em_type = EmType::FATALIS,
    .bgm_type = EvdemoBgmType::CRIMSON_FATALIS,
    .bgm_id = FileId::EFDEMO59_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition WHITE_FATALIS_INTRO = {
    .flag = EventFlagId::WHITE_FATALIS_INTRO_SEEN,
    .day_stage_id = StageId::TOWER_FATALIS_AREA_10,
    .demo_id = DemoId::WHITE_FATALIS_INTRO,
    .em_type = EmType::FATALIS,
    .bgm_type = EvdemoBgmType::ELDER_DRAGON,
    .bgm_id = FileId::EFDEMO60_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition AKANTOR_INTRO = {
    .flag = EventFlagId::AKANTOR_INTRO_SEEN,
    .day_stage_id = StageId::BATTLEGROUND_AREA_1,
    .demo_id = DemoId::AKANTOR_INTRO,
    .em_type = EmType::AKANTOR,
    .bgm_type = EvdemoBgmType::ELDER_DRAGON,
    .bgm_id = FileId::EFDEMO67_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition YAMA_TSUKAMI_INTRO = {
    .flag = EventFlagId::YAMA_TSUKAMI_INTRO_SEEN,
    .day_stage_id = StageId::TOWER_YAMA_BASE,
    .demo_id = DemoId::YAMA_TSUKAMI_INTRO,
    .em_type = EmType::YAMA_TSUKAMI_1,
    .bgm_type = EvdemoBgmType::YAMA_TSUKAMI,
    .bgm_id = FileId::EFDEMO56_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition UKANLOS_INTRO = {
    .flag = EventFlagId::UKANLOS_INTRO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_PEAK_AREA_1,
    .demo_id = DemoId::UKANLOS_INTRO,
    .em_type = EmType::UKANLOS,
    .bgm_type = EvdemoBgmType::ELDER_DRAGON,
    .bgm_id = FileId::EFDEMO72_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static FadeCue LAVASIOTH_FADE_TBL[6] = {
    {.time = 0, .type = FadeCueType::FADE_TO_BLACK, .duration = 255},
    {.time = 1, .type = FadeCueType::FADE_TO_BLACK, .duration = 1},
    {.time = 962, .type = FadeCueType::FADE_IN, .duration = 1},
    {.time = 978, .type = FadeCueType::FADE_TO_BLACK, .duration = 1},
    {.time = 1270, .type = FadeCueType::FADE_IN, .duration = 45},
{-1}};

static EvdemoDefinition LAVASIOTH_INTRO = {
    .flag = EventFlagId::LAVASIOTH_INTRO_SEEN,
    .day_stage_id = StageId::VOLCANO_D_AREA_9,
    .demo_id = DemoId::LAVASIOTH_INTRO,
    .em_type = EmType::LAVASIOTH,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO70_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
    .fades = &LAVASIOTH_FADE_TBL[0],
};

static EvdemoDefinition HYPNOCATRICE_INTRO = {
    .flag = EventFlagId::HYPNOCATRICE_INTRO_SEEN,
    .day_stage_id = StageId::GREAT_FOREST_D_AREA_4,
    .demo_id = DemoId::HYPNOCATRICE_INTRO,
    .em_type = EmType::HYPNOCATRICE,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO69_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition NARGACUGA_INTRO = {
    .flag = EventFlagId::NARGACUGA_INTRO_SEEN,
    .day_stage_id = StageId::GREAT_FOREST_D_AREA_5,
    .demo_id = DemoId::NARGACUGA_INTRO,
    .em_type = EmType::NARGACUGA,
    .bgm_type = EvdemoBgmType::DEFAULT,
    .bgm_id = FileId::EFDEMO71_AT3,
    .flags = EvdemoFlag::EM_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TRAINING_BASICS_DEMO = {
    .flag = EventFlagId::TRAINING_BASICS_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_1,
    .demo_id = DemoId::TRAINING_BASICS_DEMO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MISSION_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TRAINING_GATHERING_DEMO_1 = {
    .flag = EventFlagId::TRAINING_GATHERING_DEMO_1_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_3,
    .demo_id = DemoId::TRAINING_GATHERING_DEMO_1,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MISSION_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TRAINING_GATHERING_DEMO_2 = {
    .flag = EventFlagId::TRAINING_GATHERING_DEMO_2_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_1,
    .demo_id = DemoId::TRAINING_GATHERING_DEMO_2,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MISSION_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TRAINING_GATHERING_DEMO_3 = {
    .flag = EventFlagId::TRAINING_GATHERING_DEMO_3_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_4,
    .demo_id = DemoId::TRAINING_GATHERING_DEMO_3,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MISSION_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TRAINING_GATHERING_DEMO_4 = {
    .flag = EventFlagId::TRAINING_GATHERING_DEMO_4_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_2,
    .demo_id = DemoId::TRAINING_GATHERING_DEMO_4,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MISSION_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TRAINING_COMBINING_DEMO = {
    .flag = EventFlagId::TRAINING_COMBINING_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_3,
    .demo_id = DemoId::TRAINING_COMBINING_DEMO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MISSION_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TRAINING_FISHING_DEMO = {
    .flag = EventFlagId::TRAINING_FISHING_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_1,
    .demo_id = DemoId::TRAINING_FISHING_DEMO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MISSION_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TRAINING_TRANSPORT_DEMO = {
    .flag = EventFlagId::TRAINING_TRANSPORT_DEMO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_3,
    .demo_id = DemoId::TRAINING_TRANSPORT_DEMO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MISSION_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition FOREST_AND_HILLS_INTRO = {
    .flag = EventFlagId::FOREST_AND_HILLS_INTRO_SEEN,
    .day_stage_id = StageId::FOREST_AND_HILLS_D_AREA_1,
    .demo_id = DemoId::FOREST_AND_HILLS_INTRO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MAP_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition DESERT_INTRO = {
    .flag = EventFlagId::DESERT_INTRO_SEEN,
    .day_stage_id = StageId::DESERT_D_BASE,
    .demo_id = DemoId::DESERT_INTRO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MAP_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition SWAMP_INTRO = {
    .flag = EventFlagId::SWAMP_INTRO_SEEN,
    .day_stage_id = StageId::SWAMP_D_BASE,
    .demo_id = DemoId::SWAMP_INTRO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MAP_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition VOLCANO_INTRO = {
    .flag = EventFlagId::VOLCANO_INTRO_SEEN,
    .day_stage_id = StageId::VOLCANO_D_BASE,
    .demo_id = DemoId::VOLCANO_INTRO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MAP_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition JUNGLE_INTRO = {
    .flag = EventFlagId::JUNGLE_INTRO_SEEN,
    .day_stage_id = StageId::JUNGLE_D_BASE,
    .demo_id = DemoId::JUNGLE_INTRO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MAP_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition SNOWY_MOUNTAINS_INTRO = {
    .flag = EventFlagId::SNOWY_MOUNTAINS_INTRO_SEEN,
    .day_stage_id = StageId::SNOWY_MOUNTAINS_D_AREA_1,
    .demo_id = DemoId::SNOWY_MOUNTAINS_INTRO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MAP_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition TOWER_INTRO = {
    .flag = EventFlagId::TOWER_INTRO_SEEN,
    .day_stage_id = StageId::TOWER_YAMA_AREA_1,
    .demo_id = DemoId::TOWER_INTRO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MAP_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};

static EvdemoDefinition GREAT_FOREST_INTRO = {
    .flag = EventFlagId::GREAT_FOREST_INTRO_SEEN,
    .day_stage_id = StageId::GREAT_FOREST_D_BASE,
    .demo_id = DemoId::GREAT_FOREST_INTRO,
    .em_type = EmType::DUMMY_ID,
    .bgm_type = EvdemoBgmType::NONE,
    .bgm_id = FileId::DUMMY,
    .flags = EvdemoFlag::MAP_DEMO,
    .check = &Evdemo::check000,
    .event = &Evdemo::event000,
};
