#pragma once

#include "vtable_14_lifecycle.hpp"

struct Em75V07Actor {
    u32 *vtable;
};

struct Em75EffectObject {
    u32 unknown_0x0;
    Em75EffectObject *previous;
    Em75EffectObject *next;
    Em75V07Actor *owner;
};

struct EffectManager {
    u8 unknown_0x0[0x19030];
    Em75EffectObject *head;
    u32 count;
};

extern "C" u32 D_eboot_089BA390[];
extern "C" void func_game_sub_09C51DF0(Em75V07Actor *);
extern "C" void func_game_sub_09C51E10(Em75V07Actor *, int);
extern "C" void func_game_sub_09C51EC0(Em75V07Actor *);
extern "C" Em75EffectObject *func_game_sub_09C2CBA0(EffectManager *, u8);
extern "C" void func_game_sub_09C51EC8(Em75EffectObject *);
