#include "pad.hpp"

#include <pspsdk/pspctrl.h>
#include "vfpu.h"

#define CTRL_MODE_ANALOG 1

template<> Pad *Singleton<Pad>::objectPtr;
u32 Pad::BUTTONS;
u32 Pad::RISING_EDGE;
u32 Pad::FALLING_EDGE;
u32 Pad::UI_BUTTONS;
u16 Pad::ANALOG_ANGLE;
u16 Pad::ANALOG_MAGNITUDE;

void Pad::initialize() {
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(CTRL_MODE_ANALOG);
    lastButtons = 0;
    heldFrames = 0;
    holdButton = 0;
    homeButton = 0;
    inputDisabled = 0;
}

#define CTRL_HOME 0x00010000
#define CTRL_HOLD 0x00020000

inline s32 abs(s32 x) {
    if ((int)x < 0) {
        x = -x;
    }
    return (u32)x;
}

void Pad::update() {
    SceCtrlData sample;
    int sampleCount = sceCtrlReadBufferPositive(&sample, 1);
    if (sampleCount > 0) {
        if ((sample.Buttons & CTRL_HOME) != 0) {
            homeButton = true;
        } else {
            homeButton = false;
        }
        if ((sample.Buttons & CTRL_HOLD) != 0) {
            holdButton = true;
        } else {
            holdButton = false;
        }
    }

    if (inputDisabled == true || holdButton == true || sampleCount < 1) {
        memset(&ctrl, 0, sizeof(ctrl));
    } else {
        ctrl.buttons = (u16) sample.Buttons;
        ctrl.analogX = sample.Lx - 0x80;
        if (ctrl.analogX < -0x7F) {
            ctrl.analogX = -0x7F;
        }
        ctrl.analogY = sample.Ly - 0x80;
        if (ctrl.analogY < -0x7F) {
            ctrl.analogY = -0x7F;
        }
    }

    float rads;
    if ((ctrl.analogX | ctrl.analogY) == 0) {
        rads = 0.0f;
    } else {
        rads = atan2f_s(-ctrl.analogY, ctrl.analogX);
        if (rads < 0.0f) {
            rads += 6.2831855f;
        }
    }
    ctrl.radians = rads;
    ctrl.degrees = (ctrl.radians / 6.2831855f) * 360.0f;

    s32 max = abs(ctrl.analogX), min = abs(ctrl.analogY);
    if (max < min) {
        max ^= min ^= max ^= min;
    }
    // linear approximation of mag(x + I y)
    // after folding complex plane onto first octant
    // mag(x + I y) ~ A * max(|x|, |y|) + B * min(|x|, |y|)
    // with A = 123 / 128 and B = 51 / 128
    // max relative error is around 3.4%
    min = min + (min >> 1);
    ctrl.magnitude = (min >> 6) + ((min >> 2) + ((max - (max >> 5)) - (max >> 7)));
    if (ctrl.magnitude > 0x7F) {
        ctrl.magnitude = 0x7F;
    }

    static u16 ANALOG_DIRECTIONS[16] = {
        Ctrl::RIGHT, Ctrl::RIGHT | Ctrl::UP, Ctrl::RIGHT | Ctrl::UP, Ctrl::UP,
        Ctrl::UP, Ctrl::UP | Ctrl::LEFT, Ctrl::UP | Ctrl::LEFT, Ctrl::LEFT,
        Ctrl::LEFT, Ctrl::LEFT | Ctrl::DOWN, Ctrl::LEFT | Ctrl::DOWN, Ctrl::DOWN,
        Ctrl::DOWN, Ctrl::DOWN | Ctrl::RIGHT, Ctrl::DOWN | Ctrl::RIGHT, Ctrl::RIGHT,
    };
    u16 analogDirection;
    if (sample.Lx < 0x40 || sample.Lx > 0xC0 || sample.Ly < 0x40 || sample.Ly > 0xC0) {
        analogDirection = ANALOG_DIRECTIONS[(u32)(ctrl.degrees / 22.5f)];
    } else {
        ctrl.analogX = 0;
        ctrl.analogY = 0;
        ctrl.radians = 0;
        ctrl.degrees = 0;
        ctrl.magnitude = 0;
        analogDirection = 0;
    }
    lastButtons = BUTTONS;
    BUTTONS = ctrl.buttons | (analogDirection << 16);
    int last = lastButtons;
    RISING_EDGE = ~last & BUTTONS;
    FALLING_EDGE = ~BUTTONS & last;
    UI_BUTTONS = 0;
    if (last == BUTTONS) {
        heldFrames++;
        if (heldFrames > 12) {
            heldFrames = 10;
            UI_BUTTONS = BUTTONS;
        }
    } else {
        UI_BUTTONS = RISING_EDGE;
        heldFrames = 0;
    }

    ANALOG_ANGLE = (ctrl.degrees << 16) / 360;// + ((ctrl.degrees << 16) >> 31);
    ANALOG_MAGNITUDE = ctrl.magnitude;
}
