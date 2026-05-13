#pragma once

#include "common.h"
#include "singleton.hpp"

#include <psputility.h>

struct InputTypeId {
    enum {
        JAPANESE = 1,
        LATIN,
        DIGIT,
        LATIN_DIGIT,
    };
private:
    InputTypeId();
};

struct Osk : Singleton<Osk> {
    SceUtilityOskParams params;
    SceUtilityOskData data;
    bool visible;
    u8 systemLanguageId;
    bool buttonSwap;
    u8 inputLanguageId;
    u32 padding;

    void initialize();
    void update();
    void input(u8 inputTypeId, u16 *description, u16 *in, u16 *out, u32 outSize, s32 lineCount);

    Osk();

private:
    void initializeLanguage();
    void initializeParams(u8 inputTypeId, u16 *description, u16 *in, u16 *out, u32 outSize, s32 lineCount);
    void finish();
};
