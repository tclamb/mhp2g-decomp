#include "common.h"

struct Camera;

extern "C" void func_game_task_09AC04F8(void *, int, int, int);
extern "C" int func_eboot_08864400(void *, int, float);
extern "C" void func_eboot_08813D50(Camera *, int, void *);
extern "C" void func_em01_09D29C08(void *, int);
extern "C" bool func_em01_09D29F48(void *);
extern "C" void func_em01_09D152D0(void *, int, int);

template<typename T>
struct Singleton {
    static T *objectPtr;
};

struct Camera : Singleton<Camera> {
};

struct Em01Sub790 {
    u8 pad0[6];
    s16 unk6;
    u8 pad8[0x18 - 0x8];
    u8 unk18;
};

struct Em01 {
    u8 pad0[0x80];
    u8 unk80[0xBC - 0x80];
    u16 unkBC;
    u8 padBE[0x1D5 - 0xBE];
    u8 unk1D5;
    u8 pad1D6[0x204 - 0x1D6];
    float unk204;
    u8 pad208[0x248 - 0x208];
    float unk248;
    u8 pad24C[0x280 - 0x24C];
    u8 unk280;
    u8 pad281[0x2CC - 0x281];
    float unk2CC;
    u8 pad2D0[0x410 - 0x2D0];
    u32 unk410;
    u8 pad414[0x790 - 0x414];
    Em01Sub790 unk790;

    inline int isFlagBC() { return (unkBC & 1) != 0; }
};

extern "C" void func_em01_09D18578(Em01 *this_) {
    Em01Sub790 *sub = &this_->unk790;
    bool ok = false;

    switch (this_->unk1D5) {
    case 0:
        this_->unk1D5++;
        this_->unk280 = 0;
        this_->unk410 &= ~2;
        func_game_task_09AC04F8(this_, 0x12, 0, 0);
        func_em01_09D29C08(this_, 3);
        this_->unk248 = 20.0f;
        sub->unk18 = 0;
        break;
    case 1:
        if (func_eboot_08864400(this_->unk80, 0, 30.0f)) {
            this_->unk280 = 2;
            ok = func_em01_09D29F48(this_);
        }
        if (ok && this_->unk204 <= this_->unk2CC) {
            func_game_task_09AC04F8(this_, 0x13, 0, 0);
            this_->unk1D5++;
            this_->unk280 = 0;
            this_->unk204 = this_->unk2CC;
            func_eboot_08813D50(Camera::objectPtr, 2, this_);
        }
        break;
    case 2:
        if (!this_->isFlagBC()) {
            sub->unk6 = 0x96;
            this_->unk1D5++;
            func_em01_09D152D0(this_, 0, 0);
        }
        break;
    }
    if (this_->unk204 < this_->unk2CC) {
        this_->unk204 = this_->unk2CC;
    }
}
