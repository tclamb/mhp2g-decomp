#include "player.hpp"
#include "camera.hpp"
#include "draw_manager.hpp"
#include "light_manager.hpp"
#include "immediate_ge.hpp"
using namespace immediate_ge;

struct PlayerDraw : Player {
    virtual void vtable_0x40();
    virtual void vtable_0x44();
    virtual void vtable_0x48();
    virtual void vtable_0x4C();
    virtual void vtable_0x50();
};
extern "C" void func_eboot_08814ED4(Camera *, u8 *, ObjBase *);
extern "C" void func_game_task_09A70348(PlayerDraw *);
extern "C" s16 func_game_task_09A6E3E0(PlayerDraw *);

extern "C" void func_game_task_09A6E048(PlayerDraw *player) {
    if (*(u8 *)((u8 *)player + 0x13C9) == 0) return;
    if (player->stageId != (*(ObjBase **)((u8 *)player + 0x1214))->stageId) return;
    func_eboot_08814ED4(Camera::objectPtr, &player->alpha, player);
    if (player->alpha != 0xFF) {
        ge::alphablendenable(true);
        ge::blendmode(GE_BLENDMODE_MUL_AND_ADD, GE_SRCBLEND_SRCALPHA, GE_DSTBLEND_INVSRCALPHA);
        ge::atest(0xFF, 0, GE_OP_GREATER_THAN);
        ge::ambientalpha(player->alpha);
    } else {
        ge::alphablendenable(false);
        ge::atest(0xFF, 0xC0, GE_OP_AT_LEAST);
    }
    LightManager::objectPtr->method_08860A70(player);
    LightManager::objectPtr->method_08860EB8(player);
    DrawManager::objectPtr->world_model(&player->transform);
    func_eboot_088641B8(&player->hierarchy);
    func_game_task_09A70348(player);
    switch (*(s8 *)((u8 *)player + 0x11EE)) {
    case 0:
        player->model_pmo.drawWeightMesh(&player->hierarchy, &player->model_tmh, 1);
        player->model_pmo.drawWeightMesh(&player->hierarchy, &player->model_tmh, 2);
        break;
    case 1:
        player->model_pmo.drawWeightMesh(&player->hierarchy, &player->model_tmh, 3);
        break;
    case 2:
        player->model_pmo.drawWeightMesh(&player->hierarchy, &player->model_tmh, 4);
        break;
    }
    player->model_pmo.drawWeightMesh(&player->hierarchy, &player->model_tmh, 0);
    if ((bool)(player->lighting_flags & 0x400) == true) {
        s16 mesh = func_game_task_09A6E3E0(player);
        player->model_pmo.drawWeightMesh(&player->hierarchy, &player->model_tmh, mesh);
    }
    player->vtable_0x50();
    ge::ambientalpha(0xFF);
    ge::alphablendenable(false);
    ge::atest(0xFF, 0xC0, GE_OP_AT_LEAST);
    LightManager::objectPtr->method_08860C4C();
    LightManager::objectPtr->method_0886117C(1);
}
