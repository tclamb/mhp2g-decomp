#include "common.h"
#include "game_sys.hpp"
#include "lb_event.hpp"

extern "C" u8 func_game_sub_09C19458(LbEvent *, int);

extern "C" void func_game_sub_09C17458(LbEvent *event) {
    int index = 0;
    u8 *record = (u8 *)GameSys::objectPtr + 0x696F4;
    do {
        if (func_game_sub_09C19458(event, index) == 1) {
            if (*(s8 *)(record + 8) == 2) {
                *(s8 *)(record + 0x15) = 1;
            } else {
                *(s8 *)(record + 0x15) = 0;
            }
        }
        ++index;
        record += 0x70;
    } while (index < 5);
}
