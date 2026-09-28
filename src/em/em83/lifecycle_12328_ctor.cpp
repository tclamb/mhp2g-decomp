typedef unsigned int u32;
struct Actor { u32 *vtable; };
extern "C" u32 D_eboot_089BA6AC[];
extern "C" void func_game_sub_09C51DF0(Actor *);
extern "C" Actor *func_em83_09D27428(Actor *actor) {
    func_game_sub_09C51DF0(actor);
    actor->vtable = D_eboot_089BA6AC;
    return actor;
}
