#pragma once

// Minimal declarations for this matched BgmServer helper.
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;
struct BgmServer { u8 reserved[0xC]; u16 cue_0C; u8 reserved_0E[2]; u32 cue_10; };

