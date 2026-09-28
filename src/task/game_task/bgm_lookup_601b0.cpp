typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
template <typename T> struct Singleton { static T *objectPtr; };
struct Sound : Singleton<Sound> {};
struct BgmServer { u8 reserved[0x19]; u8 cue_19; u8 cue_1A; };
struct BgmStageEntry { u16 stage; u16 unused; u8 *cues; };
extern "C" BgmStageEntry D_game_sub_09CDFE80[];
extern "C" void func_eboot_0888368C(Sound *, u8, u8, s32, s32, s32, s32, s32, s32);

extern "C" void func_game_task_09A601B0(BgmServer *self, u16 stage) {
    BgmStageEntry *entry = D_game_sub_09CDFE80;
    while (true) {
        if (entry->stage == 0xFFFF) return;
        if (entry->stage == stage) {
            u8 *cue = entry->cues;
            while (true) {
                if (cue[0] == 0xFF) return;
                if (cue[0] == self->cue_1A && cue[1] == self->cue_19) {
                    func_eboot_0888368C(Sound::objectPtr, cue[2], cue[3], 0, 0xE0, 1, 0, 0, 0);
                    return;
                }
                cue += 4;
            }
        }
        ++entry;
    }
}
