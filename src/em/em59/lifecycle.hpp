#pragma once

#include "common.h"

struct Em59Actor {
    u32 *vtable;
    Em59Actor *previous;
    Em59Actor *next;
    u8 type;
    u8 mode;
    u8 unknown_0xE[0x58];
    u16 value_0x66;
};

struct Em59Data {
    u8 unknown_0x0[0x1E6];
    u16 value_0x1E6;
    u8 kind_0x1E8;
    u8 value_0x1E9;
    u8 unknown_0x1EA[0x6];
    u32 value_0x1F0;
    u32 value_0x1F4;
    u32 value_0x1F8;
    u8 unknown_0x1FC[0x128];
    u16 value_0x324;
    u8 unknown_0x326[0x17];
    u8 flag_0x33D;
    u8 unknown_0x33E[0xBA];
    u8 flag_0x3F8;
};

struct ShellManager {
    u8 unknown_0x0[0x502C];
    Em59Actor *head;
    Em59Actor *last;
    u32 count;
};

template <class T> struct Singleton {
    static T *objectPtr;
};

#define EM59_AT8(p, offset) (*(u8 *)((u8 *)(p) + (offset)))
#define EM59_AT16(p, offset) (*(u16 *)((u8 *)(p) + (offset)))
#define EM59_AT32(p, offset) (*(u32 *)((u8 *)(p) + (offset)))

extern "C" u32 D_eboot_089BA160[];
extern "C" void func_game_task_09B62320(Em59Actor *);
extern "C" void func_game_task_09B623B8(Em59Actor *, int);
extern "C" void func_game_task_09B62408(Em59Actor *);
