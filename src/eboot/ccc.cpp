#include "ccc.hpp"
#include "singleton.hpp"

template <> CCC *Singleton<CCC>::objectPtr;

u16 CCC::decodeSJIS(u8 **src) {
    u8 *p = *src;
    u8 c = *p++;
    u16 codepoint;
    if ((c <= 0x7FU) || (0xA0U <= c && c <= 0xDFU) || (0xFDU <= c)) {
        codepoint = c;
    } else {
        int x = 0, y = 0, z = 0;
        if (0x81U <= c && c <= 0x9FU) {
            y = ((u32)c - 0x81) * 2 + 1;
        } else if (0xE0U <= c && c <= 0xEFU) {
            y = ((u32)c - 0xE0U) * 2 + 0x3F;
        } else if (0xF0U <= c && c <= 0xFCU) {
            x = 1;
            y = ((u32)c - 0xF0U) * 2 + 7;
            if (0xF4 <= c) {
                y += 0x3E;
            }
        }
        if (y != 0) {
            c = *p++;
            if (0x40U <= c && c < 0x7FU) {
                z = (u32)c - 0x3F;
            } else if (0x80U <= c && c < 0x9FU) {
                z = (u32)c - 0x40;
            } else if (0x9FU <= c && c < 0xFDU) {
                z = (u32)c - 0x9E;
                ++y;
            }
            if (x != 0) {
                if (y == 0x7) {
                    y = 1;
                } else if (y == 0xB) {
                    y = 5;
                } else if (y == 0x4D) {
                    y = 0xF;
                }
            }
        }
        if ((y == 0) || (z == 0)) {
            codepoint = 0x3F;
        } else {
            codepoint = (x << 0xF) |  ((y + 0x20) << 8) | (z + 0x20);
        }
    }
    *src = p;
    return codepoint;
}

u16 CCC::decodeUtf8(u8 **src) {
    u8 *p = *src;
    u8 c = *p++;
    u32 codepoint;
    if (c < 0x80U) {
        codepoint = c;
    } else if (0xC2U > c || c > 0xF4U) {
        // invalid prefix byte range
        codepoint = 0x3F;
    } else {
        u32 x = 0x40;
        codepoint = c & 0x7F;
        do {
            codepoint &= x - 1;
            x <<= 5;
            codepoint = (codepoint << 6) | (*p++ & 0x3F);
        } while (codepoint >= x);
    }
    *src = p;
    if ((u8) isValidUcs2(codepoint) == false) {
        codepoint = 0x3F;
    }
    return codepoint;
}

void CCC::encodeUtf8(u8 **dst, u32 codepoint) {
    u8 *p = *dst;
    if ((u8) isValidUtf8(codepoint) == false) {
        codepoint = 0x3F;
    }
    if (codepoint < 0x80U) {
        *p++ = codepoint;
    } else {
        u32 x = 0x800;
        int n = 1;
        while (codepoint >= x) {
            x <<= 5;
            ++n;
        }
        p += n + 1;
        u8 *q = p;
        u8 m = (1 << (7 - n)) - 1;
        while (n--) {
            --q;
            *q = (codepoint & 0x3F) | 0x80;
            codepoint >>= 6;
        }
        *--q = (u8)~m | (codepoint & (m >> 1));
    }
    *dst = p;
}

// TODO: generate from open source mapping table if possible
#include "../../assets/eboot/JIS_TO_UCS2.jis2ucs.inc.hpp"

u16 CCC::jisToUcs2(u32 jis) {
    if (jis == 0) {
        return 0;
    } else {
        u16 codepoint = ((u16 *)JIS_TO_UCS2)[(u16)jis];
        if (codepoint == 0) {
            return 0x3F;
        } else {
            return codepoint;
        }
    }
}

int CCC::sjisToUtf8(u8 *dst, s32 n, u8 *src) {
    u8 buf[4];
    int len = 0;
    if (n < 1) {
        return 0;
    }
    --n;
    int jis;
    while ((jis = decodeSJIS(&src))) {
        u8 *q = buf;
        u16 codepoint = jisToUcs2(jis);
        if (codepoint == 0) {
            break;
        }
        encodeUtf8(&q, codepoint);
        int x = q - buf;
        u8 *b = buf;
        if (n < x) {
            break;
        }
        n -= x;
        while (x--) {
            *dst++ = *b++;
        }
        ++len;
    }
    *dst = 0;
    return len;
}

int CCC::utf8ToUcs2(u16 *dst, s32 n, u8 *src) {
    int len = 0;
    u8 *p = src;
    if (n < 1) {
        return 0;
    }
    u16 codepoint;
    --n;
    while ((codepoint = decodeUtf8(&p))) {
        if (n < 1) {
            break;
        }
        --n;
        *dst++ = codepoint;
        ++len;
    }
    *dst = 0;
    return len;
}

bool CCC::isValidUcs4(u32 codepoint) {
    return codepoint < 0x80000000;
}

bool CCC::isValidUcs2(u32 codepoint) {
    return codepoint < 0x10000;
}

bool CCC::isValidUtf8(u32 codepoint) {
    // oops? technically surrogates are invalid
    return isValidUcs4(codepoint);
}

int CCC::strlenUcs2(u16 *src) {
    int len = 0;
    for (u16 *p = src; *p != 0; ++p) {
        ++len;
    }
    return len;
}

u16 *CCC::strstrUcs2(u16 *haystack, u16 *needle) {
    u16 *p = haystack;
    u16 *q = NULL;

    int big = Singleton<CCC>::objectPtr->strlenUcs2(haystack);
    int small = Singleton<CCC>::objectPtr->strlenUcs2(needle);

    if (big >= small) {
        while (*p != 0) {
            if (*p == *needle) {
                if (big >= small) {
                    if (memcmp(p, needle, 2 * small) == 0) {
                        q = p;
                        break;
                    } else {
                        --big;
                        ++p;
                    }
                    continue;
                } else {
                    break;
                }
            } else {
                --big;
                ++p;
            }
        }
    }
    return q;
}

int CCC::islowerUcs2(u16 codepoint) {
    if (codepoint >= 'a' && codepoint <= 'z') {
        return true;
    }
    return false;
}

int CCC::codepointLengthUtf8(u8 c) {
    if (c == 0) {
        return 0;
    } else if (!(c & 0x80)) {
        return 1;
    } else if ((c & 0x80) && (c & 0x40) && !(c & 0x20)) {
        return 2;
    } else if ((c & 0x80) && (c & 0x40) && (c & 0x20) && !(c & 0x10)) {
        return 3;
    } else if ((c & 0x80) && (c & 0x40) && (c & 0x20) && (c & 0x10)) {
        if (!(c & 0x8)) {
            return 4;
        }
    }
    return 0;
}

int CCC::encodedSizeUtf8(u8 *src) {
    u8 *p = src;
    u32 n = 0;
    while (*p != 0) {
        int x = codepointLengthUtf8(*p);
        n += x;
        p += x;
    }
    return n;
}
