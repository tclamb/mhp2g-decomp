#pragma once

#include "common.h"

struct Em75V14Actor {
    u32 *vtable;
    Em75V14Actor *previous;
    Em75V14Actor *next;
};

struct ShellManager {
    u8 unknown_0x0[0x502C];
    Em75V14Actor *head;
    Em75V14Actor *last;
    u32 count;
};

template <class T> struct Singleton {
    static T *objectPtr;
};

#define EM75_AT8(p, offset) (*(u8 *)((u8 *)(p) + (offset)))
#define EM75_AT16(p, offset) (*(u16 *)((u8 *)(p) + (offset)))
#define EM75_AT32(p, offset) (*(u32 *)((u8 *)(p) + (offset)))
#define EM75_ATF(p, offset) (*(float *)((u8 *)(p) + (offset)))

extern "C" u32 D_eboot_089BA4C4[];
extern "C" void func_game_task_09B62320(Em75V14Actor *);
extern "C" void func_game_task_09B623B8(Em75V14Actor *, int);
extern "C" void func_game_task_09B62408(Em75V14Actor *);
