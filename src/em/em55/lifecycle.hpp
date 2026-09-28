#pragma once

#include "common.h"

struct Em55Actor {
    u32 *vtable;
    Em55Actor *previous;
    Em55Actor *next;
    u8 type;
    u8 mode;
};

struct ShellManager {
    u8 unknown_0x0[0x502C];
    Em55Actor *head;
    Em55Actor *last;
    u32 count;
};

template <class T> struct Singleton {
    static T *objectPtr;
};

#define EM55_AT8(p, offset) (*(u8 *)((u8 *)(p) + (offset)))
#define EM55_AT16(p, offset) (*(u16 *)((u8 *)(p) + (offset)))
#define EM55_AT32(p, offset) (*(u32 *)((u8 *)(p) + (offset)))
#define EM55_ATF(p, offset) (*(float *)((u8 *)(p) + (offset)))

extern "C" u32 D_eboot_089B9E90[];
extern "C" void func_game_task_09B62320(Em55Actor *);
extern "C" void func_game_task_09B623B8(Em55Actor *, int);
extern "C" void func_game_task_09B62408(Em55Actor *);
