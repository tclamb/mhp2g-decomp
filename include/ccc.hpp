#pragma once

#include "common.h"

struct CCC {
    u16 decodeSJIS(u8 **str);
    u16 decodeUtf8(u8 **str);
    void encodeUtf8(u8 **dst, u32 codepoint);
    u16 jisToUcs2(u32 jis);
    int sjisToUtf8(u8 *dst, s32 n, u8 *str);
    int utf8ToUcs2(u16 *dst, s32 n, u8 *str);
    bool isValidUcs4(u32 codepoint);
    bool isValidUcs2(u32 codepoint);
    bool isValidUtf8(u32 codepoint);
    int strlenUcs2(u16 *str);
    u16 *strstrUcs2(u16 *haystack, u16 *needle);
    int islowerUcs2(u16 codepoint);
    int codepointLengthUtf8(u8 prefixByte);
    int encodedSizeUtf8(u8 *str);
};
