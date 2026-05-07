#include "system_font.hpp"

#include "cache.hpp"
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
    decodeBufferUsed = 0;
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
    ++unknown_0x15E;
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

void SystemFont::setFontColor(s8 color) {
    fontColor = color;
}

void SystemFont::setLineSpacing(u8 spacing) {
    lineSpacing = spacing;
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

extern "C" u32 strlen(char *str);

u32 SystemFont::strlen(char *str) {
    return ::strlen(str);
}

char D_eboot_089AA26C[4] = "~";

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08890A30);

char D_eboot_089AA270[4] = "\n";

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08890B34);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08890C60);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08890D7C);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08890DB8);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08890E0C);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", method_08890F34__10SystemFontFssPce);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", method_08891070__10SystemFontFssScPce);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088911C0);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088912FC);

char D_eboot_089AA274[4] = "%s";

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_0889144C);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088914E8);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088915D4);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_0889161C);

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

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088918CC);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088919DC);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08891A48);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08891AC4);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", method_08891B68__10SystemFontFssPce);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08891C08);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08891CBC);

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

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", initializeFont__10SystemFontFv);

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

FontNewLibParams D_eboot_089AA380 = {
    .numFonts = 2,
    .allocFuncAddr = (void *)fontAlloc,
    .freeFuncAddr = (void *)fontFree,
};

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

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08893720);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894208);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", isHalfWidth__10SystemFontFUs);

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

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", updateGlowingFontColor__10SystemFontFv);

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

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", cacheCommonGlyphs__10SystemFontFv);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_088945B0);

char D_eboot_089AA4BC[] = "%d";

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894988);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894A78);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894D1C);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894D88);

INCLUDE_ASM("asm/eboot/nonmatchings/system_font", func_eboot_08894DC8);
