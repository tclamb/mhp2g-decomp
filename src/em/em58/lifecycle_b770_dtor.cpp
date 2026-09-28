typedef unsigned int u32;
struct Actor { u32 *vtable; };
extern "C" u32 D_eboot_089BA0CC[];
extern "C" void func_game_task_09B623B8(Actor *, int);
extern "C" void func_game_task_09B62408(Actor *);
extern "C" Actor *func_em58_09D208A8(Actor *actor, int flag) {
    if (actor != 0) {
        actor->vtable = D_eboot_089BA0CC;
        func_game_task_09B623B8(actor, 0);
        if ((short)flag > 0) {
            func_game_task_09B62408(actor);
        }
    }
    return actor;
}
