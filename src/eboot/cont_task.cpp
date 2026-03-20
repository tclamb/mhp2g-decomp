#include "cont_task.hpp"

#include "fade_task.hpp"
#include "system.hpp"

ContTask::ContTask() { status = 0; }

ContTask::~ContTask() {}

void ContTask::load() {
    switch (status) {
    case 0:
        func_eboot_0884D658(Singleton<FadeTask>::objectPtr, 1, 0, 1);
        func_eboot_0888FE4C(Singleton<System>::objectPtr, 0x2D, 1);
        status = (s32) (status + 1);
        return;
    case 1:
        if (func_eboot_0888FEE8(Singleton<System>::objectPtr) == 0) {
            func_eboot_0888FF00(Singleton<System>::objectPtr, 0x2D, 1);
            status = 0;
            TaskBase::mem_fn next = (TaskBase::mem_fn)&ContTask::on_load;
            if (next != 0) {
                action = next;
            }
        }
        return;
    default:
        return;
    }
}
