// Restores vtable, calls base destructor and conditionally frees actor.
typedef unsigned int u32;
struct Actor { u32 *vtable; };
extern "C" u32 D_eboot_089B9D6C[];
extern "C" void func_game_task_09ABF9D0(Actor *, int);
extern "C" void __dl__7ObjBaseFPv(Actor *);
extern "C" Actor *func_em54_09D29AB0(Actor *actor, int flag) {
    if (actor != 0) {
        actor->vtable = D_eboot_089B9D6C;
        func_game_task_09ABF9D0(actor, 0);
        if ((short)flag > 0) {
            __dl__7ObjBaseFPv(actor);
        }
    }
    return actor;
}
