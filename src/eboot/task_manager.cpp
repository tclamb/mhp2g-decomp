#include "task_manager.hpp"

#include "cont_task.hpp"
#include "fade_task.hpp"

extern "C" {
    void func_eboot_08895128(TaskManager *, u8, u32);
    int func_eboot_088950F0(TaskManager *, u8);

    // Task constructors that are still asm (see the factory below).
    TaskBase *func_eboot_0885A98C(TaskBase *);
    TaskBase *func_eboot_088BC8A4(TaskBase *);
    TaskBase *func_eboot_0887F168(TaskBase *);
    TaskBase *func_eboot_0884CF70(TaskBase *);
    TaskBase *func_eboot_088BB10C(TaskBase *);
    TaskBase *func_eboot_088BCB40(TaskBase *);
    TaskBase *func_eboot_088BCE54(TaskBase *);
    TaskBase *func_eboot_08906464(TaskBase *);
    TaskBase *func_eboot_088A9E70(TaskBase *);
    TaskBase *func_eboot_088BC554(TaskBase *);
    TaskBase *func_eboot_088CEEB0(TaskBase *);
    TaskBase *func_eboot_089092E4(TaskBase *);
}

template<> TaskManager *Singleton<TaskManager>::objectPtr;

// ptmf_eboot_089AA4C0 ({0, 0x10, 0} == &TaskBase::load) is the constant that the
// inline TaskBase ctor emits; the factory inlines it for FadeTask (case 12).

extern "C" void func_eboot_0889501C(TaskManager *this_) {
    for (int i = 0; i < 4; i++) {
        TaskBase *task = this_->active[i];
        if (task == NULL) {
            continue;
        }
        if (task->load_status != 1) {
            task->update();
        }
        if (task->load_status == 1) {
            u32 next = task->next_id;
            delete this_->active[i];
            this_->cache.free(this_->active[i]);
            this_->active[i] = NULL;
            if (next) {
                func_eboot_08895128(this_, i, next);
            }
        }
    }
}

extern "C" int func_eboot_088950F0(TaskManager *this_, u8 slot) {
    if (slot < 4 && this_->active[slot]) {
        return true;
    }
    return false;
}

// Placement-new a task of `size` bytes into the slab through its (asm) ctor.
#define CREATE_TASK(size, ctor)                                           \
    if (slot < 4) {                                                       \
        void *p = this_->cache.alloc(size, 0x10);                         \
        if (p) {                                                          \
            TaskBase *task = (TaskBase *)TaskBase::operator new(size, p); \
            if (task) {                                                   \
                task = ctor(task);                                        \
            }                                                             \
            this_->active[slot] = task;                                   \
            if (task) {                                                   \
                task->overlay_group = slot;                               \
                this_->active[slot]->id = id;                             \
                TaskBase *active = this_->active[slot];                   \
                active->load_status = 2;                                  \
                active->load_delay = 0;                                   \
            }                                                             \
        }                                                                 \
    }                                                                     \
    break;

// Same for the task classes known in C++ (new expression).
#define CREATE_T(T, DECL)                                                 \
    if (slot < 4) {                                                       \
        void *p = this_->cache.alloc(sizeof(T), 0x10);                    \
        if (p) {                                                          \
            DECL task = new (p) T();                                      \
            this_->active[slot] = task;                                   \
            if (task) {                                                   \
                task->overlay_group = slot;                               \
                this_->active[slot]->id = id;                             \
                TaskBase *active = this_->active[slot];                   \
                active->load_status = 2;                                  \
                active->load_delay = 0;                                   \
            }                                                             \
        }                                                                 \
    }                                                                     \
    break;

// Task factory: create task `id` in `slot` (0..3). Called at the end of a frame
// by func_eboot_0889501C when a task finished with next_id != 0, and by System
// init for the first task. New tasks start with load_status 2 (overlay loading).
extern "C" void func_eboot_08895128(TaskManager *this_, u8 slot, u32 id) {
    if ((u8)func_eboot_088950F0(this_, slot)) {
        return;
    }
    if (id == 0) {
        return;
    }
    switch (id) {
    case 1: CREATE_TASK(0x20, func_eboot_0885A98C)    // start task
    case 2: CREATE_TASK(0x2C, func_eboot_088BC8A4)
    case 3: CREATE_TASK(0x24, func_eboot_0887F168)
    case 4: CREATE_TASK(0xA0, func_eboot_0884CF70)
    case 5: CREATE_T(ContTask, TaskBase *)
    case 6: CREATE_TASK(0x38, func_eboot_088BB10C)
    case 7: CREATE_TASK(0x64, func_eboot_088BCB40)
    case 8: CREATE_TASK(0x68, func_eboot_088BCE54)
    case 9: CREATE_TASK(0x129B0, func_eboot_08906464) // largest, the in-quest game task
    case 10: CREATE_TASK(0x78, func_eboot_088A9E70)
    case 11: CREATE_TASK(0x70, func_eboot_088BC554)
    case 12: CREATE_T(FadeTask, FadeTask *)
    case 13: CREATE_TASK(0x218, func_eboot_088CEEB0)
    case 14: CREATE_TASK(0x20, func_eboot_089092E4)
    }
}
