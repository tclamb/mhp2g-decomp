#include "toast_notification.hpp"
#include "game_sys.hpp"
#include "sound.hpp"
#include "system_font.hpp"


extern "C" {
    char *strchr(const char *, int);
    void func_eboot_0888368C(Sound *, int, int, int, int, int, int, int, int);
    char *func_eboot_088514B8(GameSys *, u16);

    extern s16 D_eboot_0899A1B0;
    extern s16 D_eboot_0899A1B2;
    extern char D_eboot_0899A1B4[];
    extern char D_eboot_0899A1B8[];
    extern char D_eboot_0899A1C0[];
    extern char D_eboot_0899A1CC[];
    extern char D_eboot_0899A1D8[];
    extern u16 D_eboot_089A9A0C[];
    extern u16 D_eboot_089A9A40[];
    extern u16 D_eboot_089A9A68[];
    extern u16 D_eboot_089A9A7C[];
    extern u16 D_eboot_089A9A84;
    extern u16 D_eboot_089A9A86;
    extern u16 D_eboot_089A9A88;
    extern u16 D_eboot_089A9A8A;
    extern u16 D_eboot_089AA054[];
}

// Original toast .data at 0x0899A1B0..0x0899A214. Keep symbol order and
// explicit string padding: both toast translation units share these objects.
extern "C" {
s16 D_eboot_0899A1B0 = 240;
s16 D_eboot_0899A1B2 = 144;
char D_eboot_0899A1B4[4] = "%s";
char D_eboot_0899A1B8[8] = "%s%s";
char D_eboot_0899A1C0[12] = "          ";
char D_eboot_0899A1CC[12] = "%s%s%d%s";
char D_eboot_0899A1D8[12] = "%s  %s%s";
u16 D_eboot_0899A1E4 = 0;
u16 D_eboot_0899A1E6 = 0;
u16 D_eboot_0899A1E8 = 0;
u16 D_eboot_0899A1EA = 0;
u8 D_eboot_0899A1EC = 14;
u8 D_eboot_0899A1ED = 14;
u8 D_eboot_0899A1EE = 0;
u8 D_eboot_0899A1EF = 0;
u16 D_eboot_0899A1F0 = 0;
u16 D_eboot_0899A1F2 = 0;
u32 D_eboot_0899A1F4 = 16;
char D_eboot_0899A1F8[28] = "~C02%s~C00%s~C05%d~C00%s";
}


// Size and position the notification before its first draw.
static inline int textWidth(s16 n) { return n * 7; }
extern "C" void func_eboot_08859C34(ToastNotification *this_) {
    char buf[0x200];
    this_->state++;
    this_->phase = 0;
    func_eboot_0888368C(Sound::objectPtr, 0, 0x10, 0, 0, 0, 0, 0, 0);
    switch (this_->type) {
    case 0:
        sprintf(buf, D_eboot_0899A1B4, GameSys::objectPtr->method_0885143C(D_eboot_089A9A0C[this_->index]));
        break;
    case 1: {
        char *s1 = GameSys::objectPtr->method_08851448(this_->param);
        sprintf(buf, D_eboot_0899A1B8, s1, GameSys::objectPtr->method_0885143C(D_eboot_089A9A40[this_->index]));
        break;
    }
    case 2: {
        char *s1 = GameSys::objectPtr->method_0885143C(D_eboot_089A9A68[this_->index]);
        sprintf(buf, D_eboot_0899A1B8, s1, GameSys::objectPtr->method_08851448(this_->param));
        break;
    }
    case 3:
        sprintf(buf, D_eboot_0899A1B8, D_eboot_0899A1C0, GameSys::objectPtr->method_0885143C(D_eboot_089A9A7C[this_->index]));
        break;
    case 4:
    case 6:
        sprintf(buf, D_eboot_0899A1B4, this_->text);
        break;
    case 5: {
        char *s1 = GameSys::objectPtr->method_08851448(this_->index);
        char *s2 = GameSys::objectPtr->method_0885143C(D_eboot_089A9A84);
        sprintf(buf, D_eboot_0899A1CC, s1, s2, this_->param, GameSys::objectPtr->method_0885143C(D_eboot_089A9A86));
        break;
    }
    case 7: {
        char *s1 = GameSys::objectPtr->method_0885143C(D_eboot_089A9A88);
        char *s2 = GameSys::objectPtr->method_0885143C(D_eboot_089AA054[this_->index]);
        sprintf(buf, D_eboot_0899A1D8, s1, s2, GameSys::objectPtr->method_0885143C(D_eboot_089A9A8A));
        break;
    }
    case 8: {
        char *s1 = func_eboot_088514B8(GameSys::objectPtr, this_->param);
        sprintf(buf, D_eboot_0899A1B8, s1, GameSys::objectPtr->method_0885143C(D_eboot_089A9A40[this_->index]));
        break;
    }
    }

    char *line = buf;
    s16 maxHalf;
    s32 lines = 0;
    maxHalf = 0;
    char *search = line;
    for (;;) {
        char *nl = strchr(search, 0xA);
        if (nl == NULL) {
            int w = SystemFont::objectPtr->halfWidthsX(line);
            if (maxHalf < w) {
                maxHalf = w;
            }
            break;
        }
        lines++;
        int w = SystemFont::objectPtr->lineHalfWidthsX(line);
        if (maxHalf < w) {
            maxHalf = w;
        }
        line = search = nl + 1;
    }
    this_->height = lines * 16 + 32;
    s32 boxW = textWidth(maxHalf - 1) + 32;
    this_->width = (boxW / 16) * 16;
    if (boxW % 16 != 0) {
        this_->width += 16;
    }
    // The signed pair and the later unsigned alias retain the original stack copy.
    // An extra cast of uc[1] changes floating-point scheduling in mwccpsp.
    s16 c[2];
    c[0] = D_eboot_0899A1B0;
    c[1] = D_eboot_0899A1B2;
    u16 *uc = (u16 *)&c;
    u16 cx = uc[0];
    this_->left = cx - (this_->width >> 1);
    this_->timer = 60;
    this_->speed = ((float)uc[1] - (float)(this_->height >> 1)) / 3.0f;
    this_->top = 0.0f;
    this_->textLeft = cx - (textWidth((int)maxHalf) >> 1);
    func_eboot_0885A16C(this_);
}
