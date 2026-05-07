#pragma once

#include "common.h"
#include "singleton.hpp"
#include "cache.hpp"

struct GlyphRun {
    s16 left;
    s16 top;
    u8 fontWidth;
    u8 fontHeight;
    s8 fontColor;
    u8 lineSpacing;
    u16 *codepoints;
};

struct Icon {
    s16 left;
    s16 top;
    s16 size;
    u8 iconId;
    s8 fontColor;
};

// adapted from PPSSPP implementation of libfont
struct PGFFontStyle {
    float fontH;
    float fontV;
    float fontHRes;
    float fontVRes;
    float fontWeight;
    u16 fontFamily;
    u16 fontStyle;
    u16 fontStyleSub;
    u16 fontLanguage;
    u16 fontRegion;
    u16 fontCountry;
    char fontName[64];
    char fontFileName[64];
    u32 fontAttributes;
    u32 fontExpire;
};

// adapted from PPSSPP implementation of libfont
struct PGFFontInfo {
    s32 glyphMetricsFixed[10];
    float glyphMetricsFloat[10];
    u16 maxGlyphWidth;
    u16 maxGlyphHeight;
    u32 numGlyphs;
    u32 shadowMapLength;
    PGFFontStyle fontStyle;
    u8 bpp;
};

struct SystemFont : Singleton<SystemFont> {
    void *fontLib;
    s32 fontId;
    void *font;
    PGFFontInfo fontInfo;
    u8 glyphWidth;
    u8 glyphHeight;
    u8 glyphSpacingX;
    u8 glyphSpacingY;
    u8 glyphTextureWidth;
    u8 glyphTextureHeight;
    u8 glyphsPerRow;
    u8 rowsPerAtlas;
    u16 glyphsPerAtlas;
    u16 maxCachedGlyphs;
    s16 left;
    s16 top;
    s16 unknown_0x124;
    u16 cursorLeft;
    u16 cursorTop;
    u16 cursorZ;
    u8 fontWidth;
    u8 fontHeight;
    s8 fontColor;
    s8 layer;
    bool unknown_0x130;
    s8 usedColorIds;
    u8 padding_0x132[2];
    s8 layerGlyphRunCounts[6];
    s8 layerIconCounts[6];
    GlyphRun *layerGlyphRuns[6];
    s16 glyphCacheCount;
    s16 unknown_0x15A;
    u8 unknown_0x15C;
    s8 currentLayer;
    u8 unknown_0x15E;
    u8 lineSpacing;
    u16 decodeBufferUsed;
    u16 unknown_0x162;
    u32 glowingFontColor;
    u32 fontColors[48];
    GlyphRun glyphRuns[6][128];
    u16 glyphIndices[65520];
    u16 cachedCodepoints[432];
    u8 cacheStatus[432];
    Icon layerIcons[6][10];
    u8 decodeBuffer[12288];

    void initialize();
    void initializeVram();
    void initializeGlyphIndex();
    void clear();
    void setColor(s32 fontColorId, u32 alpha, u32 bgrColor);
    void initializeColors();
    s8 addColor(u32 bgrColor); // setUserFontColor?
    void setFontSize(u8 fontWidth, u8 fontHeight);
    void setFontColor(s8 fontColor);
    void setCursor(s16 left, s16 top);
    void setLayer(s8 layer);
    void setLineSpacing(u8 spacing);
    int halfWidths(u8 *utf8);
    int lineHalfWidths(u8 *utf8);
    u32 strlen(char *str);

    void drawBtnIcon(Icon *icons, u8 count, u32 renderGroup);

    void asciiToFullWidthUtf8(char *in, char *out);
    void asciiToFullWidthSJIS(char *in, char *out);

    u32 currentRenderGroup();

    int isHalfWidth(u16 codepoint);

    void initializeFont();

    void cacheCommonGlyphs();
    void updateGlowingFontColor();

    // printf-type functions
    void method_08891070(s16 left, s16 top, s8 color, char *fmt, ...);
    void method_08891B68(s16 left, s16 top, char *fmt, ...);
    void method_08890F34(s16 left, s16 top, char *fmt, ...);

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

private:
    FontColor();
};
