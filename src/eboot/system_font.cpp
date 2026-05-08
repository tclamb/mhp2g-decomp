#include "system_font.hpp"

#include "cache.hpp"
#include "vfpu.h"
#include "vram_manager.hpp"
#include "ccc.hpp"
#include "resource_manager.hpp"
#include "ge.hpp"
#include "immediate_ge.hpp"

#pragma opt_unroll_loops on

template<> SystemFont *Singleton<SystemFont>::objectPtr;

cache GLYPH_CACHE;
u8 GLYPH_CACHE_SLAB[0x28000]  __attribute__((aligned(16)));
s32 GLYPH_CACHE_USED;
s32 GLYPH_CACHE_MAX_USED;


extern "C" {
    void *memcpy( void *dst, const void *src, u32 count);
    char *strcat(char *dst, const char *src);
    char *strcpy(char *dst, const char *src);
    u32 strlen(const char *str);
    int sprintf(char *buffer, const char *format, ...);
}

inline void set(u8 &field, u16 value) {
    field = value;
    field = (field + 1) & -2;
}

void SystemFont::initialize() {
    GLYPH_CACHE.reset(GLYPH_CACHE_SLAB, sizeof(GLYPH_CACHE_SLAB));
    GLYPH_CACHE_USED = 0;
    GLYPH_CACHE_MAX_USED = 0;
    initializeFont();
    set(glyphWidth, fontInfo.maxGlyphWidth);
    set(glyphHeight, fontInfo.maxGlyphHeight);
    set(glyphSpacingX, 0);
    set(glyphSpacingY, 2);
    glyphTextureWidth = glyphWidth + glyphSpacingX;
    glyphTextureHeight = glyphHeight + glyphSpacingY;
    glyphsPerRow = 0x100 / glyphTextureWidth;
    rowsPerAtlas = 0x100 / glyphTextureHeight;
    glyphsPerAtlas = glyphsPerRow * rowsPerAtlas;
    maxCachedGlyphs = glyphsPerAtlas * 3;
    initializeVram();
}

inline void initializeVramTexture(VramTexture &texture) {
    const static u16 VRAM_DATA[64] = {
        0x0FFF, 0x1FFF, 0x2FFF, 0x3FFF, 0x4FFF, 0x5FFF, 0x6FFF, 0x7FFF,
        0x8FFF, 0x9FFF, 0xAFFF, 0xBFFF, 0xCFFF, 0xDFFF, 0xEFFF, 0xFFFF,
        0xFFFF, 0xFEEE, 0xFDDD, 0xFCCC, 0xFBBB, 0xFAAA, 0xF999, 0xF888,
        0xF777, 0xF666, 0xF555, 0xF444, 0xF333, 0xF222, 0xF111, 0xF000,
        0xF555, 0xF555, 0xF555, 0xF555, 0xF555, 0xF555, 0xF666, 0xF777,
        0xF888, 0xF999, 0xFAAA, 0xFBBB, 0xFCCC, 0xFDDD, 0xFEEE, 0xFFFF,
        0xFFFF, 0xFEEE, 0xFDDD, 0xFCCC, 0xFBBB, 0xFAAA, 0xF999, 0xF888,
        0xF777, 0xF666, 0xF555, 0xF444, 0xF333, 0xF222, 0xF111, 0xF000,
    };
    texture.paletteWidth = 2;
    const u16 *q = &VRAM_DATA[0];
    u16 *p = (u16 *)texture.vramBlockAddress;
    for (int i = 0; i < 64; ++i) {
        *p++ = *q++;
    }
}

void SystemFont::initializeVram() {
    VramManager::objectPtr->alloc(3, 0x100, 0x100, 4, 0, -1);
    VramManager::objectPtr->alloc(4, 0x100, 0x100, 4, 0, -1);
    VramManager::objectPtr->alloc(5, 0x100, 0x100, 4, 0, -1);
    initializeVramTexture(VramManager::objectPtr->allocations[3].texture);
    initializeVramTexture(VramManager::objectPtr->allocations[4].texture);
    initializeVramTexture(VramManager::objectPtr->allocations[5].texture);
    for (int i = 0; i < 6; ++i) {
        layerGlyphRuns[i] = &glyphRuns[i][0];
    }
    unknown_0x130 = false;
    initializeGlyphIndex();
    clear();
    initializeColors();
    cacheCommonGlyphs();
}

void SystemFont::initializeGlyphIndex() {
    u16 *entry = &glyphIndices[0];
    for (int i = 0; i < 0xFFF0; ++i) {
        entry[i] = 0xFFFF;
    }
    glyphCacheCount = 0;
    memset(cacheStatus, 0, sizeof(cacheStatus));
    unknown_0x15A = 0;
    unknown_0x162 = 0;
}

void SystemFont::clear() {
    for (int i = 0; i < 6; ++i) {
        layerGlyphRunCounts[i] = 0;
        layerIconCounts[i] = 0;
    }
    codepointBufferUsed = 0;
    for (int i = 0; i < 0x1B0; ++i) {
        if (cacheStatus[i] != 0) {
            --cacheStatus[i];
        }
    }
    left = 0;
    top = 0;
    unknown_0x124 = 0;
    fontWidth = 20;
    fontHeight = 20;
    lineSpacing = fontHeight;
    fontColor = FontColor::WHITE;
    layer = 0;
    unknown_0x15C = 1;
    usedColorIds = 32;
    updateGlowingFontColor();
    ++frameCount;
}

void SystemFont::setColor(s32 fontColorId, u32 alpha, u32 rgb) {
    u32 *color;
    if (fontColorId < 0) {
        color = &glowingFontColor;
    } else {
        color = &fontColors[fontColorId];
    }
    *color =  (alpha & 0xFF000000) | (rgb & 0xFF0000) >> 16 | (rgb & 0xFF00) | (rgb & 0xFF) << 16;
}

void SystemFont::initializeColors() {
    const static u32 ARGB_FONT_COLORS[48] = {
        0xFFFFFFFF, // WHITE
        0xFF000000, // BLACK
        0xFFFF435D, // FLAMINGO
        0xFF56FF56, // LIME
        0xFF57FFFF, // AQUA
        0xFFFFFF50, // LIGHT_YELLOW
        0xFFFFA461, // LIGHT_ORANGE
        0xFFFF84FF, // FUSCHIA
        0xFFBF6464, // FADED_ROSE
        0xFFA0A0A0, // LIGHT_GRAY
        0xFF808080, // DARK_GRAY
        0xFFF08200, // ORANGE
        0xFF846B5C, // WOOD_BROWN
        0xFF80212E, // MAROON
        0xFF747EFF, // SLATE_BLUE
        0xFFFF9ECA, // PINK
        0xFFF011EE, // MAGENTA
        0xFF2028FF, // BLUE
        0xFFFFFF00, // YELLOW
        0xFFFF0000, // RED
        0xFF00FF00, // GREEN
        0xFFC3BE9F, // OLIVE
        0xFF80BFFF, // SKY_BLUE
        0xFFBE84FF, // LILAC
        0xFFFFC600, // GOLD
        0xFF000000, // BLACK
        0xFF000000, // BLACK
        0xFF000000, // BLACK
        0xFF000000, // BLACK
        0xFF000000, // BLACK
        0xFFFFFFFF, // WHITE
        0xFF808080, // DARK_GRAY
    };
    const u32 *color = &ARGB_FONT_COLORS[0];
    for (int i = 0; i < 30; ++i, ++color) {
        setColor(i, 0, *color);
    }
    for (int i = 30; i < 32; ++i, ++color) {
        setColor(i, 0xFF000000, *color);
    }
    for (int i = 32; i < 48; ++i) {
        setColor(i, 0, 0);
    }
}

s8 SystemFont::addColor(u32 rgbColor) {
    s8 id = usedColorIds;
    setColor(id, 0, rgbColor);
    setFontColor(id);
    ++usedColorIds;
    return id;
}

void SystemFont::setFontSize(u8 width, u8 height) {
    fontWidth = width;
    fontHeight = height;
    lineSpacing = fontHeight;
}

void SystemFont::setCursor(s16 left, s16 top) {
    this->left = left;
    this->top = top;
}

void SystemFont::setLayer(s8 layer) {
    if (layer < 0) {
        layer = 0;
    }
    if (layer >= 6) {
        layer = 5;
    }
    this->layer = layer;
}

void SystemFont::setFontColor(s8 fontColor) {
    this->fontColor = fontColor;
}

void SystemFont::setLineSpacing(u8 lineSpacing) {
    this->lineSpacing = lineSpacing;
}

int SystemFont::halfWidths(u8 *utf8) {
    int result = 0;
    u16 codepoint;
    while ((codepoint = CCC::objectPtr->decodeUtf8(&utf8)) != 0) {
        int widths;
        if ((isHalfWidth(codepoint) != 0) ^ 1) {
            widths = 2;
        } else {
            widths = 1;
        }
        result += widths;
    }
    return result;
}

int SystemFont::lineHalfWidths(u8 *utf8) {
    int result = 0;
    u16 codepoint;
    while ((codepoint = CCC::objectPtr->decodeUtf8(&utf8)) != '\n') {
        int widths;
        if ((isHalfWidth(codepoint) != 0) ^ 1) {
            widths = 2;
        } else {
            widths = 1;
        }
        result += widths;
    }
    return result;
}

u32 SystemFont::strlen(char *str) {
    return ::strlen(str);
}

extern "C" char *strstr(char *haystack, char *needle);

int SystemFont::halfWidthsX(char *utf8x) {
    int result;
    char *next;
    int raw;
    raw = false;
    result = halfWidths((u8 *)utf8x);
    while (*utf8x != 0) {
        next = strstr(utf8x, "~");
        if (next == NULL) {
            break;
        }
        s16 out;
        parseCommand(next, &out, ParseResult::COMMAND, Encoding::UTF8);
        if ((out & 0xFF00) == 0x800) {
            raw ^= 1;
            utf8x = parseCommand(next, &out, ParseResult::HALF_WIDTHS, Encoding::UTF8);
            result += out;
            continue;
        }
        if (!raw) {
            utf8x = parseCommand(next, &out, ParseResult::HALF_WIDTHS, Encoding::UTF8);
            result += out;
        } else {
            utf8x = next + 1;
        }
    }
    return result;
}

int SystemFont::lineHalfWidthsX(char *utf8x) {
    int result;
    char *next;
    int raw;
    raw = false;
    result = lineHalfWidths((u8 *)utf8x);
    while (*utf8x != 0 && *utf8x != '\n') {
        char *end = strstr(utf8x, "\n");
        next = strstr(utf8x, "~");
        if (next == NULL) {
            break;
        }
        if (end <= next) {
            break;
        }
        s16 out;
        parseCommand(next, &out, ParseResult::COMMAND, Encoding::UTF8);
        if ((out & 0xFF00) == 0x800) {
            raw ^= 1;
            utf8x = parseCommand(next, &out, ParseResult::HALF_WIDTHS, Encoding::UTF8);
            result += out;
            continue;
        }
        if (!raw) {
            utf8x = parseCommand(next, &out, ParseResult::HALF_WIDTHS, Encoding::UTF8);
            result += out;
        } else {
            utf8x = next + 1;
        }
    }
    return result;
}

void SystemFont::print(u16 *codepoints) {
    int overflow = 0;
    u8 i = layerGlyphRunCounts[layer];
    if (i < 0x80) {
        GlyphRun &run = layerGlyphRuns[layer][i];
        ++layerGlyphRunCounts[layer];

        run.left = left;
        run.top = top;
        run.fontWidth = fontWidth;
        run.fontHeight = fontHeight;
        run.lineSpacing = lineSpacing;
        run.fontColor = fontColor;
        u16 *out = (u16 *)(codepointBuffer + codepointBufferUsed);
        run.codepoints = out;

        while (*codepoints != 0) {
            *out = *codepoints;
            ++out;
            codepointBufferUsed += 2;
            ++codepoints;
            if (codepointBufferUsed >= 0x3000) {
                codepointBufferUsed = 0x2FFE;
                ++overflow;
                out = (u16 *)(codepointBuffer + codepointBufferUsed);
            }
        }
        *out = 0;
        if (overflow == 0) {
            codepointBufferUsed += 2;
            if (codepointBufferUsed >= 0x3000) {
                codepointBufferUsed = 0x2FFE;
            }
        }
    }
}

void SystemFont::print(s16 left, s16 top, u16 *codepoints) {
    setCursor(left, top);
    print(codepoints);
}

void SystemFont::print(s16 left, s16 top, s8 fontColor, u16 *codepoints) {
    setCursor(left, top);
    setFontColor(fontColor);
    print(codepoints);
}

void SystemFont::printfUtf8(char *format, ...) {
    char buffer[0x300];
    u8 i = layerGlyphRunCounts[layer];
    if (i < 0x80) {
        GlyphRun &run = layerGlyphRuns[layer][i];
        ++layerGlyphRunCounts[layer];

        run.left = left;
        run.top = top;
        run.fontWidth = fontWidth;
        run.fontHeight = fontHeight;
        run.lineSpacing = lineSpacing;
        run.fontColor = fontColor;
        run.codepoints = (u16 *)(codepointBuffer + codepointBufferUsed);

        memset(buffer, 0, sizeof(buffer));

        va_list va_args;
        va_start(va_args, format);
        vsnprintf(buffer, sizeof(buffer), format, va_args, Encoding::UTF8);
        va_end(va_args);

        decode((u8 *)buffer, Encoding::UTF8);
    }
}

void SystemFont::printfUtf8(s16 left, s16 top, char *format, ...) {
    char buffer[0x300];
    u8 i = layerGlyphRunCounts[layer];
    if (i < 0x80) {
        setCursor(left, top);

        GlyphRun &run = layerGlyphRuns[layer][i];
        ++layerGlyphRunCounts[layer];

        run.left = this->left;
        run.top = this->top;
        run.fontWidth = fontWidth;
        run.fontHeight = fontHeight;
        run.lineSpacing = lineSpacing;
        run.fontColor = fontColor;
        run.codepoints = (u16 *)(codepointBuffer + codepointBufferUsed);

        memset(buffer, 0, sizeof(buffer));

        va_list va_args;
        va_start(va_args, format);
        vsnprintf(buffer, sizeof(buffer), format, va_args, Encoding::UTF8);
        va_end(va_args);

        decode((u8 *)buffer, Encoding::UTF8);
    }
}

void SystemFont::printfUtf8(s16 left, s16 top, s8 fontColor, char *format, ...) {
    char buffer[0x300];
    u8 i = layerGlyphRunCounts[layer];
    if (i < 0x80) {
        setCursor(left, top);
        setFontColor(fontColor);

        GlyphRun &run = layerGlyphRuns[layer][i];
        ++layerGlyphRunCounts[layer];

        run.left = this->left;
        run.top = this->top;
        run.fontWidth = fontWidth;
        run.fontHeight = fontHeight;
        run.lineSpacing = lineSpacing;
        run.fontColor = this->fontColor;
        run.codepoints = (u16 *)(codepointBuffer + codepointBufferUsed);

        memset(buffer, 0, sizeof(buffer));

        va_list va_args;
        va_start(va_args, format);
        vsnprintf(buffer, sizeof(buffer), format, va_args, Encoding::UTF8);
        va_end(va_args);

        decode((u8 *)buffer, Encoding::UTF8);
    }
}

void SystemFont::printfSJIS(s16 left, s16 top, char *format, ...) {
    char buffer[0x300];
    u8 i = layerGlyphRunCounts[layer];
    if (i < 0x80) {
        setCursor(left, top);

        GlyphRun &run = layerGlyphRuns[layer][i];
        ++layerGlyphRunCounts[layer];

        run.left = this->left;
        run.top = this->top;
        run.fontWidth = fontWidth;
        run.fontHeight = fontHeight;
        run.lineSpacing = lineSpacing;
        run.fontColor = fontColor;
        run.codepoints = (u16 *)(codepointBuffer + codepointBufferUsed);

        memset(buffer, 0, sizeof(buffer));

        va_list va_args;
        va_start(va_args, format);
        vsnprintf(buffer, sizeof(buffer), format, va_args, Encoding::SJIS);
        va_end(va_args);

        decode((u8 *)buffer, Encoding::SJIS);
    }
}

void SystemFont::printfSJIS(s16 left, s16 top, s8 fontColor, char *format, ...) {
    char buffer[0x300];
    u8 i = layerGlyphRunCounts[layer];
    if (i < 0x80) {
        setCursor(left, top);
        setFontColor(fontColor);

        GlyphRun &run = layerGlyphRuns[layer][i];
        ++layerGlyphRunCounts[layer];

        run.left = this->left;
        run.top = this->top;
        run.fontWidth = fontWidth;
        run.fontHeight = fontHeight;
        run.lineSpacing = lineSpacing;
        run.fontColor = this->fontColor;
        run.codepoints = (u16 *)(codepointBuffer + codepointBufferUsed);

        memset(buffer, 0, sizeof(buffer));

        va_list va_args;
        va_start(va_args, format);
        vsnprintf(buffer, sizeof(buffer), format, va_args, Encoding::SJIS);
        va_end(va_args);

        decode((u8 *)buffer, Encoding::SJIS);
    }
}

void SystemFont::printShadowUtf8(s16 left, s16 top, s8 shadowColor, s8 fontColor, u8 *utf8, s16 offsetLeft, s16 offsetTop) {
    printfUtf8(left + offsetLeft, top + offsetTop, shadowColor, "%s", utf8);
    printfUtf8(left, top, fontColor, "%s", utf8);
}

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088914E8);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088915D4);

int SystemFont::widthUtf8(u8 *utf8) {
    if (utf8 == NULL) {
        return 0;
    }

    if (*utf8 == 0) {
        return 0;
    }

    u8 *p = utf8;
    u16 codepoint = CCC::objectPtr->decodeUtf8(&p);
    if (isHalfWidth(codepoint)) {
        return 1;
    } else {
        return 2;
    }
}

struct IconAtlasCoordinate {
    u8 left;
    u8 top;
    u8 width;
    u8 height;
};

const IconAtlasCoordinate ICON_COORDINATES[12] = {
    {  0,   0, 16, 16}, // CIRCLE
    { 16,   0, 16, 16}, // CROSS
    { 32,   0, 16, 16}, // TRIANGLE
    { 48,   0, 16, 16}, // SQUARE
    { 64,   0, 20, 15}, // L_TRIGGER
    { 84,   0, 20, 15}, // R_TRIGGER
    {  0,  17, 20, 15}, // START
    { 20,  17, 20, 15}, // SELECT
    {104,   0, 16, 16}, // ANALOG_STICK
    { 64,  16, 16, 16}, // DIRECTIONAL_PAD
    { 41,  17, 15, 15}, // TRADEMARK
    { 58, 112, 15, 15}, // REGISTERED
};

const u8 ASCII_TO_FULL_WIDTH_UTF8[128][3] = {
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xE3', '\x80', '\x80'},
    {'\xEF', '\xBC', '\x81'},
    {'\xEF', '\xBC', '\x82'},
    {'\xEF', '\xBC', '\x83'},
    {'\xEF', '\xBC', '\x84'},
    {'\xEF', '\xBC', '\x85'},
    {'\xEF', '\xBC', '\x86'},
    {'\xEF', '\xBC', '\x87'},
    {'\xEF', '\xBC', '\x88'},
    {'\xEF', '\xBC', '\x89'},
    {'\xEF', '\xBC', '\x8A'},
    {'\xEF', '\xBC', '\x8B'},
    {'\xEF', '\xBC', '\x8C'},
    {'\xEF', '\xBC', '\x8D'},
    {'\xEF', '\xBC', '\x8E'},
    {'\xEF', '\xBC', '\x8F'},
    {'\xEF', '\xBC', '\x90'},
    {'\xEF', '\xBC', '\x91'},
    {'\xEF', '\xBC', '\x92'},
    {'\xEF', '\xBC', '\x93'},
    {'\xEF', '\xBC', '\x94'},
    {'\xEF', '\xBC', '\x95'},
    {'\xEF', '\xBC', '\x96'},
    {'\xEF', '\xBC', '\x97'},
    {'\xEF', '\xBC', '\x98'},
    {'\xEF', '\xBC', '\x99'},
    {'\xEF', '\xBC', '\x9A'},
    {'\xEF', '\xBC', '\x9B'},
    {'\xEF', '\xBC', '\x9C'},
    {'\xEF', '\xBC', '\x9D'},
    {'\xEF', '\xBC', '\x9E'},
    {'\xEF', '\xBC', '\x9F'},
    {'\xEF', '\xBC', '\xA0'},
    {'\xEF', '\xBC', '\xA1'},
    {'\xEF', '\xBC', '\xA2'},
    {'\xEF', '\xBC', '\xA3'},
    {'\xEF', '\xBC', '\xA4'},
    {'\xEF', '\xBC', '\xA5'},
    {'\xEF', '\xBC', '\xA6'},
    {'\xEF', '\xBC', '\xA7'},
    {'\xEF', '\xBC', '\xA8'},
    {'\xEF', '\xBC', '\xA9'},
    {'\xEF', '\xBC', '\xAA'},
    {'\xEF', '\xBC', '\xAB'},
    {'\xEF', '\xBC', '\xAC'},
    {'\xEF', '\xBC', '\xAD'},
    {'\xEF', '\xBC', '\xAE'},
    {'\xEF', '\xBC', '\xAF'},
    {'\xEF', '\xBC', '\xB0'},
    {'\xEF', '\xBC', '\xB1'},
    {'\xEF', '\xBC', '\xB2'},
    {'\xEF', '\xBC', '\xB3'},
    {'\xEF', '\xBC', '\xB4'},
    {'\xEF', '\xBC', '\xB5'},
    {'\xEF', '\xBC', '\xB6'},
    {'\xEF', '\xBC', '\xB7'},
    {'\xEF', '\xBC', '\xB8'},
    {'\xEF', '\xBC', '\xB9'},
    {'\xEF', '\xBC', '\xBA'},
    {'\xEF', '\xBC', '\xBB'},
    {'\xEF', '\xBC', '\xBC'},
    {'\xEF', '\xBC', '\xBD'},
    {'\xEF', '\xBC', '\xBE'},
    {'\xEF', '\xBC', '\xBF'},
    {'\xEF', '\xBD', '\x80'},
    {'\xEF', '\xBD', '\x81'},
    {'\xEF', '\xBD', '\x82'},
    {'\xEF', '\xBD', '\x83'},
    {'\xEF', '\xBD', '\x84'},
    {'\xEF', '\xBD', '\x85'},
    {'\xEF', '\xBD', '\x86'},
    {'\xEF', '\xBD', '\x87'},
    {'\xEF', '\xBD', '\x88'},
    {'\xEF', '\xBD', '\x89'},
    {'\xEF', '\xBD', '\x8A'},
    {'\xEF', '\xBD', '\x8B'},
    {'\xEF', '\xBD', '\x8C'},
    {'\xEF', '\xBD', '\x8D'},
    {'\xEF', '\xBD', '\x8E'},
    {'\xEF', '\xBD', '\x8F'},
    {'\xEF', '\xBD', '\x90'},
    {'\xEF', '\xBD', '\x91'},
    {'\xEF', '\xBD', '\x92'},
    {'\xEF', '\xBD', '\x93'},
    {'\xEF', '\xBD', '\x94'},
    {'\xEF', '\xBD', '\x95'},
    {'\xEF', '\xBD', '\x96'},
    {'\xEF', '\xBD', '\x97'},
    {'\xEF', '\xBD', '\x98'},
    {'\xEF', '\xBD', '\x99'},
    {'\xEF', '\xBD', '\x9A'},
    {'\xEF', '\xBD', '\x9B'},
    {'\xEF', '\xBD', '\x9C'},
    {'\xEF', '\xBD', '\x9D'},
    {'\xEF', '\xBD', '\x9E'},
    {'\xEF', '\xBD', '\x9F'},
};

void SystemFont::asciiToFullWidthUtf8(char *in, char *out) {
    while (*in != 0) {
        int codepointLength = CCC::objectPtr->codepointLengthUtf8(*in);
        if (codepointLength == 0) {
            break;
        }
        if (codepointLength == 1) {
            char c = *in;
            if (c >= 0x20 && c < 0x7F) {
                ++in;
                for (int i = 0; i < 3; ++i) {
                    out[i] = ASCII_TO_FULL_WIDTH_UTF8[c][i];
                }
                out += 3;
            } else {
                ++in;
                *out++ = c;
                continue;
            }
        } else {
            for (int i = 0; i < codepointLength; ++i) {
                *out++ = *in++;
            }
        }
    }
    *out = 0;
}

// this likely was a multibyte string literal
// try converting this file to Shift-JIS
// once the toolchain no longer requires UTF-8
typedef u8 sjisTable[129][2];
inline sjisTable *asciiToFullWidthSJISTable() {
    static u8 SJIS_TABLE[129][2] = {
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x40'},
        {'\x81', '\x49'},
        {'\x81', '\x68'},
        {'\x81', '\x94'},
        {'\x81', '\x90'},
        {'\x81', '\x93'},
        {'\x81', '\x95'},
        {'\x81', '\x66'},
        {'\x81', '\x69'},
        {'\x81', '\x6A'},
        {'\x81', '\x96'},
        {'\x81', '\x7B'},
        {'\x81', '\x43'},
        {'\x81', '\x7C'},
        {'\x81', '\x44'},
        {'\x81', '\x5E'},
        {'\x82', '\x4F'},
        {'\x82', '\x50'},
        {'\x82', '\x51'},
        {'\x82', '\x52'},
        {'\x82', '\x53'},
        {'\x82', '\x54'},
        {'\x82', '\x55'},
        {'\x82', '\x56'},
        {'\x82', '\x57'},
        {'\x82', '\x58'},
        {'\x81', '\x46'},
        {'\x81', '\x47'},
        {'\x81', '\x83'},
        {'\x81', '\x81'},
        {'\x81', '\x84'},
        {'\x81', '\x48'},
        {'\x81', '\x97'},
        {'\x82', '\x60'},
        {'\x82', '\x61'},
        {'\x82', '\x62'},
        {'\x82', '\x63'},
        {'\x82', '\x64'},
        {'\x82', '\x65'},
        {'\x82', '\x66'},
        {'\x82', '\x67'},
        {'\x82', '\x68'},
        {'\x82', '\x69'},
        {'\x82', '\x6A'},
        {'\x82', '\x6B'},
        {'\x82', '\x6C'},
        {'\x82', '\x6D'},
        {'\x82', '\x6E'},
        {'\x82', '\x6F'},
        {'\x82', '\x70'},
        {'\x82', '\x71'},
        {'\x82', '\x72'},
        {'\x82', '\x73'},
        {'\x82', '\x74'},
        {'\x82', '\x75'},
        {'\x82', '\x76'},
        {'\x82', '\x77'},
        {'\x82', '\x78'},
        {'\x82', '\x79'},
        {'\x81', '\x6D'},
        {'\x81', '\x8F'},
        {'\x81', '\x6E'},
        {'\x81', '\x4F'},
        {'\x81', '\x51'},
        {'\x81', '\x65'},
        {'\x82', '\x81'},
        {'\x82', '\x82'},
        {'\x82', '\x83'},
        {'\x82', '\x84'},
        {'\x82', '\x85'},
        {'\x82', '\x86'},
        {'\x82', '\x87'},
        {'\x82', '\x88'},
        {'\x82', '\x89'},
        {'\x82', '\x8A'},
        {'\x82', '\x8B'},
        {'\x82', '\x8C'},
        {'\x82', '\x8D'},
        {'\x82', '\x8E'},
        {'\x82', '\x8F'},
        {'\x82', '\x90'},
        {'\x82', '\x91'},
        {'\x82', '\x92'},
        {'\x82', '\x93'},
        {'\x82', '\x94'},
        {'\x82', '\x95'},
        {'\x82', '\x96'},
        {'\x82', '\x97'},
        {'\x82', '\x98'},
        {'\x82', '\x99'},
        {'\x82', '\x9A'},
        {'\x81', '\x6F'},
        {'\x81', '\x62'},
        {'\x81', '\x70'},
        {'\x81', '\x60'},
        {'\x81', '\x45'},
    };
    static u8 (*SJIS_TABLE_PTR)[129][2] = &SJIS_TABLE;
    return SJIS_TABLE_PTR;
}

void SystemFont::asciiToFullWidthSJIS(char *in, char *out) {
    char *p = in;
    char *q = out;
    s8 c;
    while ((c = *p) != 0) {
        if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c < 0x100)) {
            q[0] = c;
            q[1] = p[1];
            p += 2;
            q += 2;
        } else if (c >= 0x20 && c < 0x7F) {
            ++p;
            q[0] = (*asciiToFullWidthSJISTable())[(int)c][0];
            q[1] = (*asciiToFullWidthSJISTable())[(int)c][1];
            q += 2;
        } else {
            *q = c;
            ++p;
            ++q;
        }
    }
    *q = 0;
}

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", cacheGlyphsSJIS__10SystemFontFPc);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088919DC);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08891A48);

void SystemFont::printfUtf8x(char *format, ...) {
    char buffer[0x300];
    memset(buffer, 0, sizeof(buffer));

    va_list va_args;
    va_start(va_args, format);
    vsnprintf(buffer, sizeof(buffer), format, va_args, Encoding::UTF8);
    va_end(va_args);

    printTextUtf8x(-1, (u8 *)buffer);
}

void SystemFont::printfUtf8x(s16 left, s16 top, char *format, ...) {
    char buffer[0x300];

    setCursor(left, top);

    memset(buffer, 0, sizeof(buffer));

    va_list va_args;
    va_start(va_args, format);
    vsnprintf(buffer, sizeof(buffer), format, va_args, Encoding::UTF8);
    va_end(va_args);

    printTextUtf8x(-1, (u8 *)buffer);
}

void SystemFont::printfUtf8x(s16 left, s16 top, s8 fontColor, char *format, ...) {
    char buffer[0x300];
    va_list va_args;

    setCursor(left, top);
    setFontColor(fontColor);

    memset(buffer, 0, sizeof(buffer));

    va_start(va_args, format);
    vsnprintf(buffer, sizeof(buffer), format, va_args, Encoding::UTF8);
    va_end(va_args);

    printTextUtf8x(-1, (u8 *)buffer);
}

extern "C"
char *strncpy(char *dst, const char *src, u32 n);
extern "C"
char *strncat(char *dst, const char *src, u32 n);

void SystemFont::printTextUtf8x(s16 n, u8 *utf8x) {
    char bufferOne[0x300];
    char bufferTwo[0x300];
    char *(buffers[2]);
    s16 command;

    zero(buffers, sizeof(buffers));

    char *p = bufferOne;
    int raw = false;
    int i, b = 0;

    buffers[0] = bufferOne;
    buffers[1] = bufferTwo;

    strncpy(buffers[0], (char *)utf8x, sizeof(bufferOne));
    bufferOne[0x2FF] = 0;

    memset(bufferTwo, 0, sizeof(bufferTwo));

    s16 indent = this->left;
    s16 left = indent;
    s16 top = this->top;

    i = 0;
    while (true) {
        char c = *p;
        if (c == 0) {
            break;
        }
        if (c == '~') {
            if (raw) {
                char *tmp = p;
                p = parseCommand(p, &command, ParseResult::COMMAND, Encoding::UTF8);
                if ((command & 0xFF00) != 0x800) {
                    p = tmp;
                } else {
                    raw = false;
                    continue;
                }
            } else {
                // flush
                buffers[b ^ 1][i] = 0;
                printfUtf8("%s", buffers[b ^ 1]);
                i = 0;
                setCursor(left, top);

                p = parseCommand(p, &command, ParseResult::COMMAND, Encoding::UTF8);
                switch (command & 0xFF00) {
                case 0:
                default:
                    break;
                case 0x100:
                    setFontColor(command & 0xFF);
                    continue;
                    break;
                case 0x200:
                    b ^= 1;
                    copySubstitution(buffers[b], command & 0xFF, Encoding::UTF8);
                    strncat(buffers[b], p, 0x300 - CCC::objectPtr->encodedSizeUtf8((u8 *)buffers[b]));
                    p = buffers[b];
                    p[0x2FF] = 0;
                    continue;
                    break;
                case 0x400:
                    addIcon(left, top, fontHeight, fontColor, command & 0xFF);
                    left += fontWidth;
                    setCursor(left, top);
                    if (n > 0) {
                        --n;
                    }
                    if (!n) {
                        *p = 0;
                    }
                    continue;
                    break;
                case 0x800:
                    raw = true;
                    continue;
                    break;
                }
                if (*p == 0) {
                    break;
                }
            }
        }

        int type;
        if (widthUtf8((u8 *)p) == 2) {
            type = 0; // fullwidth
            if (n > 0) {
                --n;
            }
        } else if (*p == '\n') {
            type = 2; // newline
        } else {
            type = 1; // halfwidth
            if (n > 0) {
                --n;
            }
        }

        int len = CCC::objectPtr->codepointLengthUtf8(*p);
        if (i + len < 0x2FF) {
            for (int j = 0; j < len; ++j) {
                char c = *p++;
                buffers[b ^ 1][i] = c;
                ++i;
            }
        } else {
            break;
        }

        if (i >= 0x2FF) {
            i = 0x2FF;
            break;
        }

        if (type == 0) {
            left += fontWidth;
        } else if (type == 1) {
            left += fontWidth / 2;
        } else {
            left = indent;
            top += lineSpacing;
            buffers[b ^ 1][i - 1] = 0;
            printfUtf8("%s", buffers[b ^ 1]);
            i = 0;
            setCursor(indent, top);
        }

        if (n) {
            continue;
        }
        *p = 0;
    }
    buffers[b ^ 1][i] = 0;
    printfUtf8("%s", buffers[b ^ 1]);
}

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088921C8);

void SystemFont::drawBtnIcon(Icon *icons, u8 count, u32 renderGroup) {
    using namespace immediate_ge;

    GeTexture texture;

    tmh_header *tmh = (tmh_header *)ResourceManager::objectPtr->find(ResourceType::RESIDES_TMH);
    Ge::objectPtr->method_08859768(tmh, 1, 0, 0, &texture);

    u32 *start = Ge::objectPtr->write_head();
    u32 *out = start;
    ge::ztestenable(&out, false);
    ge::ztest(&out, GE_OP_NEVER);
    ge::alphablendenable(&out, true);
    ge::blendmode(&out, GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);
    ge::alphatestenable(&out, true);
    ge::atest(&out, 0xFF, 0x00, GE_OP_NOT_EQUALS);
    ge::texfunc(&out, GE_TEXFUNC_MODULATE, GE_TEXFUNC_COMPONENTS_RGBA);
    ge::texmapmode(&out, GE_TEXMAP_TEXTURE_COORDS, GE_PROJMAP_POSITION);
    ge::texwrap(&out, GE_TWRAP_CLAMP, GE_TWRAP_CLAMP);
    ge::texfilter(&out, GE_TFILT_LINEAR, GE_TFILT_LINEAR);
    ge::texmode(&out, GE_TEXMODE_SWIZZLE);
    ge::loadclut(&out, texture);
    ge::texflush(&out);
    ge::jump(&out, NULL);
    Ge::objectPtr->method_088595E8(start, 22U, renderGroup);
    Ge::objectPtr->set_write_head(start + 22);

    if (count > 0) {
        Icon *icon = icons;
        while (count > 0) {
            u8 id = icon->iconId;
            const IconAtlasCoordinate &coord = ICON_COORDINATES[id];
            u8 left = coord.left;
            u8 top = coord.top;
            u8 width = coord.width;
            u8 height = coord.height;

            u32 color;
            if (id - 10U < 2U) {
                if (icon->fontColor >= 48) {
                    icon->fontColor = 0;
                }
                if (icon->fontColor < 0) {
                    color = glowingFontColor;
                } else {
                    color = fontColors[icon->fontColor];
                }
                color |= 0xFF000000;
            } else {
                color = 0xFFFFFFFF;
            }

            u32 *start = Ge::objectPtr->write_head();
            u32 *out = start;
            *out++ = (top << 16) | (u16)left;
            *out++ = color;
            *out++ = (icon->top << 16) | (u16)icon->left;
            *out++ = (u16)cursorZ;
            *out++ = ((top + height) << 16) | (left + width);
            *out++ = color;
            *out++ = ((icon->top + icon->size) << 16) | (u16)(icon->left + icon->size);
            *out++ = (u16)cursorZ;
            ge::vaddr(&out, start);
            ge::texturemapenable(&out, true);
            ge::shademode(&out, GE_SHADE_GOURAUD);
            ge::vertextype(&out,
                GE_VTYPE_TC_16BIT,
                GE_VTYPE_COL_8888,
                GE_VTYPE_NRM_NONE,
                GE_VTYPE_POS_16BIT,
                GE_VTYPE_WEIGHT_NONE,
                GE_VTYPE_IDX_NONE,
                0,
                0,
                true);
            ge::prim(&out, GE_PRIM_RECTANGLES, 2);
            ge::jump(&out, 0);

            Ge::objectPtr->method_088595E8(start + 8, 8, renderGroup);
            Ge::objectPtr->set_write_head(start + 16);

            --count;
            ++icon;
        }
    }
}

// adapted from PPSSPP implementation of libfont
struct FontNewLibParams {
	void *userDataAddr;
	u32 numFonts;
	void *cacheDataAddr;

	void *allocFuncAddr;
	void *freeFuncAddr;
	void *openFuncAddr;
	void *closeFuncAddr;
	void *readFuncAddr;
	void *seekFuncAddr;
	void *errorFuncAddr;
	void *ioFinishFuncAddr;
};

void *fontAlloc(void *, int);
void fontFree(void *, void *);

FontNewLibParams D_eboot_089AA380 = {
    .numFonts = 2,
    .allocFuncAddr = (void *)fontAlloc,
    .freeFuncAddr = (void *)fontFree,
};

extern "C" {
    void *sceFontNewLib(FontNewLibParams *params, int *errorOut);
    s32 sceFontGetNumFontList(void *fontLib, int *errorOut);
    s32 sceFontFindOptimumFont(void *fontLib, PGFFontStyle *style, int *errorOut);
    void *sceFontOpen(void *fontLib, s32 fontId, u32 mode, int *errorOut);
    int sceFontGetFontInfo(void *font, PGFFontInfo *infoOut);
}

#define FONT_FAMILY_DEFAULT 0
#define FONT_STYLE_DEFAULT 0
#define FONT_LANGUAGE_JAPANESE 1
#define FONT_OPEN_INTERNAL_STINGY 0

void SystemFont::initializeFont() {
    int error;
    fontLib = sceFontNewLib(&D_eboot_089AA380, &error);
    sceFontGetNumFontList(fontLib, &error);
    PGFFontStyle style;
    memset(&style, 0, sizeof(style));
    style.fontFamily = FONT_FAMILY_DEFAULT;
    style.fontStyle = FONT_STYLE_DEFAULT;
    style.fontLanguage = FONT_LANGUAGE_JAPANESE;
    fontId = sceFontFindOptimumFont(fontLib, &style, &error);
    font = sceFontOpen(fontLib, fontId, FONT_OPEN_INTERNAL_STINGY, &error);
    error = sceFontGetFontInfo(font, &fontInfo);
}

void *fontAlloc(void *unused, int size) {
    if (size == 0) {
        size = 4;
    }
    void *p = GLYPH_CACHE.alloc(size, 4);
    if (p == NULL) {
        return NULL;
    }
    slab *s = (slab *)((int)p - 0x20);
    GLYPH_CACHE_USED += 0x10 * s->free_block_start;
    if (GLYPH_CACHE_MAX_USED < GLYPH_CACHE_USED) {
        GLYPH_CACHE_MAX_USED = GLYPH_CACHE_USED;
    }
    return p;
}

void fontFree(void *unused, void *p) {
    slab *s = (slab *)((int)p - 0x20);
    GLYPH_CACHE_USED -= 0x10 * s->free_block_start;
    GLYPH_CACHE.free(p);
}


INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08892A70);

extern "C"
void func_eboot_08892B60(void) {
}

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08892B68);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08892C58);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08892D68);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08892E50);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08892F04);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08893230);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08893348);

int SystemFont::vsnprintf(char *buffer, int n, char *format, va_list args, u8 encoding) {
    char conv[0x300];
    char spec[0x300];
    int i = 0;
    int j;
    int isParsing = true;
    int inSpecification = false;
    int convLen;
    int specLen;
    while (isParsing) {
        u8 c = *format;
        switch (c) {
        case 0:
            buffer[i] = c;
            isParsing = false;
            break;
        case '%':
            inSpecification = true;
            conv[0] = 0;
            specLen = 1;
            while (inSpecification) {
                u8 c = format[specLen];
                switch (c) {
                case 'd':
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    sprintf(conv, spec, va_arg(args, int));
                    inSpecification = false;
                    break;
                case 'u':
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    sprintf(conv, spec, va_arg(args, unsigned int));
                    inSpecification = false;
                    break;
                case 'x':
                case 'X':
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    sprintf(conv, spec, va_arg(args, int));
                    inSpecification = false;
                    break;
                case 'o':
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    sprintf(conv, spec, va_arg(args, int));
                    inSpecification = false;
                    break;
                case 'b': {
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    u32 x = va_arg(args, u32);
                    int count = 32;
                    int mult = 1;
                    if (specLen > 2) {
                        count = 0;
                        for (j = specLen - 2; j > 0; --j) {
                            count += (spec[j] - '0') * mult;
                            mult *= 10;
                        }
                        if (count > 0x20) {
                            count = 0x20;
                        }
                    }
                    for (j = 0; j < count; ++j) {
                        if (x & (1 << ((count - 1) - j))) {
                            conv[j] = '1';
                        } else {
                            conv[j] = '0';
                        }
                    }
                    conv[j] = 0;
                    inSpecification = false;
                    break;
                }
                case 'f': {
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    if ((int)args & 7) {
                        *(char **)&args += 8 - ((int)args & 7);
                    }
                    int shift;
                    if ((int) args & 7) {
                        shift = 4;
                    } else {
                        shift = 0;
                    }
                    *(char **)&args += shift + 8;
                    sprintf(conv, spec, *((u32 *)args - 2), *((u32 *)args - 1));
                    inSpecification = false;
                    break;
                }
                case 'c':
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    sprintf(conv, spec, (char)va_arg(args, int));
                    inSpecification = false;
                    break;
                case 's':
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    sprintf(conv, spec, va_arg(args, char *));
                    inSpecification = false;
                    break;
                case '%': {
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    int count = 1;
                    int mult = 1;
                    if (specLen > 2) {
                        count = 0;
                        for (j = specLen - 2; j > 0; --j) {
                            count += (spec[j] - '0') * mult;
                            mult *= 10;
                        }
                    }
                    for (j = 0; j < count; ++j) {
                        conv[j] = '%';
                    }
                    conv[j] = 0;
                    inSpecification = false;
                    break;
                }
                case 0:
                    ++specLen;
                    memcpy(spec, format, specLen);
                    spec[specLen] = 0;
                    strcpy(conv, spec);
                    inSpecification = false;
                    isParsing = false;
                    break;
                default:
                    if ((c < '0' || c > '9') && c != '.' && c != '-' && c != '+' && c != '#' && c != ' ') {
                        ++specLen;
                        memcpy(spec, format, specLen);
                        spec[specLen] = 0;
                        strcpy(conv, spec);
                        inSpecification = false;
                    } else {
                        ++specLen;
                    }
                    break;
                }
            }
            if (encoding == Encoding::UTF8) {
                convLen = CCC::objectPtr->encodedSizeUtf8((u8 *)conv);
            } else {
                convLen = ::strlen(conv);
            }
            if (i + convLen >= n - 1) {
                buffer[i] = 0;
                isParsing = false;
            } else {
                strcat(buffer, conv);
                i += convLen;
                format += specLen;
            }
            break;
        default:
            if (encoding == Encoding::UTF8) {
                int len = CCC::objectPtr->codepointLengthUtf8(c);
                switch (len) {
                default:
                    buffer[i] = 0;
                    isParsing = false;
                    break;
                case 1:
                    buffer[i++] = c;
                    break;
                case 2:
                    if (i + 2 >= n - 1) {
                        buffer[i] = 0;
                        isParsing = false;
                    } else {
                        buffer[i] = c;
                        buffer[i + 1] = *++format;
                        i += 2;
                    }
                    break;
                case 3:
                    if (i + 3 >= n - 1) {
                        buffer[i] = 0;
                        isParsing = false;
                    } else {
                        buffer[i] = c;
                        buffer[i + 1] = format[1];
                        format += 2;
                        buffer[i + 2] = *format;
                        i += 3;
                    }
                    break;
                case 4:
                    if (i + 4 >= n - 1) {
                        buffer[i] = 0;
                        isParsing = false;
                    } else {
                        buffer[i] = c;
                        buffer[i + 1] = format[1];
                        buffer[i + 2] = format[2];
                        format += 3;
                        buffer[i + 3] = *format;
                        i += 4;
                    }
                    break;
                }
                ++format;
            } else {
                if ((c >= 0x20 && c <= 0x7E) || c == '\n') {
                    buffer[i] = c;
                    ++i;
                } else if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c < 0x100)) {
                    if (i + 2 >= n - 1) {
                        buffer[i] = 0;
                        isParsing = false;
                    } else {
                        buffer[i] = c;
                        ++format;
                        c = *format;
                        buffer[i + 1] = c;
                        i += 2;
                        if (c == 0) {
                            isParsing = false;
                        }
                    }
                } else {
                    buffer[i] = c;
                    ++i;
                }
                ++format;
            }
            if (i >= n - 1) {
                i = n - 1;
                buffer[i] = 0;
                isParsing = false;
            }
            break;
        }
    }
    return i;
}

void SystemFont::decode(u8 *str, u8 encoding) {
    u16 *p = (u16 *)(codepointBuffer + codepointBufferUsed);
    int overflow = 0;
    while (*str) {
        u16 codepoint;
        if (encoding == Encoding::UTF8) {
            codepoint = CCC::objectPtr->decodeUtf8(&str);
        } else {
            codepoint = CCC::objectPtr->decodeSJIS(&str);
            codepoint = CCC::objectPtr->jisToUcs2(codepoint);
        }
        *p++ = codepoint;
        codepointBufferUsed += 2;
        if (codepointBufferUsed >= 0x3000) {
            codepointBufferUsed = 0x2FFE;
            ++overflow;
            p = (u16 *)(codepointBuffer + codepointBufferUsed);
        }
    }
    *p = 0;
    if (overflow == 0) {
        codepointBufferUsed += 2;
        if (codepointBufferUsed >= 0x3000) {
            codepointBufferUsed = 0x2FFE;
        }
    }
}

int SystemFont::isHalfWidth(u16 codepoint) {
    if (codepoint >=  0x250U && codepoint < 0x2000U) {
        return 0;
    }
    if (codepoint >= 0x2000U && codepoint < 0x2070U) {
        return 0;
    }
    if (codepoint >= 0x2070U && codepoint < 0x20A0U) {
        return 0;
    }
    if (codepoint >= 0x20A0U && codepoint < 0x20D0U) {
        return 1;
    }
    if (codepoint >= 0x20D0U && codepoint < 0xFF60U) {
        return 0;
    }
    if (codepoint >= 0xFF60U && codepoint < 0xFFE0U) {
        return 2;
    }
    if (codepoint >= 0xFFE0U && codepoint < 0xFFF0U) {
        return 0;
    }
    return 1;
}

u32 SystemFont::currentRenderGroup() {
    switch (currentLayer) {
    default:
    case 0:
        return 9;
    case 1:
        return 10;
    case 2:
        return 11;
    case 3:
        return 12;
    case 4:
        return 13;
    case 5:
        return 14;
    }
}

void SystemFont::updateGlowingFontColor( ) {
    u16 angle = (u16)((frameCount & 0x1F) << 11);
    float x, y = (3.1415927f * angle) / 32768.0f;
    x = vsin_s_slow(y);
    u8 r = (s8)(s32)(x * 64.0f) + 0x50,
       g = (s8)(s32)(x * 20.0f) + 0xE4,
       b = (s8)(s32)(x * 7.0f) + 0xF7;
    setColor(-1, 0, 0xFF000000 | r << 16 | g << 8 | b);
}

char *D_eboot_089AA4B8 =
  "0 1 2 3 4 5 6 7 8 9 \x82\xcc\x81\x42"
  "\x82\xe9\x82\xf0\x82\xc9\x83\x58\x81\x5b\x82\xa2"
  "\x82\xbd\x83\x93\x81\x41\x82\xb5\x82\xaa\x82\xc8"
  "\x82\xea\x82\xc6\x82\xc5\x83\x8b\x83\x43\x82\xcd"
  "\x82\xc4\x97\x70\x82\xb7\x83\x67\x91\x95\x94\xf5"
  "\x83\x5e\x82\xe7\x82\xb3\x83\x8c\x8b\xad\x82\xe0"
  "\x82\xe8\x82\xad\x83\x89\x83\x41\x83\x68\x97\xcd"
  "\x82\xdc\x82\xa9\x83\x4e\x83\x8a\x82\xb1\x82\xab"
  "\x90\xab\x82\xa4\x96\x68\x89\xbb\x82\xc1\x83\x62"
  "\x83\x6e\x83\x80\x82\xa6\x82\xc2\x97\xb3\x83\x4b"
  "\x83\x74\x82\xe6\x83\x49\x83\x82\x83\x8d\x81\x49"
  "\x91\xe5\x82\xaf\x8f\xe3\x8d\xec\x82\xb6\x8d\x82"
  "\x82\xa0\x82\xbe\x8b\xef\x91\x66\x8d\xde\x83\x4f"
  "\x82\x66\x89\xc1\x83\x7b\x91\xcf\x8d\xc5\x8c\xe4"
  "\x82\x72\x92\x65\x83\x45\x83\x75\x82\xdf\x82\xe1"
  "\x81\x79\x81\x7a\x81\xe1\x81\xe2\x82\xbb\x83\x65"
  "\x83\x4c\x83\x81\x8c\x82\x82\xb9\x97\xd8\x8a\x6b"
  "\x8e\x67\x93\xc1\x83\x7c\x89\xce\x82\xf1\x8c\x95"
  "\x8d\x48\x82\xed\x81\x46\x97\xb4\x8e\xe8\x83\x76"
  "\x90\xb6\x8d\x87\x82\xe2\x83\x83\x95\x90\x83\x57"
  "\x95\x69\x90\x46\x8d\x62\x94\x5c\x88\xea\x83\x4d";

void SystemFont::cacheCommonGlyphs() {
    // TODO: replace with string literal once source file can be encoded in Shift-JIS
    cacheGlyphsSJIS(D_eboot_089AA4B8);
}

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", parseCommand__10SystemFontFPcPsii);

void SystemFont::copySubstitution(char *dst, s16 substitutionId, u8 encoding) {
    char buffer[0x300];
    int value;
    switch (substitutionId) {
    case 0:
    case 1:
        if (substitutionId == 0) {
            // TODO: sizeof( savedata type? )
            value = 1824;
        } else {
            // TODO: sizeof( userdata type? )
            value = 580;
        }
        sprintf(buffer, "%d", value);
        if (encoding == Encoding::UTF8) {
            asciiToFullWidthUtf8(buffer, dst);
        } else {
            asciiToFullWidthSJIS(buffer, dst);
        }
        break;
    case 2:
        buffer[0] = '0';
        buffer[1] = 0;
        if (encoding == Encoding::UTF8) {
            asciiToFullWidthUtf8(buffer, dst);
        } else {
            asciiToFullWidthSJIS(buffer, dst);
        }
        break;
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894A78);

void SystemFont::addIcon(s16 left, s16 top, u16 size, s8 fontColor, s16 iconId) {
    if (layerIconCounts[layer] < 10) {
        Icon &icon = layerIcons[layer][layerIconCounts[layer]];
        icon.left = left;
        icon.top = top;
        icon.size = size;
        icon.iconId = iconId;
        icon.fontColor = fontColor;
        ++layerIconCounts[layer];
    }
}

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894D88);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894DC8);
