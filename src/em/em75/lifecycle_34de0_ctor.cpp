typedef unsigned int u32;
struct Actor { u32 *vtable; };
extern "C" u32 D_eboot_089BA54C[];
extern "C" void func_game_task_09B62320(Actor *);
extern "C" Actor *func_em75_09D49EE0(Actor *actor) {
    func_game_task_09B62320(actor);
    actor->vtable = D_eboot_089BA54C;
    return actor;
}
