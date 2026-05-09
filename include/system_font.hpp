#pragma once

#include "common.h"
#include "singleton.hpp"

struct Encoding {
    enum {
        SJIS,
        UTF8,
    };
private:
    Encoding();
};

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
    s32 maxGlyphWidthI;
    s32 maxGlyphHeightI;
    s32 maxGlyphAscenderI;
    s32 maxGlyphDescenderI;
    s32 maxGlyphLeftXI;
    s32 maxGlyphBaseYI;
    s32 minGlyphCenterXI;
    s32 maxGlyphTopYI;
    s32 maxGlyphAdvanceXI;
    s32 maxGlyphAdvanceYI;
    float maxGlyphWidthF;
    float maxGlyphHeightF;
    float maxGlyphAscenderF;
    float maxGlyphDescenderF;
    float maxGlyphLeftXF;
    float maxGlyphBaseYF;
    float minGlyphCenterXF;
    float maxGlyphTopYF;
    float maxGlyphAdvanceXF;
    float maxGlyphAdvanceYF;
    u16 maxGlyphWidth;
    u16 maxGlyphHeight;
    u32 numGlyphs;
    u32 shadowMapLength;
    PGFFontStyle fontStyle;
    u8 bpp;
};

// adapted from PPSSPP implementation of libfont
struct PGFCharInfo {
    u32 bitmapWidth;
    u32 bitmapHeight;
    u32 bitmapLeft;
    u32 bitmapTop;
    u32 sfp26Width;
    u32 sfp26Height;
    s32 sfp26Ascender;
    s32 sfp26Descender;
    s32 sfp26BearingHX;
    s32 sfp26BearingHY;
    s32 sfp26BearingVX;
    s32 sfp26BearingVY;
    s32 sfp26AdvanceH;
    s32 sfp26AdvanceV;
    s16 shadowFlags;
    s16 shadowId;
};

// adapted from PPSSPP implementation of libfont
struct GlyphImage {
    u32 pixelFormat;
    s32 xPos;
    s32 yPos;
    u16 bufWidth;
    u16 bufHeight;
    u16 bytesPerLine;
    u16 pad;
    void *bufferPtr;
};

struct ParseResult {
    enum {
        COMMAND,
        HALF_WIDTHS,
        CHARACTERS,
    };
private:
    ParseResult();
};

struct CharacterType {
    enum {
        HALFWIDTH,
        FULLWIDTH,
        ICON,
        END_OF_STRING = 0x4,
    };
private:
    CharacterType();
};

struct CacheState {
    enum {
        EMPTY,
        STALE,
        FRESH,
    };
private:
    CacheState();
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
    bool leftAlignHalfwidthGlyphs;
    s8 usedColorIds;
    u8 padding_0x132[2];
    u8 layerGlyphRunCounts[6];
    u8 layerIconCounts[6];
    GlyphRun *layerGlyphRuns[6];
    u16 nextCacheIndex;
    u16 numCachedGlyphs;
    u8 unknown_0x15C;
    s8 currentLayer;
    u8 frameCount;
    u8 lineSpacing;
    u16 codepointBufferUsed;
    u16 residentGlyphCount;
    u32 glowingFontColor;
    u32 fontColors[48];
    GlyphRun glyphRuns[6][128];
    u16 glyphIndices[65520];
    u16 cacheCodepoints[432];
    u8 cacheStates[432];
    Icon layerIcons[6][10];
    u8 codepointBuffer[12288];

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
    int halfWidthsX(char *utf8x);
    int lineHalfWidthsX(char *utf8x);
    void print(u16 *codepoints);
    void print(s16 left, s16 top, u16 *codepoints);
    void print(s16 left, s16 top, s8 fontColor, u16 *codepoints);
    void printfUtf8(char *format, ...);
    void printfUtf8(s16 left, s16 top, char *format, ...);
    void printfUtf8(s16 left, s16 top, s8 fontColor, char *format, ...);
    void printfSJIS(s16 left, s16 top, char *format, ...);
    void printfSJIS(s16 left, s16 top, s8 fontColor, char *format, ...);
    void printShadowUtf8(s16 left, s16 top, s8 shadowColor, s8 fontColor, u8 *utf8, s16 offsetLeft, s16 offsetTop);
    void drawLayer(s8 layer);
    void draw();
    int widthUtf8(u8 *utf8);
    void asciiToFullWidthUtf8(char *in, char *out);
    void asciiToFullWidthSJIS(char *in, char *out);
    void loadResidentGlyphsSJIS(char *sjis);
    int characterCountUtf8(char *utf8);
    int characterCountSJIS(char *sjis);
    void printfUtf8x(char *format, ...);
    void printfUtf8x(s16 left, s16 top, char *format, ...);
    void printfUtf8x(s16 left, s16 top, s8 fontColor, char *format, ...);
    void printTextUtf8x(s16 n, u8 *utf8x);
    s16 nthCharacterUtf8x(s16 n, char *out, char *str, char **nextOut);
    void drawBtnIcon(Icon *icons, u8 count, u32 renderGroup);
    void initializeFont();
    void beforeDrawGlyphs();
    void afterDrawIcons();
    void markUsedGlyphs();
    bool markUsedGlyph(u16 codepoint);
    u16 allocateCacheIndex();
    void loadGlyphs();
    void loadGlyph(u16 codepoint, int atlas, s32 u, s32 v);
    void drawGlyphRun(GlyphRun *run);
    void drawGlyph(u16 codepoint, int width, GlyphRun *run);
    int vsnprintf(char *buffer, int size, char *format, va_list args, u8 encoding);
    void decode(u8 *str, u8 encoding);
    int isHalfWidth(u16 codepoint);
    u32 currentRenderGroup();
    void updateGlowingFontColor();
    void cacheCommonGlyphs();
    char *parseCommand(char *str, s16 *out, s16 type, u8 encoding);
    void copySubstitution(char *dst, s16 substitutionId, u8 encoding);
    s16 nthCharacter(s16 n, char *out, char **str, u8 encoding);
    void addIcon(s16 left, s16 top, u16 size, s8 fontColor, s16 iconId);
    // glyphWidth?
    // hasDiacritic?

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
