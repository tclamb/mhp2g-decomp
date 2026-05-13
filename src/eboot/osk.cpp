#include "osk.hpp"
#include "psputility.h"
#include "psputility_osk.h"
#include "system.hpp"
#include "fade_task.hpp"

template<> Osk *Singleton<Osk>::objectPtr;

void Osk::initialize() {
    visible = false;
    initializeLanguage();
}

struct SystemLanguageId {
    enum {
        JAPANESE,
        ENGLISH,
        FRENCH,
        SPANISH,
        GERMAN,
        ITALIAN,
        DUTCH,
        PORTUGUESE,
        RUSSIAN,
        KOREAN,
        CHINESE_TRADITIONAL,
        CHINESE_SIMPLIFIED,
    };
private:
    SystemLanguageId();
};

struct InputLanguageId {
    enum {
        DEFAULT,
        JAPANESE,
        ENGLISH,
        FRENCH,
        SPANISH,
        GERMAN,
        ITALIAN,
        DUTCH,
        PORTUGUESE,
        RUSSIAN,
    };
private:
    InputLanguageId();
};

void Osk::initializeLanguage() {
    switch (System::objectPtr->languageId) {
    default:
    case LanguageId::JAPANESE:
        systemLanguageId = SystemLanguageId::JAPANESE;
        buttonSwap = false;
        break;
    case LanguageId::ENGLISH:
        systemLanguageId = SystemLanguageId::ENGLISH;
        buttonSwap = true;
        break;
    case LanguageId::GERMAN:
        systemLanguageId = SystemLanguageId::GERMAN;
        buttonSwap = true;
        break;
    case LanguageId::FRENCH:
        systemLanguageId = SystemLanguageId::FRENCH;
        buttonSwap = true;
        break;
    case LanguageId::SPANISH:
        systemLanguageId = SystemLanguageId::SPANISH;
        buttonSwap = true;
        break;
    case LanguageId::ITALIAN:
        systemLanguageId = SystemLanguageId::ITALIAN;
        buttonSwap = true;
        break;
    case LanguageId::KOREAN:
        systemLanguageId = SystemLanguageId::KOREAN;
        buttonSwap = false;
        break;
    }

    switch (systemLanguageId) {
        case SystemLanguageId::JAPANESE:
            inputLanguageId = InputLanguageId::JAPANESE;
            break;
        case SystemLanguageId::ENGLISH:
            inputLanguageId = InputLanguageId::ENGLISH;
            break;
        case SystemLanguageId::FRENCH:
            inputLanguageId = InputLanguageId::FRENCH;
            break;
        case SystemLanguageId::SPANISH:
            inputLanguageId = InputLanguageId::SPANISH;
            break;
        case SystemLanguageId::GERMAN:
            inputLanguageId = InputLanguageId::GERMAN;
            break;
        case SystemLanguageId::ITALIAN:
            inputLanguageId = InputLanguageId::ITALIAN;
            break;
        case SystemLanguageId::DUTCH:
            inputLanguageId = InputLanguageId::DUTCH;
            break;
        case SystemLanguageId::PORTUGUESE:
            inputLanguageId = InputLanguageId::PORTUGUESE;
            break;
        case SystemLanguageId::RUSSIAN:
            inputLanguageId = InputLanguageId::RUSSIAN;
            break;
        case SystemLanguageId::KOREAN:
            inputLanguageId = InputLanguageId::DEFAULT;
            break;
        case SystemLanguageId::CHINESE_TRADITIONAL:
            inputLanguageId = InputLanguageId::DEFAULT;
            break;
        case SystemLanguageId::CHINESE_SIMPLIFIED:
            inputLanguageId = InputLanguageId::DEFAULT;
            break;
        default:
            inputLanguageId = InputLanguageId::DEFAULT;
            break;
    }
}

#define PSP_UTILITY_ANIMSPEED_30FPS 2

void Osk::update() {
    if (visible) {
        switch (sceUtilityOskGetStatus()) {
        case PSP_UTILITY_COMMON_STATUS_INIT:
            break;
        case PSP_UTILITY_COMMON_STATUS_RUNNING:
            sceUtilityOskUpdate(PSP_UTILITY_ANIMSPEED_30FPS);
            break;
        case PSP_UTILITY_COMMON_STATUS_FINISHED:
            sceUtilityOskShutdownStart();
            break;
        case PSP_UTILITY_COMMON_STATUS_SHUTDOWN:
            break;
        case PSP_UTILITY_COMMON_STATUS_NONE:
            finish();
            break;
        }
    }
}

void Osk::input(u8 inputTypeId, u16 *description, u16 *in, u16 *out, u32 outSize, s32 lineCount) {
    Pad::objectPtr->inputDisabled = true;
    visible = true;

    initializeLanguage();
    initializeParams(inputTypeId, description, in, out, outSize, lineCount);

    if (sceUtilityOskInitStart(&params) != 0) {
        Pad::objectPtr->inputDisabled = false;
        visible = false;
    } else {
        FadeTask::objectPtr->fadeBlack(4, true);
        FadeTask::objectPtr->alphaEnd = 192;
    }
}

#define PSP_UTILITY_OSK_IME_ENABLED 1 // 0 disables character conversion

void Osk::initializeParams(u8 inputTypeId, u16 *description, u16 *in, u16 *out, u32 outSize, s32 lineCount) {
    memset(&params, 0, sizeof(params));

    params.base.size = sizeof(params);
    params.base.language = systemLanguageId;
    params.base.buttonSwap = buttonSwap;
    params.base.graphicsThread = 42;
    params.base.accessThread = 44;
    params.base.fontThread = 43;
    params.base.soundThread = 41;

    params.datacount = 1;
    params.data = &data;

    SceUtilityOskData &d = data;
    d.unk_00 = PSP_UTILITY_OSK_IME_ENABLED;
    d.unk_04 = 0;
    d.language = inputLanguageId;
    d.unk_12 = 0;
    d.lines = lineCount;
    d.unk_24 = 1; // doesn't seem to do anything

    static const u16 EMPTY_UNICODE_STRING[1] = {};
    if (description == NULL) {
        d.desc = const_cast<u16 *>(EMPTY_UNICODE_STRING);
    } else {
        d.desc = description;
    }

    if (in == NULL) {
        d.intext = const_cast<u16 *>(EMPTY_UNICODE_STRING);
    } else {
        d.intext = in;
    }

    d.outtextlength = outSize;
    d.outtext = out;
    d.result = PSP_UTILITY_OSK_RESULT_UNCHANGED;
    d.outtextlimit = 0;

    switch (inputTypeId) {
    default:
        d.inputtype = PSP_UTILITY_OSK_INPUTTYPE_ALL;
        break;
    case InputTypeId::JAPANESE:
        d.inputtype = PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_KANJI
                    | PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_KATAKANA
                    | PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_HIRAGANA
                    | PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_UPPERCASE
                    | PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_LOWERCASE
                    | PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_SYMBOL
                    | PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_DIGIT;
        break;
    case InputTypeId::LATIN:
        d.inputtype = PSP_UTILITY_OSK_INPUTTYPE_LATIN_UPPERCASE
                    | PSP_UTILITY_OSK_INPUTTYPE_LATIN_LOWERCASE
                    | PSP_UTILITY_OSK_INPUTTYPE_LATIN_SYMBOL
                    | PSP_UTILITY_OSK_INPUTTYPE_LATIN_DIGIT;
        break;
    case InputTypeId::DIGIT:
        d.inputtype = PSP_UTILITY_OSK_INPUTTYPE_JAPANESE_DIGIT
                    | PSP_UTILITY_OSK_INPUTTYPE_LATIN_DIGIT;
        break;
    case InputTypeId::LATIN_DIGIT:
        d.inputtype = PSP_UTILITY_OSK_INPUTTYPE_LATIN_DIGIT;
        break;
    }
}

void Osk::finish() {
    SceUtilityOskData &d = data;
    FadeTask::objectPtr->fadeBlack(1, false);
    switch (d.result) {
    case PSP_UTILITY_OSK_RESULT_UNCHANGED:
        break;
    case PSP_UTILITY_OSK_RESULT_CANCELLED: {
        u16 *q = d.intext;
        u16 *p = d.outtext;
        u32 n = d.outtextlength;
        while (n != 0) {
            *p = *q;
            if (*q == 0) {
                break;
            }
            --n;
            ++p;
            ++q;
        }
        if (n == 0) {
            p[-1] = 0;
        }
        break;
    }
    case PSP_UTILITY_OSK_RESULT_CHANGED:
        break;
    }
    Pad::objectPtr->inputDisabled = false;
    visible = false;
}
