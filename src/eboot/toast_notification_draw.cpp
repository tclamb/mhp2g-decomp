#include "toast_notification.hpp"
#include "game_sys.hpp"
#include "system_font.hpp"
#include "cockpit.hpp"
#include "lobby_net.hpp"
extern "C" {
char *func_eboot_088514B8(GameSys *, u16);
extern u16 D_eboot_0899A1E4, D_eboot_0899A1E6, D_eboot_0899A1E8, D_eboot_0899A1EA;
extern u8 D_eboot_0899A1EC, D_eboot_0899A1ED, D_eboot_0899A1EE, D_eboot_0899A1EF;
extern u16 D_eboot_0899A1F0, D_eboot_0899A1F2;
extern u32 D_eboot_0899A1F4;
extern char D_eboot_0899A1B4[], D_eboot_0899A1B8[], D_eboot_0899A1C0[];
extern char D_eboot_0899A1D8[], D_eboot_0899A1F8[];
extern u16 D_eboot_089A9A0C[], D_eboot_089A9A40[], D_eboot_089A9A68[], D_eboot_089A9A7C[];
extern u16 D_eboot_089A9A84, D_eboot_089A9A86, D_eboot_089A9A88, D_eboot_089A9A8A;
extern u16 D_eboot_089AA054[];
}


// Toast textbox, colored message rendering, and slide/hold/close animation.
extern "C" void func_eboot_0885A16C(ToastNotification *this_) {
    // Preserve the original fieldwise default copy, with boxLeft loaded last.
    CockpitTextbox box;
    box.boxTop = D_eboot_0899A1E6;
    box.boxWidth = D_eboot_0899A1E8;
    box.boxHeight = D_eboot_0899A1EA;
    box.fontWidth = D_eboot_0899A1EC;
    box.fontHeight = D_eboot_0899A1ED;
    box.fontColor = D_eboot_0899A1EE;
    box.boxStyle = D_eboot_0899A1EF;
    box.textLeft = D_eboot_0899A1F0;
    box.textTop = D_eboot_0899A1F2;
    box.textLineSpacing = D_eboot_0899A1F4;
    box.boxLeft = D_eboot_0899A1E4;
    box.boxLeft = this_->left;
    box.boxWidth = this_->width;
    box.boxHeight = this_->height;
    box.textLeft = this_->textLeft;
    box.boxTop = (u16)this_->top;
    // Read the stored unsigned coordinate through a fresh alias; forwarding it
    // changes the original float-to-coordinate conversion and load scheduling.
    box.textTop = ((u16 *)&box)[1] + 9;
    Cockpit::objectPtr->method_0882FE94((CockpitTextbox *)&box, 0, 255);
    switch (this_->type) {
    case 0:
        SystemFont::objectPtr->setFontColor(0);
        SystemFont::objectPtr->printfUtf8(box.textLeft, box.textTop, D_eboot_0899A1B4,
            GameSys::objectPtr->method_0885143C(D_eboot_089A9A0C[this_->index]));
        break;
    case 1: {
        SystemFont::objectPtr->setFontColor(2);
        SystemFont::objectPtr->printfUtf8(box.textLeft, box.textTop, D_eboot_0899A1B4,
            GameSys::objectPtr->method_08851448(this_->param));
        int width = SystemFont::objectPtr->halfWidths((u8 *)GameSys::objectPtr->method_08851448(this_->param));
        SystemFont::objectPtr->setFontColor(0);
        SystemFont::objectPtr->printfUtf8(box.textLeft + width * 7, box.textTop, D_eboot_0899A1B4,
            GameSys::objectPtr->method_0885143C(D_eboot_089A9A40[this_->index]));
        break;
    }
    case 2: {
        SystemFont::objectPtr->setFontColor(0);
        SystemFont::objectPtr->printfUtf8(box.textLeft, box.textTop, D_eboot_0899A1B4,
            GameSys::objectPtr->method_0885143C(D_eboot_089A9A68[this_->index]));
        int width = SystemFont::objectPtr->halfWidths((u8 *)GameSys::objectPtr->method_0885143C(D_eboot_089A9A68[this_->index]));
        SystemFont::objectPtr->setFontColor(2);
        SystemFont::objectPtr->printfUtf8(box.textLeft + width * 7, box.textTop, D_eboot_0899A1B4,
            GameSys::objectPtr->method_08851448(this_->param));
        break;
    }
    case 3:
        SystemFont::objectPtr->setFontColor(2);
        SystemFont::objectPtr->setFontSize(16, 14);
        SystemFont::objectPtr->print(box.textLeft, box.textTop,
            (u16 *)((u8 *)LobbyNet::objectPtr + 0xBC8 + *((u8 *)&this_->param) * 200));
        SystemFont::objectPtr->setFontSize(14, 14);
        SystemFont::objectPtr->setFontColor(0);
        SystemFont::objectPtr->printfUtf8(box.textLeft, box.textTop, D_eboot_0899A1B8, D_eboot_0899A1C0,
            GameSys::objectPtr->method_0885143C(D_eboot_089A9A7C[this_->index]));
        break;
    case 4:
    case 6:
        SystemFont::objectPtr->setFontColor(0);
        SystemFont::objectPtr->printfUtf8x(box.textLeft, box.textTop, D_eboot_0899A1B4, this_->text);
        break;
    case 5: {
        char *item = GameSys::objectPtr->method_08851448(this_->index);
        char *prefix = GameSys::objectPtr->method_0885143C(D_eboot_089A9A84);
        SystemFont::objectPtr->printfUtf8x(box.textLeft, box.textTop, D_eboot_0899A1F8,
            item, prefix, this_->param, GameSys::objectPtr->method_0885143C(D_eboot_089A9A86));
        break;
    }
    case 7: {
        SystemFont::objectPtr->setFontColor(0);
        char *prefix = GameSys::objectPtr->method_0885143C(D_eboot_089A9A88);
        char *name = GameSys::objectPtr->method_0885143C(D_eboot_089AA054[this_->index]);
        SystemFont::objectPtr->printfUtf8(box.textLeft, box.textTop, D_eboot_0899A1D8,
            prefix, name, GameSys::objectPtr->method_0885143C(D_eboot_089A9A8A));
        break;
    }
    case 8: {
        SystemFont::objectPtr->setFontColor(2);
        SystemFont::objectPtr->printfUtf8(box.textLeft, box.textTop, D_eboot_0899A1B4,
            func_eboot_088514B8(GameSys::objectPtr, this_->param));
        int width = SystemFont::objectPtr->halfWidths((u8 *)func_eboot_088514B8(GameSys::objectPtr, this_->param));
        SystemFont::objectPtr->setFontColor(0);
        SystemFont::objectPtr->printfUtf8(box.textLeft + width * 7, box.textTop, D_eboot_0899A1B4,
            GameSys::objectPtr->method_0885143C(D_eboot_089A9A40[this_->index]));
        break;
    }
    }
    switch (this_->phase) {
    case 0:
        this_->top += this_->speed;
        if (this_->top >= (float)(136 - (this_->height >> 1))) {
            this_->phase++;
            this_->top = 136 - (this_->height >> 1);
        }
        break;
    case 1:
        this_->timer--;
        if (this_->timer <= 0 || (this_->timer <= 30 && *(s8 *)((u8 *)Cockpit::objectPtr + 0x10F8) > 0)) {
            this_->speed *= -1.0f;
            *((u8 *)GameSys::objectPtr + 0x381) = 255;
            this_->phase++;
        }
        break;
    case 2:
        this_->top += this_->speed;
        if (this_->top <= 0.0f) {
            this_->state++;
        }
        break;
    }
}
