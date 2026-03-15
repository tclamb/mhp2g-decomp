#pragma once

#include "ge_manager.hpp"

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
#define GE_CMD_FOGENABLE        0x1F

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

#define GE_CMD_WORLDMATRIXNUMBER 0x3A
#define GE_CMD_WORLDMATRIXDATA   0x3B
#define GE_CMD_VIEWMATRIXNUMBER  0x3C
#define GE_CMD_VIEWMATRIXDATA    0x3D
#define GE_CMD_PROJMATRIXNUMBER  0x3E
#define GE_CMD_PROJMATRIXDATA    0x3F

#define GE_CMD_TGENMATRIXDATA       0x41
#define GE_CMD_VIEWPORTXSCALE       0x42
#define GE_CMD_VIEWPORTYSCALE       0x43
#define GE_CMD_VIEWPORTZSCALE       0x44
#define GE_CMD_VIEWPORTXCENTER      0x45
#define GE_CMD_VIEWPORTYCENTER      0x46
#define GE_CMD_VIEWPORTZCENTER      0x47
#define GE_CMD_TEXSCALEU            0x48
#define GE_CMD_TEXSCALEV            0x49
#define GE_CMD_TEXOFFSETU           0x4A
#define GE_CMD_TEXOFFSETV           0x4B
#define GE_CMD_OFFSETX              0x4C
#define GE_CMD_OFFSETY              0x4D

#define GE_CMD_SHADEMODE            0x50
#define GE_CMD_MATERIALUPDATE       0x53
#define GE_CMD_MATERIALEMISSIVE     0x54
#define GE_CMD_MATERIALAMBIENT      0x55
#define GE_CMD_MATERIALDIFFUSE      0x56
#define GE_CMD_MATERIALSPECULAR     0x57
#define GE_CMD_MATERIALALPHA        0x58
#define GE_CMD_MATERIALSPECULARCOEF 0x5B
#define GE_CMD_AMBIENTCOLOR         0x5C
#define GE_CMD_AMBIENTALPHA         0x5D
#define GE_CMD_LIGHTMODE            0x5E

#define GE_CMD_LAC0 0x8F
#define GE_CMD_LDC0 0x90
#define GE_CMD_LSC0 0x91
#define GE_CMD_LAC1 0x92
#define GE_CMD_LDC1 0x93
#define GE_CMD_LSC1 0x94
#define GE_CMD_LAC2 0x95
#define GE_CMD_LDC2 0x96
#define GE_CMD_LSC2 0x97
#define GE_CMD_LAC3 0x98
#define GE_CMD_LDC3 0x99
#define GE_CMD_LSC3 0x9A

#define GE_CMD_TEXADDR0       0xA0
#define GE_CMD_TEXBUFWIDTH0   0xA8

#define GE_CMD_CLUTADDR       0xB0
#define GE_CMD_CLUTADDRUPPER  0xB1
#define GE_CMD_TEXSIZE        0xB8
#define GE_CMD_TRANSFERSRC    0xB2
#define GE_CMD_TRANSFERSRCW   0xB3
#define GE_CMD_TRANSFERDST    0xB4
#define GE_CMD_TRANSFERDSTW   0xB5

#define GE_CMD_TEXMAPMODE   0xC0
#define GE_CMD_TEXSHADELS   0xC1
#define GE_CMD_TEXMODE      0xC2
#define GE_CMD_TEXFORMAT    0xC3
#define GE_CMD_LOADCLUT     0xC4
#define GE_CMD_CLUTFORMAT   0xC5
#define GE_CMD_TEXFILTER    0xC6
#define GE_CMD_TEXWRAP      0xC7
#define GE_CMD_TEXLEVEL     0xC8
#define GE_CMD_TEXFUNC      0xC9
#define GE_CMD_TEXENVCOLOR  0xCA
#define GE_CMD_TEXFLUSH     0xCB
#define GE_CMD_TEXSYNC      0xCC

#define GE_CMD_FOG1     0xCD
#define GE_CMD_FOG2     0xCE
#define GE_CMD_FOGCOLOR 0xCF

#define GE_CMD_SCISSOR1 0xD4
#define GE_CMD_SCISSOR2 0xD5
#define GE_CMD_MINZ     0xD6
#define GE_CMD_MAXZ     0xD7

#define GE_CMD_COLORTEST        0xD8
#define GE_CMD_COLORREF         0xD9
#define GE_CMD_COLORTESTMASK    0xDA
#define GE_CMD_ATEST            0xDB
#define GE_CMD_STENCILTEST      0xDC
#define GE_CMD_ZTEST            0xDE
#define GE_CMD_BLENDMODE        0xDF

#define GE_CMD_BLENDFIXEDA      0xE0
#define GE_CMD_BLENDFIXEDB      0xE1
#define GE_CMD_DITH0            0xE2
#define GE_CMD_DITH1            0xE3
#define GE_CMD_DITH2            0xE4
#define GE_CMD_DITH3            0xE5
#define GE_CMD_ZWRITEDISABLE    0xE7

#define GE_CMD_TRANSFERSTART  0xEA
#define GE_CMD_TRANSFERSRCPOS 0xEB
#define GE_CMD_TRANSFERDSTPOS 0xEC
#define GE_CMD_TRANSFERSIZE   0xEE

#define GE_OP_NEVER 0
#define GE_OP_ALWAYS 1
#define GE_OP_NOT_EQUALS 3
#define GE_OP_AT_MOST 5
#define GE_OP_GREATER_THAN 6
#define GE_OP_AT_LEAST 7

#define GE_MATERIALCOLOR_EMISSIVE 0
#define GE_MATERIALCOLOR_AMBIENT  1
#define GE_MATERIALCOLOR_DIFFUSE  2
#define GE_MATERIALCOLOR_SPECULAR 3

#define GE_TEXMAP_TEXTURE_COORDS 0
#define GE_PROJMAP_POSITION 0

#define GE_TEXMODE_SWIZZLE 1

#define GE_TEXFUNC_MODULATE 0

#define GE_TEXFUNC_COMPONENTS_RGBA 1

#define GE_TFILT_LINEAR 1
#define GE_TFILT_LINEAR_MIPMAP_LINEAR 7

#define GE_SHADE_GOURAUD 1

#define GE_LIGHTMODE_SINGLECOLOR 0

#define GE_BLENDMODE_MUL_AND_ADD 0
#define GE_BLENDMODE_MUL_AND_SUBTRACT 1

#define GE_SRCBLEND_SRCALPHA 2
#define GE_SRCBLEND_FIXA 10

#define GE_DSTBLEND_INVSRCALPHA 3
#define GE_DSTBLEND_FIXB 10

#define GE_TRANSFER_BPP_4 1

#define GE_PRIM_RECTANGLES 6

#define GE_VTYPE_TC_NONE 0

#define GE_VTYPE_COL_8888 7

#define GE_VTYPE_NRM_NONE 0

#define GE_VTYPE_POS_S16 2

#define GE_VTYPE_WEIGHT_NONE 0

#define GE_VTYPE_IDX_NONE 0

#define GE_VTYPE_THROUGH 1

extern u32 *DRAWABLE_WRITE_HEAD;

namespace immediate_ge {
    namespace ge {
        namespace impl {

            inline void emit(u32 cmd) {
                *DRAWABLE_WRITE_HEAD++ = cmd;
            }

        }

        // 0x0X

        inline void vaddr(void *address) {
            impl::emit((GE_CMD_BASE << 24) | (((u32)address & 0xFF000000) >> 8));
            impl::emit((GE_CMD_VADDR << 24) | (((u32)address << 8) >> 8));
        }

        inline void prim(int type, u16 count) {
            impl::emit(
                (GE_CMD_PRIM << 24) |
                ((type & 0x7) << 16) |
                count
            );
        }

        inline void call(void *address, u32 offset) {
            impl::emit((GE_CMD_BASE << 24) | (((u32)address & 0xFF000000) >> 8));
            impl::emit((GE_CMD_OFFSETADDR << 24) | (offset & 0x00FFFFFF));
            impl::emit((GE_CMD_CALL << 24) | ((u32)address & 0x00FFFFFF));
        }

        // 0x1X

        inline void vertextype(int uv_type, int color_type, int norm_type, int position_type, int weight_type, int index_type, int weight_count, int morph_count, bool through) {
            impl::emit(
                (GE_CMD_VERTEXTYPE << 24) |
                (through << 23) |
                ((morph_count & 0x7) << 18) |
                ((weight_count & 0x7) << 14) |
                ((index_type & 0x3) << 11) |
                ((weight_type & 0x3) << 9) |
                ((position_type & 0x3) << 7) |
                ((norm_type & 0x3) << 5) |
                ((color_type & 0x7) << 2) |
                (uv_type & 0x3)
            );
        }

        inline void lightingenable(bool enable) {
            impl::emit((GE_CMD_LIGHTINGENABLE << 24) | enable);
        }

        inline void lightenable(int index, bool enable) {
            impl::emit(((GE_CMD_LIGHTENABLE0 + index) << 24) | enable);
        }

        inline void depthclampenable(bool enable) {
            impl::emit((GE_CMD_DEPTHCLAMPENABLE << 24) | enable);
        }
        inline void cullfaceenable(bool enable) {
            impl::emit((GE_CMD_CULLFACEENABLE << 24) | enable);
        }

        inline void texturemapenable(bool enable) {
            impl::emit((GE_CMD_TEXTUREMAPENABLE << 24) | enable);
        }

        inline void fogenable(bool enable) {
            impl::emit((GE_CMD_FOGENABLE << 24) | enable);
        }

        // 0x2X

        inline void ditherenable(bool enable) {
            impl::emit((GE_CMD_DITHERENABLE << 24) | enable);
        }

        inline void alphablendenable(bool enable) {
            impl::emit((GE_CMD_ALPHABLENDENABLE << 24) | enable);
        }

        inline void alphatestenable(bool enable) {
            impl::emit((GE_CMD_ALPHATESTENABLE << 24) | enable);
        }

        inline void ztestenable(bool enable) {
            impl::emit((GE_CMD_ZTESTENABLE << 24) | enable);
        }

        inline void stenciltestenable(bool enable) {
            impl::emit((GE_CMD_STENCILTESTENABLE << 24) | enable);
        }

        inline void antialiasenable(bool enable) {
            impl::emit((GE_CMD_ANTIALIASENABLE << 24) | enable);
        }

        inline void colortestenable(bool enable) {
            impl::emit((GE_CMD_COLORTESTENABLE << 24) | enable);
        }

        inline void logicopenable(bool enable) {
            impl::emit((GE_CMD_LOGICOPENABLE << 24) | enable);
        }

        // 0x3X

        inline void world(ScePspFMatrix4 *m) {
            ScePspMatrix4 *mm = reinterpret_cast<ScePspMatrix4*>(m);

            impl::emit(GE_CMD_WORLDMATRIXNUMBER << 24);

            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.x.x >> 8));
            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.x.y >> 8));
            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.x.z >> 8));

            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.y.x >> 8));
            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.y.y >> 8));
            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.y.z >> 8));

            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.z.x >> 8));
            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.z.y >> 8));
            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.z.z >> 8));

            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.w.x >> 8));
            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.w.y >> 8));
            impl::emit((GE_CMD_WORLDMATRIXDATA << 24) | ((u32)mm->im.w.z >> 8));
        }

        inline void view(ScePspFMatrix4 *m) {
            ScePspMatrix4 *mm = reinterpret_cast<ScePspMatrix4*>(m);

            impl::emit(GE_CMD_VIEWMATRIXNUMBER << 24);

            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.x.x >> 8));
            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.x.y >> 8));
            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.x.z >> 8));

            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.y.x >> 8));
            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.y.y >> 8));
            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.y.z >> 8));

            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.z.x >> 8));
            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.z.y >> 8));
            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.z.z >> 8));

            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.w.x >> 8));
            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.w.y >> 8));
            impl::emit((GE_CMD_VIEWMATRIXDATA << 24) | ((u32)mm->im.w.z >> 8));
        }

        inline void projection(ScePspFMatrix4 *m) {
            ScePspMatrix4 *mm = reinterpret_cast<ScePspMatrix4*>(m);

            impl::emit(GE_CMD_PROJMATRIXNUMBER << 24);

            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.x.x >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.x.y >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.x.z >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.x.w >> 8));

            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.y.x >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.y.y >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.y.z >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.y.w >> 8));

            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.z.x >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.z.y >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.z.z >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.z.w >> 8));

            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.w.x >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.w.y >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.w.z >> 8));
            impl::emit((GE_CMD_PROJMATRIXDATA << 24) | ((u32)mm->im.w.w >> 8));
        }

        // 0x4X

        inline void viewportscale(ScePspVector3 &scale) {
            impl::emit((GE_CMD_VIEWPORTXSCALE << 24) | ((u32)scale.iv.x >> 8));
            impl::emit((GE_CMD_VIEWPORTYSCALE << 24) | ((u32)scale.iv.y >> 8));
            impl::emit((GE_CMD_VIEWPORTZSCALE << 24) | ((u32)scale.iv.z >> 8));
        }

        inline void viewportcenter(ScePspVector3 &center) {
            impl::emit((GE_CMD_VIEWPORTXCENTER << 24) | ((u32)center.iv.x >> 8));
            impl::emit((GE_CMD_VIEWPORTYCENTER << 24) | ((u32)center.iv.y >> 8));
            impl::emit((GE_CMD_VIEWPORTZCENTER << 24) | ((u32)center.iv.z >> 8));
        }

        inline void texscale(ScePspIVector2 &uv) {
            impl::emit((GE_CMD_TEXSCALEU << 24) | ((u32)uv.x >> 8));
            impl::emit((GE_CMD_TEXSCALEV << 24) | ((u32)uv.y >> 8));
        }

        inline void texscale(ScePspUnion32 &a) {
            impl::emit((GE_CMD_TEXSCALEU << 24) | (a.ui >> 8));
            impl::emit((GE_CMD_TEXSCALEV << 24) | (a.ui >> 8));
        }

        inline void texoffset() {
            impl::emit((GE_CMD_TEXOFFSETU << 24));
            impl::emit((GE_CMD_TEXOFFSETV << 24));
        }

        inline void texoffsetu() {
            impl::emit((GE_CMD_TEXOFFSETU << 24));
        }

        inline void texoffsetv() {
            impl::emit((GE_CMD_TEXOFFSETV << 24));
        }

        inline void offset(u16 x, u16 y) {
            impl::emit((GE_CMD_OFFSETX << 24) | x);
            impl::emit((GE_CMD_OFFSETY << 24) | y);
        }

        // 0x5X

        inline void shademode(u32 mode) {
            impl::emit((GE_CMD_SHADEMODE << 24) | mode);
        }

        inline void materialupdate(u8 flags) {
            impl::emit((GE_CMD_MATERIALUPDATE << 24) | flags);
        }

        inline void materialemissive(u8 r, u8 g, u8 b) {
            impl::emit((GE_CMD_MATERIALEMISSIVE << 24) | (b << 16) | (g << 8) | r);
        }

        inline void materialambient(u8 r, u8 g, u8 b) {
            impl::emit((GE_CMD_MATERIALAMBIENT << 24) | (b << 16) | (g << 8) | r);
        }

        inline void materialambient(ScePspUnion32 &color) {
            impl::emit((GE_CMD_MATERIALAMBIENT << 24) | (color.uc[2] << 16) | (color.uc[1] << 8) | color.uc[0]);
        }

        inline void materialdiffuse(u8 r, u8 g, u8 b) {
            impl::emit((GE_CMD_MATERIALDIFFUSE << 24) | (b << 16) | (g << 8) | r);
        }

        inline void materialdiffuse(ScePspUnion32 &color) {
            impl::emit((GE_CMD_MATERIALDIFFUSE << 24) | (color.uc[2] << 16) | (color.uc[1] << 8) | color.uc[0]);
        }

        inline void materialspecular(u8 r, u8 g, u8 b) {
            impl::emit((GE_CMD_MATERIALSPECULAR << 24) | (b << 16) | (g << 8) | r);
        }

        inline void materialalpha(u8 a) {
            impl::emit((GE_CMD_MATERIALALPHA << 24) | a);
        }

        inline void materialspecularcoef() {
            impl::emit(GE_CMD_MATERIALSPECULARCOEF << 24);
        }

        inline void ambientcolor(u8 r, u8 g, u8 b) {
            impl::emit((GE_CMD_AMBIENTCOLOR << 24) | (b << 16) | (g << 8) | r);
        }

        inline void ambientalpha(u8 a) {
            impl::emit((GE_CMD_AMBIENTALPHA << 24) | a);
        }

        inline void lightmode(u32 mode) {
            impl::emit((GE_CMD_LIGHTMODE << 24) | mode);
        }

        // 0x9X (+ 0x8F)

        inline void lightambientcolor(int index, u8 r, u8 g, u8 b) {
            impl::emit(((GE_CMD_LAC0 + 3 * index) << 24) | (b << 16) | (g << 8) | r);
        }

        inline void lightdiffusecolor(int index, u8 r, u8 g, u8 b) {
            impl::emit(((GE_CMD_LDC0 + 3 * index) << 24) | (b << 16) | (g << 8) | r);
        }

        inline void lightspecularcolor(int index, u8 r, u8 g, u8 b) {
            impl::emit(((GE_CMD_LSC0 + 3 * index) << 24) | (b << 16) | (g << 8) | r);
        }

        // 0xCX

        inline void texmapmode(u32 mode, u32 projmode) {
            impl::emit((GE_CMD_TEXMAPMODE << 24) | (projmode << 8) | mode);
        }

        inline void texmode(u32 flags) {
            impl::emit((GE_CMD_TEXMODE << 24) | flags);
        }

        inline void texfilter(u8 min, u8 mag) {
            impl::emit((GE_CMD_TEXFILTER << 24) | (mag << 8) | min);
        }

        inline void texwrap() {
            impl::emit(GE_CMD_TEXWRAP << 24);
        }

        inline void texfunc(u32 func, u32 components) {
            impl::emit((GE_CMD_TEXFUNC << 24) | (components << 8) | func);
        }

        inline void texflush() {
            impl::emit(GE_CMD_TEXFLUSH << 24);
        }

        inline void fog(float color, float begin, float end) {
            ScePspUnion32 c; c.f = color;
            ScePspUnion32 fog1; fog1.f = end;
            float slope = end - begin;
            slope = 1.0f / (slope ? slope : 1.0f);
            impl::emit((GE_CMD_FOGCOLOR << 24) | (c.uc[2] << 16) | (c.uc[1] << 8) | (c.uc[0]));
            impl::emit((GE_CMD_FOG1 << 24) | (fog1.ui >> 8));
            ScePspUnion32 fog2; fog2.f = slope;
            impl::emit((GE_CMD_FOG2 << 24) | (fog2.ui >> 8));
        }

        // 0xDX

        inline void scissor(u16 x1, u16 y1, u16 x2, u16 y2) {
            impl::emit((GE_CMD_SCISSOR1 << 24) | ((y1 & 0x3FF) << 10) | (x1 & 0x3FF));
            impl::emit((GE_CMD_SCISSOR2 << 24) | ((y2 & 0x3FF) << 10) | (x2 & 0x3FF));
        }

        inline void minz(u16 z) {
            impl::emit((GE_CMD_MINZ << 24) | z);
        }

        inline void maxz(u16 z) {
            impl::emit((GE_CMD_MAXZ << 24) | z);
        }

        inline void colortest(u32 op) {
            impl::emit((GE_CMD_COLORTEST << 24) | op);
        }

        inline void colorref(u8 r, u8 g, u8 b) {
            impl::emit((GE_CMD_COLORREF << 24) | (b << 16) | (g << 8) | r);
        }

        inline void colortestmask(u8 rmask, u8 gmask, u8 bmask) {
            impl::emit((GE_CMD_COLORTESTMASK << 24) | (bmask << 16) | (gmask << 8) | rmask);
        }

        inline void atest(u8 mask, u8 threshold, u8 op) {
            impl::emit(((GE_CMD_ATEST << 24) | (mask << 16) | op) | (threshold << 8));
        }

        inline void ztest(u8 op) {
            impl::emit((GE_CMD_ZTEST << 24) | op);
        }

        inline void blendmode(u8 func, u8 srcfactor, u8 dstfactor) {
            impl::emit((GE_CMD_BLENDMODE << 24) | (func << 8) | (dstfactor << 4) | srcfactor);
        }

        // 0xEX

        inline void blendfixeda(u8 rmask, u8 gmask, u8 bmask) {
            impl::emit((GE_CMD_BLENDFIXEDA << 24) | (bmask << 16) | (gmask << 8) | rmask);
        }

        inline void blendfixedb(u8 rmask, u8 gmask, u8 bmask) {
            impl::emit((GE_CMD_BLENDFIXEDB << 24) | (bmask << 16) | (gmask << 8) | rmask);
        }

        inline void zwritedisable(bool value) {
            impl::emit((GE_CMD_ZWRITEDISABLE << 24) |  value);
        }

    }
}
