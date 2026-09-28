#pragma once

#include "common.h"
#include "cache.hpp"

// Shells: projectiles (bowgun shots, arrows, monster projectiles) owned by ShellManager.
struct Shell {
    virtual ~Shell();
    virtual void update();                        // vtable +0x0C
    virtual void vtable_0x10();
    virtual void register_draw(s32 game_state);   // vtable +0x14
    Shell *next;
    Shell *prev;

    static void operator delete(void *) {}
};

struct ShellManagerView {
    u8 pad[0x5000];
    cache shell_cache;   // +0x5000
    u8 pad_501C[0x502C - 0x5000 - sizeof(cache)];
    Shell *head;         // +0x502C
    Shell *tail;         // +0x5030
    s32 count;           // +0x5034
};
