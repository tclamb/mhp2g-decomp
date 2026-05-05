#pragma once

#include "common.h"
#include "singleton.hpp"

struct Ctrl {
    enum {
        SELECT       = 0x1,
        START        = 0x8,
        UP           = 0x10,
        RIGHT        = 0x20,
        DOWN         = 0x40,
        LEFT         = 0x80,
        L_TRIGGER    = 0x100,
        R_TRIGGER    = 0x200,
        TRIANGLE     = 0x1000,
        CIRCLE       = 0x2000,
        CROSS        = 0x4000,
        SQUARE       = 0x8000,
        ANALOG_UP    = 0x100000,
        ANALOG_RIGHT = 0x200000,
        ANALOG_DOWN  = 0x400000,
        ANALOG_LEFT  = 0x800000,
    };

private:
    Ctrl();
};

struct Pad : Singleton<Pad> {
    static u32 BUTTONS;
    static u32 RISING_EDGE;
    static u32 FALLING_EDGE;
    static u32 UI_BUTTONS;
    static u16 ANALOG_ANGLE;
    static u16 ANALOG_MAGNITUDE;

    struct {
        u16 buttons;
        s16 analogX;
        s16 analogY;
        s16 magnitude;
        s16 degrees;
        float radians;
    } ctrl;
    u32 lastButtons;
    u8 heldFrames;
    bool holdButton;
    bool homeButton;
    bool inputDisabled;

    Pad();

    void initialize();
    void update();
};
