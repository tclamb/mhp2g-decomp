#pragma once

// Stack-built immediate GE packet, copied into the Ge display-list slab.
//
// Used (inlined) by every Cockpit sprite/rect/line/quad helper, the Cockpit
// texture binds, FadeTask::draw and the game_task fade rectangle
// (func_game_task_09A5E6C8). A packet is built on the stack: `vertexWords`
// words of through-mode vertex data followed by `commandWords` GE commands
// whose last 5 words are BASE/VADDR (patched here to point at the copied
// vertices), PRIM and a BASE/JUMP pair that Ge::method_088595E8 overwrites to
// chain the fragment into render slot `slot`. The packet is copied to the Ge
// write head (vertices first), so the vertex data lives in the same slab.
//
// Matching quirks (each one maps to a specific asm pattern):
//  - `vertexWords` is read into a local before the `header` alias: MW forwards
//    the caller's store into it (blez on a register), while the size check
//    through the alias reloads both header words from the stack.
//  - the size check reads `ge->active_write_head` itself and `start` is read
//    again after it. With a variable slot MW CSEs both reads (start is the
//    size-check register); with a constant slot (the fade) it reloads start in
//    the blez delay slot. Reading `start` before the check only matches the
//    variable-slot case.
//  - `words = c ? &commandWords : &commandWords; src = words + 1` keeps the
//    unfolded `addiu v0, sp, X; addiu t3, v0, 4` pair.
//  - loads before the loop: vw, n = header[0] + header[1], cw = header[1].
#include "common.h"
#include "ge.hpp"
#include "immediate_ge.hpp"

template <int V, int C, int B = C - 5>
struct GePacket {
    enum { VADDR_INDEX = B };
    s32 vertexWords;
    s32 commandWords;
    u32 vertices[V];
    u32 commands[C];
};

// A packet with no vertex data (render-state changes only); the BASE/VADDR slot
// sits after the 12 counted commands and is never written (vertexWords == 0).
struct GeStatePacket {
    enum { VADDR_INDEX = 12 };
    s32 vertexWords;
    s32 commandWords;
    u32 commands[14];
};

template <class T>
static inline bool geDrawPacket(Ge *ge, T *packet, s32 slot) {
    if (slot < 0 || slot >= 20) {
        return false;
    }
    // read before the alias below: MW forwards the caller store into this
    // local, while the size check through the alias reloads both words
    s32 vertexWords = packet->vertexWords;
    s32 *header = (s32 *)packet;
    if ((u32)((ge->active_write_head - ge->slab[ge->active_buffer]) + (header[0] + header[1])) >= 0x10000) {
        return false;
    }
    u32 *start = ge->active_write_head;
    if (vertexWords > 0) {
        // BASE + VADDR -> the vertex copy at the write head
        packet->commands[T::VADDR_INDEX] = (GE_CMD_BASE << 24) | (((u32)ge->active_write_head & 0xFF000000) >> 8);
        packet->commands[T::VADDR_INDEX + 1] = (GE_CMD_VADDR << 24) | ((u32)ge->active_write_head & 0x00FFFFFF);
    }
    u32 *words = vertexWords > 0 ? (u32 *)&packet->commandWords : (u32 *)&packet->commandWords;
    s32 vw = packet->vertexWords;
    s32 n = header[0] + header[1];
    s32 cw = header[1];
    u32 *src = words + 1;
    for (s32 i = 0; i < n; i++) {
        *ge->active_write_head = src[i];
        ge->active_write_head++;
    }
    ge->method_088595E8(start + vw, cw, slot);
    return true;
}
