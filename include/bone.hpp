#pragma once

#include "common.h"

struct bind_pose {
    ScePspFMatrix4 transform;
    ScePspFVector3 scale;
    ScePspFVector3 rotation;
    ScePspFVector3 position;
    u16 f0x64[14];
};

struct bone {
    virtual ~bone();
    virtual void update(ScePspFMatrix4 *, ScePspFMatrix4 *, float x, float y, float z);

    ScePspFVector3 f0x4;
    ScePspFMatrix4 f0x10;
    ScePspFMatrix4 f0x50;
    ScePspFMatrix4 localPose; // these might be swapped
    ScePspFMatrix4 globalPose;
    u16 f0x110;
    u16 f0x112;
    u16 key;
    u16 f0x116;
    float f0x118;
    float f0x11c;
    ScePspFMatrix3 f0x120;
    bone *parent;
    bone *sibling;
    bone *child;
    bind_pose bind;
    bind_pose alt_bind;

    bone *func_eboot_0885F920(bone *arg1, u32 arg2);
    void func_eboot_0885FFB4(ScePspFVector4 *arg1, s32 arg2, float arg3, float arg4);

    static void operator delete(void *);
    static void *operator new(u32, void *);
};
