#include "common.h"

extern "C" void func_em01_09D23378(void *);
extern "C" void func_em01_09D235D8(void *);
extern "C" void func_em01_09D23818(void *);
extern "C" void func_em01_09D23B40(void *);

struct Em01 {
    u8 pad0[0x299];
    u8 unk299;
};

extern "C" void func_em01_09D24870(Em01 *this_) {
    switch (this_->unk299) {
    case 0:
        func_em01_09D23378(this_);
        break;
    case 1:
        func_em01_09D235D8(this_);
        break;
    case 2:
        func_em01_09D23818(this_);
        break;
    case 3:
        func_em01_09D23B40(this_);
        break;
    }
}
