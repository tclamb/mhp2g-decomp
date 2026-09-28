#include "manager_pass.hpp"

extern "C" void func_game_task_09A5D9B0(GameTask *task) {
    func_game_task_09A5E6C8(task);
    if ((u8)func_eboot_088566DC(GameSys::objectPtr) == 0) {
        func_game_task_09A5DA28(task);
        func_game_sub_09C3CD68();
    } else {
        func_game_task_09A5DB38(task);
        func_eboot_08856488(GameSys::objectPtr);
    }
    func_game_task_09A5DB68(task);
}
