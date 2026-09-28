#pragma once

#include "common.h"

// One on-screen notification popup ("toast"). Cockpit owns 20 of them
// (Cockpit::toasts at Cockpit+0xE28, 0x24 bytes each). The box is centered
// horizontally on x = 240, slides down from y = 0 to y = 136 - height / 2
// (vertical screen center) in about 3 frames, holds 60 frames and slides back up.
// Layout reads a separate data center (240, 144): x controls horizontal placement,
// while y computes the initial slide speed. Draw clamps to its own constant 136,
// so changing the data y alone does not move the resting toast vertically.
struct ToastNotification {
    u8 active;      // 0x00
    u8 type;        // 0x01: 0..8, selects the message format (4 and 6 = caller text, 6 = heap text freed on close)
    u8 state;       // 0x02: 0 layout, 1 draw/animate, 2 finished, 3 close
    u8 phase;       // 0x03: 0 slide in, 1 hold, 2 slide out
    float speed;      // 0x04: y pixels per frame (negated for the slide out)
    s16 param;      // 0x08: message argument (string id / count)
    s16 index;      // 0x0A: message table index
    s16 left;       // 0x0C: box left x = 240 - width / 2
    s16 width;      // 0x0E: box width (multiple of 16)
    float top;        // 0x10: animated box top y
    s16 height;     // 0x14: box height = 16 * lines + 32
    s16 textLeft;   // 0x16: text x
    s16 timer;      // 0x18: hold frames left (60)
    u8 pad_0x1A[0x20 - 0x1A];
    char *text;     // 0x20: caller text for types 4 and 6
};

extern "C" {
void func_eboot_08859C34(ToastNotification *this_); // layout: format text, measure it, size and place the box
void func_eboot_0885A0E8(ToastNotification *this_); // per-frame update, dispatches on state
void func_eboot_0885A16C(ToastNotification *this_); // draw box + text, animate
void func_eboot_0885A8D8(ToastNotification *this_); // state++
void func_eboot_0885A8E8(ToastNotification *this_); // close
void func_eboot_0885A92C(ToastNotification *this_, u8 type, s16 index, s16 param, u8 sysFlag);
void func_eboot_0885A954(ToastNotification *this_, char *text, u8 sysFlag, int heapText);
}
