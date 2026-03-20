#include "task_base.hpp"

void TaskBase::update() {
    if ((u8)is_overlay_loaded() != true) {
        return;
    }
    if (action) {
        (this->*action)();
    }
}

#define STAGE(status) ((status) & ~0x80)
#define FLAG(status) ((status) & 0x80)
#define SET_STAGE(status, n) \
    do {              \
        status &= 0x80; \
        status |= n;    \
    } while (0)

s32 TaskBase::is_overlay_loaded() {
    s32 active = 0;
    switch (STAGE(load_status)) {
    case 1:
        load_status = 1;
        return 0;
    case 4:
        if (!FLAG(load_status)) {
            if (load_delay-- <= 0) {
                load_delay = 0;
                load_status = 3;
                return 1;
            }
        }
        break;
    case 2:
        SET_STAGE(load_status, 3);
        // fallthrough
    case 3:
        if (!FLAG(load_status)) {
            active = 1;
        }
        break;
    case 0:
        break;
    default:
        break;
    }
    return active;
}

void *TaskBase::operator new(u32, void *p) {
    return p;
}

void TaskBase::operator delete(void *) {
    // empty
}
