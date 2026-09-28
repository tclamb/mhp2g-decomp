#include "toast_notification.hpp"
#include "cockpit.hpp"
#include "game_sys.hpp"

// GameSys+0x381: a byte the toast owner hands over when a toast starts and resets to 0xFF when it
// starts sliding out (func_eboot_0885A16C).
static inline void setSysFlag381(u8 v) { *((u8 *)GameSys::objectPtr + 0x381) = v; }

// state 2 -> 3
extern "C" void func_eboot_0885A8D8(ToastNotification *this_) {
    this_->state++;
}

// close: frees the text of a type-6 toast (allocated from the Cockpit cache)
extern "C" void func_eboot_0885A8E8(ToastNotification *this_) {
    this_->active = 0;
    this_->phase = 0;
    this_->state = 0;
    if (this_->type == 6) {
        Cockpit::objectPtr->cache.free(this_->text);
    }
}

// start a table-driven toast (types 0..3, 5, 7, 8)
extern "C" void func_eboot_0885A92C(ToastNotification *this_, u8 type, s16 index, s16 param, u8 sysFlag) {
    this_->type = type;
    this_->index = index;
    this_->param = param;
    this_->active = 1;
    this_->state = 0;
    setSysFlag381(sysFlag);
}

// start a toast showing caller text (type 4, or type 6 when the text is heap memory to free on close)
extern "C" void func_eboot_0885A954(ToastNotification *this_, char *text, u8 sysFlag, int heapText) {
    if (heapText == 0) {
        this_->type = 4;
    } else {
        this_->type = 6;
    }
    this_->active = 1;
    this_->state = 0;
    this_->text = text;
    setSysFlag381(sysFlag);
}
