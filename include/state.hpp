#pragma once

#include "common.h"

struct base_state {
    typedef void (base_state::*mem_fn)();

    inline base_state() { set_action(&base_state::load); }
    inline base_state(mem_fn action) { set_action(action); };

    virtual ~base_state() {};
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

struct cont_state : base_state {
    cont_state();
    virtual ~cont_state();
    virtual void load();

    u32 status;

    void on_load();
};
