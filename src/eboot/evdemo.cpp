#include "evdemo.hpp"

#include "bgm_server.hpp"
#include "camera.hpp"
#include "cockpit.hpp"
#include "data_manager.hpp"
#include "enemy.hpp"
#include "enemy_manager.hpp"
#include "fade_task.hpp"
#include "game_sys.hpp"
#include "gamestage.hpp"
#include "pad.hpp"
#include "player_manager.hpp"
#include "quest.hpp"
#include "quest_net.hpp"
#include "sound.hpp"
#include "stage_manager.hpp"
#include "stream.hpp"
#include "system.hpp"

template<> Evdemo *Singleton<Evdemo>::objectPtr;

#include "evdemo.definitions.inc.cpp"

void Evdemo::EvDemoInitialize() {
    GameSys::objectPtr->is_demo = false;
    is_playing = 0;
    is_requested = 0;
    demos[0].u = 0;
    demos[1].u = 0;
    demos[2].u = 0;
    demos[3].u = 0;
    pl_skip_mask = 0;
    is_skipping = 0;
    if (dragon_quest_chk() != 0) {
        return;
    }
    map_demo_chk();
    switch (GameSys::objectPtr->mission_id) {
    case 20001:
        ev_demo_init_sub(&demos[0], &TRAINING_BASICS_DEMO);
        break;
    case 20003:
        ev_demo_init_sub(&demos[0], &TRAINING_GATHERING_DEMO_1);
        ev_demo_init_sub(&demos[1], &TRAINING_GATHERING_DEMO_2);
        ev_demo_init_sub(&demos[2], &TRAINING_GATHERING_DEMO_3);
        ev_demo_init_sub(&demos[3], &TRAINING_GATHERING_DEMO_4);
        break;
    case 20004:
        ev_demo_init_sub(&demos[0], &TRAINING_COMBINING_DEMO);
        break;
    case 20005:
        ev_demo_init_sub(&demos[0], &TRAINING_FISHING_DEMO);
        break;
    case 20006:
        ev_demo_init_sub(&demos[0], &TRAINING_TRANSPORT_DEMO);
        break;
    case 10105:
        ev_demo_init_sub(&demos[0], &RECKLESS_BULLDROME_HUNTER_DEMO);
        break;
    case 10109:
        ev_demo_init_sub(&demos[0], &HUNT_DOWN_THE_VELOCIDROME_DEMO);
        break;
    case 10104:
        ev_demo_init_sub(&demos[0], &THE_CARNIVOROUS_LEADER_DEMO);
        break;
    case 10110:
        ev_demo_init_sub(&demos[0], &JUNGLE_MENACE_DEMO);
        break;
    case 10116:
        ev_demo_init_sub(&demos[0], &THE_LAND_SHARK_DEMO);
        break;
    case 10213:
        ev_demo_init_sub(&demos[0], &GYPCEROS_VENOMOUS_TERROR_DEMO);
        break;
    case 10210:
        ev_demo_init_sub(&demos[0], &THE_LURKING_DESERT_GIANT_DEMO);
        break;
    case 10111:
        ev_demo_init_sub(&demos[0], &RAREST_OF_THE_RARE_BEASTS_DEMO);
        break;
    case 10318:
        ev_demo_init_sub(&demos[0], &THE_ELDER_DRAGON_OF_WIND_DEMO);
        break;
    case 10203:
        ev_demo_init_sub(&demos[0], &SHADOW_IN_THE_SNOW_DEMO);
        break;
    case 10302:
        ev_demo_init_sub(&demos[0], &THE_RULER_OF_THE_SNOW_DEMO);
        break;
    case 10211:
        ev_demo_init_sub(&demos[0], &WATER_WYVERN_IN_THE_DESERT_DEMO);
        break;
    case 10313:
        ev_demo_init_sub(&demos[0], &COMMANDER_IN_THE_FLAMES_DEMO);
        break;
    case 10406:
        ev_demo_init_sub(&demos[0], &THE_RUNAWAY_DIABLOS_DEMO);
        break;
    case 10317:
        ev_demo_init_sub(&demos[0], &THE_FROZEN_DICTATOR_DEMO);
        break;
    case 10413:
        ev_demo_init_sub(&demos[0], &THE_EMPRESS_S_BLAZING_THRONE_DEMO);
        break;
    case 10506:
        ev_demo_init_sub(&demos[0], &EMPEROR_OF_FLAME_DEMO);
        break;
    case 10411:
        ev_demo_init_sub(&demos[0], &TERROR_OF_THE_GRAVIOS_DEMO);
        break;
    case 10414:
        ev_demo_init_sub(&demos[0], &OVERSEER_OF_THE_ANCIENTS_DEMO);
        break;
    case 10402:
        ev_demo_init_sub(&demos[0], &THE_LEGENDARY_KIRIN_DEMO);
        break;
    case 10307:
        ev_demo_init_sub(&demos[0], &BATTLE_OF_THE_BLOS_DEMO);
        break;
    case 10415:
        ev_demo_init_sub(&demos[0], &THE_ELDER_DRAGON_OF_MIST_DEMO);
        break;
    case 10510:
        ev_demo_init_sub(&demos[0], &THE_FINAL_INVITATION_DEMO);
        break;
    case 10304:
        ev_demo_init_sub(&demos[0], &THE_LONE_BLACK_GARUGA_DEMO);
        break;
    case 10004:
        ev_demo_init_sub(&demos[0], &SINKING_FEELING_DEMO);
        break;
    case 10401:
        ev_demo_init_sub(&demos[0], &ABSOLUTE_POWER_DEMO);
        break;
    case 50002:
        ev_demo_init_sub(&demos[0], &LAVASIOTH_INTRO);
        break;
    case 11005:
        ev_demo_init_sub(&demos[0], &HYPNOCATRICE_INTRO);
        break;
    case 11201:
        ev_demo_init_sub(&demos[0], &NARGACUGA_INTRO);
        break;
    }
}

int Evdemo::ev_demo_init_sub(EvdemoData *data, EvdemoDefinition *definition) {
    if (GameSys::objectPtr->Event_flag_ck(definition->flag) == false) {
        data->uc.is_present = 1;
        data->definition = definition;
        return 0;
    }
    return -1;
}

u16 Evdemo::dragon_quest_chk() {
    is_dragon_demo = 1;
    u8 emTypes[4];
    s32 emCount = Quest::objectPtr->Quest_em_serch(emTypes);
    if (emCount == 0) {
        is_dragon_demo = 0;
    }
    for (int i = 0; i < emCount; ++i) {
        switch (emTypes[i]) {
        case EmType::LAOSHAN_LUNG:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::LAOSHAN_LUNG_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &LAOSHAN_LUNG_INTRO);
            return EmType::LAOSHAN_LUNG;
        case EmType::ASHEN_LAOSHAN_LUNG:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::ASHEN_LAOSHAN_LUNG_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &ASHEN_LAOSHAN_LUNG_INTRO);
            return EmType::ASHEN_LAOSHAN_LUNG;
        case EmType::FATALIS:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::FATALIS_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &FATALIS_INTRO);
            return EmType::FATALIS;
        case EmType::CRIMSON_FATALIS:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::CRIMSON_FATALIS_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &CRIMSON_FATALIS_INTRO);
            return EmType::CRIMSON_FATALIS;
        case EmType::WHITE_FATALIS:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::WHITE_FATALIS_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &WHITE_FATALIS_INTRO);
            return EmType::WHITE_FATALIS;
        case EmType::SHEN_GAOREN:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::SHEN_GAOREN_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &SHEN_GAOREN_INTRO);
            return EmType::SHEN_GAOREN;
        case EmType::AKANTOR:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::AKANTOR_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &AKANTOR_INTRO);
            return EmType::AKANTOR;
        case EmType::YAMA_TSUKAMI_1:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::YAMA_TSUKAMI_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &YAMA_TSUKAMI_INTRO);
            return EmType::YAMA_TSUKAMI_1;
        case EmType::UKANLOS:
            GameSys::objectPtr->Event_flag_clear(EventFlagId::UKANLOS_INTRO_SEEN);
            ev_demo_init_sub(&demos[0], &UKANLOS_INTRO);
            return EmType::UKANLOS;
        }
        is_dragon_demo = 0;
    }
    return 0;
}

int Evdemo::map_demo_chk() {
    if ((bool)(System::objectPtr->wirelessIsOn & 1) == true) {
        return 0;
    }
    if ((bool)(GameSys::objectPtr->gameSysFlags & GameSysFlag::BEGINNER_EXERCISES) == true) {
        return 0;
    }
    if ((bool)(GameSys::objectPtr->gameSysFlags & GameSysFlag::TRAINING_SCHOOL) == true) {
        return 0;
    }
    Quest *quest = Quest::objectPtr;
    int value;
    if (GameSys::objectPtr->is_gallery_task != false) {
        value = 0;
    } else {
        value = *(s8 *)((u8 *)quest->field_0x4C + 0x45);
    }
    if (value != 0) {
        return 0;
    }
    u8 map_id = GameSys::objectPtr->map_id;
    u16 map_type = Gamestage::Map_night_to_day(map_id);
    switch (map_type) {
    case MapId::FOREST_AND_HILLS_D:
        ev_demo_init_sub(&demos[3], &FOREST_AND_HILLS_INTRO);
        break;
    case MapId::DESERT_D:
        ev_demo_init_sub(&demos[3], &DESERT_INTRO);
        break;
    case MapId::SWAMP_D:
        ev_demo_init_sub(&demos[3], &SWAMP_INTRO);
        break;
    case MapId::VOLCANO_D:
        ev_demo_init_sub(&demos[3], &VOLCANO_INTRO);
        break;
    case MapId::JUNGLE_D:
        ev_demo_init_sub(&demos[3], &JUNGLE_INTRO);
        break;
    case MapId::SNOWY_MOUNTAINS_D:
        ev_demo_init_sub(&demos[3], &SNOWY_MOUNTAINS_INTRO);
        break;
    case MapId::TOWER_YAMA:
    case MapId::TOWER_FATALIS:
        ev_demo_init_sub(&demos[3], &TOWER_INTRO);
        break;
    case MapId::GREAT_FOREST_D:
        ev_demo_init_sub(&demos[3], &GREAT_FOREST_INTRO);
        break;
    }
    return 0;
}

void Evdemo::EvDemoMove() {
    switch (is_playing) {
    case 1:
        if (event000(&demos[slot]) == 0) {
            return;
        }
        GameSys::objectPtr->is_demo = false;
        is_requested = 0;
        is_playing = 0;
    case 0:
        if (is_requested != 0) {
            EvdemoData *data = (EvdemoData *)&demos[slot];
            is_playing = 1;
            event000(data);
        } else {
            func_eboot_08895A20(Cockpit::objectPtr);
            return;
        }
        break;
    default:
        break;
    }
}

int Evdemo::check000(EvdemoData *data) {
    if (GameSys::objectPtr->Game_clear_ck(0) == true) {
        return -1;
    } else {
        if (GameSys::objectPtr->Event_flag_ck(data->definition->flag) == true) {
            data->u = 0;
            return -1;
        } else {
            u16 stage = Gamestage::stage_night_to_day(GameSys::objectPtr->stage_id);
            if (stage != data->definition->day_stage_id) {
                return 0;
            }
            return 1;
        }
    }
}

int Evdemo::event000(EvdemoData *data) {
    switch (data->uc.state) {
    case 0:
        GameSys::objectPtr->is_demo = true;
        is_requested = 1;
        evdemo_camera_request(data->definition);
        if (data->definition->bgm_id != 0) {
            func_eboot_08882B28(Sound::objectPtr, 0, data->definition->bgm_id, 0x7F);
            func_eboot_08885D78(BgmServer::objectPtr, data->definition->bgm_id);
        }
        FadeTask::objectPtr->fadeBlack(1, true);
        fade_sub(data->definition);
        is_bgm_loading = 1;
        data->uc.state++;
        break;
    case 1:
        FadeTask::objectPtr->fadeBlack(0x10, false);
        data->uc.state++;
        // fallthrough
    case 2:
        sceKernelPowerTick(0);
        if (demo_skip_chk() == true) {
            if (is_skipping != false) {
                demo_finish(data);
                GameSys::objectPtr->flag_0x6AF1B = 1;
                FadeTask::objectPtr->fadeBlack(1, true);
                func_eboot_08885574(Sound::objectPtr, 0x10, 0);
                sound_stop_timer = 0x10;
                data->uc.state = 10;
            } else {
                if (demo_finish(data) != false) {
                    return 1;
                }
                data->uc.state++;
            }
        } else {
            fade_sub(data->definition);
        }
        break;
    case 3:
        Camera::objectPtr->DemoCameraCancel();
        FadeTask::objectPtr->fadeBlack(1, false);
        data->u = 0;
        return 1;
    case 10:
        if (--sound_stop_timer >= 0) {
            break;
        }
        func_eboot_08885234(Sound::objectPtr, 0);
        func_eboot_08882C5C(Sound::objectPtr, 0);
        data->uc.state++;
        break;
    case 11:
        if (GameSys::objectPtr->is_gallery_task != false) {
            break;
        }
        void *buffer = DataManager::objectPtr->getAdrs(BGM);
        if (buffer != NULL) {
            load_demo_bgm();
            data->uc.state++;
        }
        break;
    case 12:
        GameSys::objectPtr->flag_0x6AF1B = 0;
        FadeTask::objectPtr->fadeBlack(0x10, false);
        func_eboot_08885420(Sound::objectPtr, 0x10, 0);
        u16 bgm = BgmServer::objectPtr->unknown_0x28;
        u32 volume = 0x7F;
        if (bgm == FileId::MT_CP121_AT3) {
            StageBgm *normal = &Gamestage::Snd_bgm_tbl[StageManager::objectPtr->stage_id];
            volume = normal->volume;
        }
        func_eboot_08882B28(Sound::objectPtr, 0, bgm, volume);
        Camera::objectPtr->DemoCameraCancel();
        data->u = 0;
        return 1;
    }
    if (GameSys::objectPtr->is_gallery_task == false && is_dragon_demo != false && is_bgm_loading != false && !FileSys::objectPtr->is_loading()) {
        u16 file_id;
        if ((DataManager::objectPtr->entries[BGM].status & (DataManager::LOADING | DataManager::ENABLE)) != 0) {
            file_id = DataManager::objectPtr->entries[BGM].file_id;
        } else {
            file_id = FileId::INVALID_ID;
        }
        if (file_id == FileId::INVALID_ID) {
            int demo_id;
            EvdemoDefinition *definition = Evdemo::objectPtr->demos[0].definition;
            if (definition != NULL) {
                demo_id = definition->demo_id;
            } else {
                demo_id = DemoId::NONE;
            }
            if (demo_id == DemoId::YAMA_TSUKAMI_INTRO) {
                DataManager::objectPtr->load(BGM, FileId::MT_CP121_AT3, 0);
            } else {
                int fight_bgm_file_id = func_game_task_09A5FBF0(BgmServer::objectPtr);
                DataManager::objectPtr->load(BGM, fight_bgm_file_id, 0);
            }
        }
    }
    return 0;
}

int Evdemo::demo_finish(EvdemoData *data) {
    func_eboot_08885D9C(BgmServer::objectPtr, 0);
    is_requested = 0;
    GameSys::objectPtr->is_demo = false;
    GameSys::objectPtr->Event_flag_set(data->definition->flag);
    if (GameSys::objectPtr->is_gallery_task != false) {
        return 1;
    }
    switch (data->definition->bgm_type) {
    case EvdemoBgmType::DEFAULT:
        func_eboot_08882C5C(Sound::objectPtr, 0);
        func_game_task_09A60190(BgmServer::objectPtr);
        break;
    case EvdemoBgmType::LAOSHAN_LUNG:
    case EvdemoBgmType::FATALIS:
    case EvdemoBgmType::ELDER_DRAGON:
    case EvdemoBgmType::SHEN_GAOREN:
    case EvdemoBgmType::CRIMSON_FATALIS:
        func_game_task_09A60148(BgmServer::objectPtr, 1);
        break;
    case EvdemoBgmType::YAMA_TSUKAMI:
        if (is_skipping == 0) {
            func_eboot_08882C5C(Sound::objectPtr, 0);
            if (GameSys::objectPtr->is_gallery_task == false) {
                u8 *buffer = DataManager::objectPtr->getAdrs(BGM);
                if (buffer != 0) {
                    load_demo_bgm();
                }
            }
            func_game_task_09A60148(BgmServer::objectPtr, 2);
        }
        break;
    case EvdemoBgmType::UNUSED_4:
    case EvdemoBgmType::UNUSED_5:
    case EvdemoBgmType::UNUSED_6:
    case EvdemoBgmType::UNUSED_7:
    default:
        break;
    }
    PlayerManager::objectPtr->demoOff();
    return 0;
}

void Evdemo::evdemo_camera_request(EvdemoDefinition *definition) {
    if (definition->em_type == EmType::DUMMY_ID) {
        Camera::objectPtr->DemoCameraCancel();
        Camera::objectPtr->DemoCameraRequest(definition->demo_id, NULL, false);
        return;
    }
    for (u32 i = 0; i < 20; i++) {
        Enemy *enemy = EnemyManager::objectPtr->by_index((int)i);
        if (enemy == NULL) {
            continue;
        }
        if ((bool)(enemy->drawFlags & DrawFlag::ACTIVE) != false
            && GameSys::objectPtr->em_moto_type_ck(enemy->pl_type) == definition->em_type) {
            Camera::objectPtr->DemoCameraCancel();
            Camera::objectPtr->DemoCameraRequest(definition->demo_id, enemy, false);
            return;
        }
    }
}

bool Evdemo::demo_skip_chk() {
    if (Camera::objectPtr->DemoCameraCheck() <= 0) {
        return true;
    }
    bool result = false;
    if (is_dragon_demo != false) {
        if (!System::objectPtr->Online_ck()) {
            if ((Pad::RISING_EDGE & Ctrl::SELECT) != 0) {
                if (GameSys::objectPtr->is_gallery_task == false) {
                    is_skipping = true;
                    return true;
                }
            }
        } else {
            if ((pl_skip_mask & (1 << GameSys::objectPtr->player_id)) == 0 && (Pad::RISING_EDGE & Ctrl::SELECT) != 0) {
                pl_skip_mask |= (1 << GameSys::objectPtr->player_id);
                func_eboot_088AE398(QuestNet::objectPtr, 0xD, 0);
            }
            result = true;
            for (int i = 0; i < 4; i++) {
                if ((bool)GameSys::objectPtr->playerModels[(u8)i].active != false) {
                    if ((pl_skip_mask & (1 << i)) == 0) {
                        result = false;
                    }
                }
            }
            if (result == true) {
                is_skipping = true;
            }
            return result;
        }
    }
    return result;
}

void Evdemo::start_next_demo() {
    EvdemoData *d = &demos[0];
    for (int i = 0; i < 4; i++, d++) {
        if (d->uc.is_present == 0) {
            continue;
        }
        if (check000(d) > 0) {
            is_requested = 1;
            slot = i;
            Camera::objectPtr->DemoCameraRequest(DemoId::CLEAR_DEMO_STATE, NULL, false);
            return;
        }
    }
    is_requested = 0;
}

u16 Evdemo::bgm_id() {
    EvdemoData *d = &demos[slot];
    u16 id = d->definition->bgm_id;
    if (id != FileId::DUMMY) {
        return id;
    }
    return FileId::INVALID_ID;
}

u8 Evdemo::bgm_type() {
    EvdemoData *d = &demos[slot];
    return d->definition->bgm_type;
}

int Evdemo::event_flag_id() {
    if (is_playing == 0) {
        return -1;
    }
    EvdemoData *d = &demos[slot];
    return d->definition->flag;
}

bool Evdemo::Em_demo_chk() {
    if (is_playing != 0) {
        EvdemoData *d = &demos[slot];
        return d->definition->flags & EvdemoFlag::EM_DEMO;
    }
    return false;
}

int Evdemo::em_demo_ready_chk() {
    EvdemoData *d = &demos[0];
    for (int i = 0; i < 4; i++, d++) {
        if (d->uc.is_present == 0) {
            continue;
        }
        if ((d->definition->flags & EvdemoFlag::EM_DEMO) != 0 && check000(d) > 0) {
            return 1;
        }
    }
    return 0;
}

void Evdemo::load_demo_bgm() {
    u32 bgm = BgmServer::objectPtr->unknown_0x28;
    u32 size = FileSys::objectPtr->file_size(bgm);
    func_eboot_088814EC(Sound::objectPtr, 0x12, size);
    func_eboot_088814EC(Sound::objectPtr, 0x13, 0);
    func_eboot_088814EC(Sound::objectPtr, 0x14, 0);
    for (int i = 0; i < 3; i++) {
        func_eboot_08886350(Stream::objectPtr, i + 0x12);
    }
    func_eboot_08882620(Sound::objectPtr, 2);
    u32 size2 = FileSys::objectPtr->file_size((u16)bgm);
    func_eboot_0888633C(Stream::objectPtr, 0x12, (u16)bgm, size2);
    void *p = func_eboot_08886368(Stream::objectPtr, 0x12);
    DataManager::objectPtr->copy(p, BGM, -1);
    DataManager::objectPtr->free(BGM);
    is_bgm_loading = 0;
}

bool Evdemo::stage_se_mute_chk(u32 tsb_id, bool is_update) {
    if (is_requested != 0) {
        if (tsb_id >= 0x78) {
            return true;
        }
        if (!is_update) {
            switch (GameSys::objectPtr->stage_id) {
            case StageId::JUNGLE_D_AREA_8:
            case StageId::JUNGLE_N_BASE:
            case StageId::JUNGLE_D_BASE:
                return true;
            }
        }
    }
    return false;
}

void Evdemo::fade_sub(EvdemoDefinition *definition) {
    int t = Camera::objectPtr->GetDemoCameraTimer();
    if (t < 0) {
        return;
    }
    FadeCue *cmd = definition->fades;
    if (cmd == 0) {
        return;
    }
    while (cmd->time != -1) {
        if (cmd->time > t) {
            break;
        }
        if (cmd->time == t) {
            fade_sub_sub(cmd);
        }
        cmd++;
    }
}

void Evdemo::fade_sub_sub(FadeCue *fade) {
    switch (fade->type) {
    case FadeCueType::NONE:
        break;
    case FadeCueType::FADE_IN:
        FadeTask::objectPtr->fadeBlack(fade->duration, true);
        break;
    case FadeCueType::FADE_TO_BLACK:
        FadeTask::objectPtr->fadeBlack(fade->duration, false);
        break;
    }
}
