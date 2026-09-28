typedef unsigned int u32;
struct Actor { u32 *vtable; };
extern "C" u32 D_eboot_089BA2C8[];
extern "C" void func_game_sub_09C51E10(Actor *, int);
extern "C" void func_game_sub_09C51EC0(Actor *);
extern "C" Actor *func_em75_09D167E0(Actor *actor, int flag) {
    if (actor != 0) {
        actor->vtable = D_eboot_089BA2C8;
        func_game_sub_09C51E10(actor, 0);
        if ((short)flag > 0) {
            func_game_sub_09C51EC0(actor);
        }
    }
    return actor;
}
