#include "stage_lifecycle.hpp"

extern "C" void func_game_task_09A5E1E0(GameTask *, s16 lighting) {
    func_eboot_088714F0(Quest::objectPtr);
    method_088138DC__6CameraFv(Camera::objectPtr);
    method_088609FC__12LightManagerFs(LightManager::objectPtr, lighting);
    StageBase *stage = StageManager::objectPtr->stage;
    emit_fog__9StageBaseFv(stage);
    stage = StageManager::objectPtr->stage;
    stage->vtable_0x24();
    func_eboot_0886D160(Quest::objectPtr);
}

extern "C" void func_game_task_09A5E260(GameTask *task) {
    func_game_task_09B61E28(ShellManager::objectPtr);
    func_eboot_088718EC(Quest::objectPtr);
    destroy_prop_list__12StageManagerFv(StageManager::objectPtr);
    if ((task->flags_54 & 8) == 0) {
        vram_clear__12StageManagerFv(StageManager::objectPtr);
    }
}
