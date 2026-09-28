typedef unsigned int u32;
struct Actor { u32 *vtable; };
extern "C" u32 D_eboot_089B99FC[];
extern "C" void func_game_task_09B62320(Actor *);
extern "C" Actor *func_em14_09D24568(Actor *actor) {
    func_game_task_09B62320(actor);
    actor->vtable = D_eboot_089B99FC;
    return actor;
}
