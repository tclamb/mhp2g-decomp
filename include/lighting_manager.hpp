#pragma once

#include "singleton.hpp"
#include "character.hpp"

struct player;

struct light_data {
    ScePspFVector3 diffuse;
    ScePspFVector3 ambient;
    ScePspFVector4 position;
};

struct lighting_data {
    u8 unknown_0x0[0x10];
    light_data lights[3];
};

struct lighting_manager_data {
    u8 unknown_0x0[0x10];
    lighting_data lighting[2];
};

struct lighting_manager : singleton<lighting_manager> {
    lighting_manager_data data;
    u8 unknown_0x150[0xC];
    u16 unknown_0x15C;

    lighting_manager();
    ~lighting_manager();

    void method_088607F0();
    void method_08860840(light_data *, s16, u16);
    void method_088609FC(s16);
    void method_08860A70(character *);
    void method_08860B1C(player *);
    void method_08860C4C();
    ScePspUnion32 method_08860CA8(character *, int);
    void method_08860EB8(character *);
    void method_0886117C(s8);
};
