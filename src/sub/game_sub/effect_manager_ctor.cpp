extern "C" {
void *func_game_sub_09C26DB8(void *);
void *func_game_sub_09C26D70(void *, int);
void *func_game_sub_09C26D28(void *);
void *func_game_sub_09C26CF8(void *, int);
void __construct_array(void *, void *(*)(void *), void *(*)(void *, int), int, int);
void reset__5cacheFPvUi(void *, void *, unsigned int);
}
#include "singleton.hpp"
struct EffectManager;
extern "C" void *func_game_sub_09C26BC8(unsigned char *self)
{
    Singleton<EffectManager>::objectPtr = (EffectManager *)self;
    __construct_array(self + 0x190a0, func_game_sub_09C26DB8, func_game_sub_09C26D70, 0x140, 3);
    __construct_array(self + 0x19520, func_game_sub_09C26DB8, func_game_sub_09C26D70, 0x140, 4);
    __construct_array(self + 0x19bc0, func_game_sub_09C26D28, func_game_sub_09C26CF8, 0x30, 82);
    __construct_array(self + 0x1ab20, func_game_sub_09C26D28, func_game_sub_09C26CF8, 0x30, 256);
    reset__5cacheFPvUi(self + 0x19004, self + 4, 0x19000);
    *(void **)(self + 0x19030) = 0;
    *(int *)(self + 0x19034) = 0;
    self[0x19490] = 0;
    self[0x19491] = 0;
    self[0x19492] = 0;
    self[0x19493] = 0;
    return self;
}
