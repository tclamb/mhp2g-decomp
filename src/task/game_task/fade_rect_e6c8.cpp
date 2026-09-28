#include "common.h"
#include "ge.hpp"
#include "immediate_ge.hpp"
#include "ge_packet.hpp"
struct FadeTaskView {
    u8 reserved[0x68];
    s32 fade_alpha_68;
    u8 fade_flag_6C;
};

extern "C" void func_game_task_09A5E6C8(FadeTaskView *task) {
    if (task->fade_flag_6C == 1) {
        task->fade_alpha_68 += 0x20;
        if (task->fade_alpha_68 >= 0xFF) {
            task->fade_alpha_68 = 0xFF;
        }
        u32 *head = Ge::objectPtr->write_head();
        head[0] = 0x23000000;
        head[1] = 0xDE000000;
        head[2] = 0x21000001;
        head[3] = 0xDF000032;
        head[4] = 0x22000000;
        head[5] = 0xDBFF0003;
        head[6] = 0x10000000;
        head[7] = 0x08000000;
        Ge::objectPtr->method_088595E8(head, 8, 9);
        Ge::objectPtr->set_write_head(head + 8);
        GePacket<6, 8> d;
        d.vertexWords = 6;
        d.commandWords = 8;
        d.commands[0] = 0x1E000000;
        d.commands[1] = 0x50000001;
        d.commands[2] = 0x1280011C;
        d.commands[5] = 0x04060002;
        d.commands[6] = 0x10000000;
        d.commands[7] = 0x08000000;
        d.vertices[0] = task->fade_alpha_68 << 24;
        d.vertices[1] = 0;
        d.vertices[2] = 0;
        d.vertices[3] = task->fade_alpha_68 << 24;
        d.vertices[4] = 0x011001E0;
        d.vertices[5] = 0;
        geDrawPacket(Ge::objectPtr, &d, 9);
    } else {
        task->fade_alpha_68 = 0;
    }
}
