#include "toast_notification.hpp"

// Per-frame update of one toast (called for each active Cockpit toast).
extern "C" void func_eboot_0885A0E8(ToastNotification *this_) {
    switch (this_->state) {
    case 0:
        func_eboot_08859C34(this_);
        break;
    case 1:
        func_eboot_0885A16C(this_);
        break;
    case 2:
        func_eboot_0885A8D8(this_);
        break;
    case 3:
        func_eboot_0885A8E8(this_);
        break;
    }
}
