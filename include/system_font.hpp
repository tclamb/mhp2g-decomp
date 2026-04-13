#pragma once

#include "common.h"
#include "singleton.hpp"

struct SystemFont : Singleton<SystemFont> {
    u8 pad_0x0[0x25CF8];

    // printf-type functions
    void method_08891070(s16 left, s16 top, s8 color, char *fmt, ...);
    void method_08891B68(s16 left, s16 top, char *fmt, ...);
    void method_08890F34(s16 left, s16 top, char *fmt, ...);

    // setFontSize
    void method_088908A8(u8 fontWidth, u8 fontHeight);
    // setFontColor
    void method_088908F8(s8 fontColor);
    // setLayer?
    void method_088908C8(u8);

    SystemFont();
};

struct FontColor {
    enum {
        WHITE,
        BLACK,
        FLAMINGO,
        LIME,
        AQUA,
        LIGHT_YELLOW,
        LIGHT_ORANGE,
        FUSCHIA,
        FADED_ROSE,
        LIGHT_GRAY,
        DARK_GRAY,
        ORANGE,
        BROWN,
        MAROON,
        SLATE_BLUE,
        PINK,
        MAGENTA,
        BLUE,
        YELLOW,
        RED,
        GREEN,
        OLIVE,
        SKY_BLUE,
        LILAC,
        GOLD,
    };

    s8 value;

    inline FontColor() : value(WHITE) {}
    inline FontColor(s8 value) : value(value) {}

    inline void apply() const {
        Singleton<SystemFont>::objectPtr->method_088908F8(value);
    }

    inline operator s8() const {
        return value;
    }


    static inline void white() { FontColor(WHITE).apply(); }
    static inline void black() { FontColor(BLACK).apply(); }
    static inline void flamingo() { FontColor(FLAMINGO).apply(); }
    static inline void lime() { FontColor(LIME).apply(); }
    static inline void aqua() { FontColor(AQUA).apply(); }
    static inline void lightYellow() { FontColor(LIGHT_YELLOW).apply(); }
    static inline void lightOrange() { FontColor(LIGHT_ORANGE).apply(); }
    static inline void fuschia() { FontColor(FUSCHIA).apply(); }
    static inline void fadedRose() { FontColor(FADED_ROSE).apply(); }
    static inline void lightGray() { FontColor(LIGHT_GRAY).apply(); }
    static inline void darkGray() { FontColor(DARK_GRAY).apply(); }
    static inline void orange() { FontColor(ORANGE).apply(); }
    static inline void brown() { FontColor(BROWN).apply(); }
    static inline void maroon() { FontColor(MAROON).apply(); }
    static inline void slateBlue() { FontColor(SLATE_BLUE).apply(); }
    static inline void pink() { FontColor(PINK).apply(); }
    static inline void magenta() { FontColor(MAGENTA).apply(); }
    static inline void blue() { FontColor(BLUE).apply(); }
    static inline void yellow() { FontColor(YELLOW).apply(); }
    static inline void red() { FontColor(RED).apply(); }
    static inline void green() { FontColor(GREEN).apply(); }
    static inline void olive() { FontColor(OLIVE).apply(); }
    static inline void skyBlue() { FontColor(SKY_BLUE).apply(); }
    static inline void lilac() { FontColor(LILAC).apply(); }
    static inline void gold() { FontColor(GOLD).apply(); }

};
