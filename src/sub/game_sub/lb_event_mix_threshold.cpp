#include "common.h"
#include "item_manager.hpp"
#include "lb_event.hpp"

extern "C" int func_game_sub_09C15E60(LbEvent *) {
    return ItemManager::objectPtr->knownMixCount(3) == 18;
}
