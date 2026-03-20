#pragma once

#include "common.h"

struct TaskBase {
    typedef void (TaskBase::*mem_fn)();

    inline TaskBase() { set_action(&TaskBase::load); }
    inline TaskBase(mem_fn action) { set_action(action); };

    virtual ~TaskBase() {};
    virtual void update();
    virtual void load() = 0;

    mem_fn action;
    u8 overlay_group;
    u8 load_status;
    s8 load_delay;
    u32 id;
    u32 next_id;

    static void *operator new(u32, void *);
    static void operator delete(void *);

protected:
    inline void set_action(mem_fn new_action) {
        if (new_action != 0) {
            action = new_action;
        }
    }

private:
    s32 is_overlay_loaded();
};
