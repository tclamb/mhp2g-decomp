typedef unsigned char u8;

struct ObjBase {
    bool pl_action_ck(u8, u8);
};

extern "C" u8 func_game_task_09AD29D0(void *, int);

extern "C" int func_em59_09D18BD0(u8 *actor) {
    if ((actor[0x2B8] & 1) != 0) {
        return func_game_task_09AD29D0(actor, 0) != 1;
    }
    return 2;
}

extern "C" int func_em59_09D18C08(u8 *actor) {
    ObjBase *base = (ObjBase *)actor;
    if (actor[0x298] == 5 || base->pl_action_ck(6, 1) ||
        base->pl_action_ck(6, 2)) {
        return 1;
    }
    return 0;
}
