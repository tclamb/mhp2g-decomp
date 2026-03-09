#pragma once

#define GE_CMD_NOP              0x00
#define GE_CMD_VADDR            0x01
#define GE_CMD_IADDR            0x02
#define GE_CMD_PRIM             0x04
#define GE_CMD_BEZIER           0x05
#define GE_CMD_SPLINE           0x06
#define GE_CMD_BOUNDINGBOX      0x07
#define GE_CMD_JUMP             0x08
#define GE_CMD_BJUMP            0x09
#define GE_CMD_CALL             0x0A
#define GE_CMD_RET              0x0B
#define GE_CMD_END              0x0C
#define GE_CMD_SIGNAL           0x0E
#define GE_CMD_FINISH           0x0F

#define GE_CMD_BASE             0x10
#define GE_CMD_VERTEXTYPE       0x12
#define GE_CMD_OFFSETADDR       0x13
#define GE_CMD_ORIGIN           0x14
#define GE_CMD_REGION1          0x15
#define GE_CMD_REGION2          0x16
#define GE_CMD_LIGHTINGENABLE   0x17
#define GE_CMD_LIGHTENABLE0     0x18
#define GE_CMD_LIGHTENABLE1     0x19
#define GE_CMD_LIGHTENABLE2     0x1A
#define GE_CMD_LIGHTENABLE3     0x1B
#define GE_CMD_DEPTHCLAMPENABLE 0x1C
#define GE_CMD_CULLFACEENABLE   0x1D
#define GE_CMD_TEXTUREMAPENABLE 0x1E

#define GE_CMD_DITHERENABLE         0x20
#define GE_CMD_ALPHABLENDENABLE     0x21
#define GE_CMD_ALPHATESTENABLE      0x22
#define GE_CMD_ZTESTENABLE          0x23
#define GE_CMD_STENCILTESTENABLE    0x24
#define GE_CMD_ANTIALIASENABLE      0x25
#define GE_CMD_PATCHCULLENABLE      0x26
#define GE_CMD_COLORTESTENABLE      0x27
#define GE_CMD_LOGICOPENABLE        0x28
#define GE_CMD_BONEMATRIXNUMBER     0x2A
#define GE_CMD_BONEMATRIXDATA       0x2B
#define GE_CMD_MORPHWEIGHT0         0x2C
#define GE_CMD_MORPHWEIGHT1         0x2D
#define GE_CMD_MORPHWEIGHT2         0x2E
#define GE_CMD_MORPHWEIGHT3         0x2F

#define GE_CMD_TEXSCALEU        0x48
#define GE_CMD_TEXSCALEV        0x49
#define GE_CMD_TEXOFFSETU       0x4A
#define GE_CMD_TEXOFFSETV       0x4B

#define GE_CMD_MATERIALUPDATE   0x53
#define GE_CMD_MATERIALAMBIENT  0x55
#define GE_CMD_MATERIALDIFFUSE  0x56
#define GE_CMD_MATERIALALPHA    0x58

#define GE_CMD_TEXMAPMODE       0xC0
#define GE_CMD_TEXSHADELS       0xC1
#define GE_CMD_TEXMODE          0xC2
#define GE_CMD_TEXFORMAT        0xC3
#define GE_CMD_LOADCLUT         0xC4
#define GE_CMD_CLUTFORMAT       0xC5
#define GE_CMD_TEXFILTER        0xC6
#define GE_CMD_TEXWRAP          0xC7
#define GE_CMD_TEXLEVEL         0xC8
#define GE_CMD_TEXFUNC          0xC9
#define GE_CMD_TEXENVCOLOR      0xCA
#define GE_CMD_TEXFLUSH         0xCB
#define GE_CMD_TEXSYNC          0xCC

#define GE_CMD_FOG1             0xCD
#define GE_CMD_FOG2             0xCE
#define GE_CMD_FOGCOLOR         0xCF

#define GE_CMD_ATEST            0xDB
#define GE_CMD_ZTEST            0xDE
#define GE_CMD_BLENDMODE        0xDF
#define GE_CMD_ZWRITEDISABLE    0xE7

#define GE_OP_ALWAYS 1
#define GE_OP_AT_MOST 5
#define GE_OP_GREATER_THAN 6
#define GE_OP_AT_LEAST 7

#define GE_MATERIALCOLOR_EMISSIVE 0
#define GE_MATERIALCOLOR_AMBIENT  1
#define GE_MATERIALCOLOR_DIFFUSE  2
#define GE_MATERIALCOLOR_SPECULAR 3

#define GE_TEXMODE_SWIZZLE 1

extern u32 *D_eboot_089C70D0;

namespace immediate_ge {
    namespace ge {
        namespace impl {
            union AsU32 {
                float input;
                u32 output;

                inline AsU32(float f) : input(f) {}

                operator u32() { return output; }
            };

            inline void emit(u32 cmd) {
                *D_eboot_089C70D0++ = cmd;
            }

        }

        // 0x1X

        inline void call(void *address, u32 offset) {
            impl::emit((GE_CMD_BASE << 24) | (((u32)address & 0xFF000000) >> 8));
            impl::emit((GE_CMD_OFFSETADDR << 24) | (offset & 0x00FFFFFF));
            impl::emit((GE_CMD_CALL << 24) | ((u32)address & 0x00FFFFFF));

        }

        inline void texturemapenable(bool enable) {
            impl::emit((GE_CMD_TEXTUREMAPENABLE << 24) | enable);
        }

        // 0x2X

        inline void colortestenable(bool enable) {
            impl::emit((GE_CMD_COLORTESTENABLE << 24) | enable);
        }

        // 0x4X

        inline void texscaleu(u32 u) {
            impl::emit((GE_CMD_TEXSCALEU << 24) | (u >> 8));
        }

        inline void texscalev(u32 v) {
            impl::emit((GE_CMD_TEXSCALEV << 24) | (v >> 8));
        }

        inline void texoffsetu() {
            impl::emit((GE_CMD_TEXOFFSETU << 24));
        }

        inline void texoffsetv() {
            impl::emit((GE_CMD_TEXOFFSETV << 24));
        }

        // 0x5X

        inline void materialupdate(u8 flags) {
            impl::emit((GE_CMD_MATERIALUPDATE << 24) | flags);
        }

        inline void materialdiffuse(u8 r, u8 g, u8 b) {
            impl::emit((GE_CMD_MATERIALDIFFUSE << 24) | (b << 16) | (g << 8) | r);
        }

        inline void materialalpha(u8 a) {
            impl::emit((GE_CMD_MATERIALALPHA << 24) | a);
        }

        inline void materialambient(u8 r, u8 g, u8 b) {
            impl::emit((GE_CMD_MATERIALAMBIENT << 24) | (b << 16) | (g << 8) | r);
        }

        // 0xCX

        inline void texmode(u32 flags) {
            impl::emit((GE_CMD_TEXMODE << 24) | flags);
        }

        inline void texflush() {
            impl::emit(GE_CMD_TEXFLUSH << 24);
        }

        // 0xDX

        inline void atest(u8 mask, u8 threshold, u8 op) {
            impl::emit(((GE_CMD_ATEST << 24) | (mask << 16) | op) | (threshold << 8));
        }

        inline void ztest(u8 op) {
            impl::emit((GE_CMD_ZTEST << 24) |  op);
        }

        // 0xEX

        inline void zwritedisable(bool value) {
            impl::emit((GE_CMD_ZWRITEDISABLE << 24) |  value);
        }

    }
}
