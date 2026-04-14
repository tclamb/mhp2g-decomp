#include "ge.hpp"
#include "singleton.hpp"

#include <pspgecmd.h>

#define emit(out, cmd) do { \
    *out++ = cmd; \
} while(0)

#define jump(out, target) do { \
    emit(out, GE_SET_BASE_BASE8(target)); \
    emit(out, GE_SET_JUMP_ADDR24(target)); \
} while(0)

#include <pspge.h>
#include <pspdisplay.h>

template<> Ge *Singleton<Ge>::objectPtr;
u8 D_eboot_08A5DD14;
volatile bool GE_END_REACHED;
u32 D_eboot_08A5DD18;
u32 D_eboot_08A5DD1C;

s16 BLANK_BUFFER_VERTEX_DATA[2][4] = {
    {0, 0, 0, -1},
    {0, 480, 272, -1},
};

ge_command BLANK_BUFFER_DISPLAY_LISTS[2][12] = {
    {
        GE_SET_FBP_ADDR24(0x04000000), // Framebuf ptr: 0x04000000
        0x9D000200, // Framebuf stride: 0200
        0x9E088000, // Zbuf ptr: 088000
        0x9F000200, // Zbuf stride: 0200
        0xD3000701, // Clear mode: on, color, alpha/stencil, depth
        0x12800114, // SetVertexType: through, ABGR 1555 colors, s16 positions
        GE_SET_BASE_BASE8(BLANK_BUFFER_VERTEX_DATA),
        GE_SET_VADR_ADDR24(BLANK_BUFFER_VERTEX_DATA),
        0x04060002, // DRAW PRIM RECTANGLES: count= 2 vaddr= 00000000
        0xD3000000, // Clear mode: off
        0x0F000000, // FINISH 000000
        0x0C000000, // END
    }, {
        GE_SET_FBP_ADDR24(0x04044000), // Framebuf ptr: 0x04044000
        0x9D000200, // Framebuf stride: 0200
        0x9E088000, // Zbuf ptr: 088000
        0x9F000200, // Zbuf stride: 0200
        0xD3000701, // Clear mode: on, color, alpha/stencil, depth
        0x12800114, // SetVertexType: through, ABGR 1555 colors, s16 positions
        GE_SET_BASE_BASE8(BLANK_BUFFER_VERTEX_DATA),
        GE_SET_VADR_ADDR24(BLANK_BUFFER_VERTEX_DATA),
        0x04060002, // DRAW PRIM RECTANGLES: count= 2 vaddr= 00000000
        0xD3000000, // Clear mode: off
        0x0F000000, // FINISH 000000
        0x0C000000, // END
    }
};

static ge_command INITIALIZE_GE_DISPLAY_LIST[221] = {
    0x01000000, // VADDR: 000000 => 00000000
    0x02000000, // IADDR: 000000 => 00000000
    0x10000000, // BASE: high=00
    0x12000000, // SetVertexType: e
    0x13000000, // OffsetAddr: 000000 (offset=00000000)
    0x15000000, // Region Rate: 0 0
    0x16043ddf, // Region BR: 479 271
    0x17000000, // Lighting enable: 0
    0x18000000, // Light 0 enable: 0
    0x19000000, // Light 1 enable: 0
    0x1a000000, // Light 2 enable: 0
    0x1b000000, // Light 3 enable: 0
    0x1c000000, // Depth clamp enable: 0
    0x1d000000, // CullFace enable: 0
    0x1e000000, // Texture map enable: 0
    0x1f000000, // Fog enable: 0
    0x20000000, // Dither enable: 0
    0x21000000, // Alpha blend enable: 0
    0x22000000, // Alpha test enable: 0
    0x23000000, // Z test enable: 0
    0x24000000, // Stencil test enable: 0
    0x25000000, // Antialias enable: 0
    0x26000000, // Patch cull enable: 0
    0x27000000, // Color test enable: 0
    0x28000000, // Logic op enable: 0
    0x2a000000, // BONE #0
    0x2b000000, // BONE data # 0.000000
    0x2c000000, // MorphWeight 0 = 0.000000
    0x2d000000, // MorphWeight 1 = 0.000000
    0x2e000000, // MorphWeight 2 = 0.000000
    0x2f000000, // MorphWeight 3 = 0.000000
    0x30000000, // MorphWeight 4 = 0.000000
    0x31000000, // MorphWeight 5 = 0.000000
    0x32000000, // MorphWeight 6 = 0.000000
    0x33000000, // MorphWeight 7 = 0.000000
    0x36001010, // Patch subdivision: 16 x 16
    0x37000000, // Patch Primitive: triangles
    0x38000000, // Patch Facing: standard normals
    0x3a000000, // World # 0
    0x3b000000, // World data # 0.000000
    0x3c000000, // VIEW # 0
    0x3d000000, // VIEW data # 0.000000
    0x3e000000, // PROJECTION # 0
    0x3f000000, // PROJECTION matrix data # 0.000000
    0x40000000, // TGEN # 0
    0x41000000, // TGEN data # 0.000000
    0x42000000, // Viewport X scale: 0.000000
    0x43000000, // Viewport Y scale: 0.000000
    0x44000000, // Viewport Z scale: 0.000000
    0x45000000, // Viewport X center: 0.000000
    0x46000000, // Viewport Y center: 0.000000
    0x47000000, // Viewport Z center: 0.000000
    0x483f8000, // Texture U scale: 1.000000
    0x493f8000, // Texture V scale: 1.000000
    0x4a000000, // Texture U offset: 0.000000
    0x4b000000, // Texture V offset: 0.000000
    0x4c000000, // Offset X: 0000 / 0 with sub 0
    0x4d000000, // Offset Y: 0000 / 0 with sub 0
    0x50000000, // Shade: 0 (flat)
    0x51000000, // Reverse normal: standard
    0x53000007, // Material update: ambient, diffuse, specular
    0x54000000, // Material emissive color: 000000
    0x55000000, // Material ambient color: 000000
    0x56000000, // Material diffuse color: 000000
    0x57000000, // Material specular color: 000000
    0x58000000, // Material alpha: 00
    0x5b3f8000, // Material specular coef: 1.000000
    0x5c000000, // Ambient color: 000000
    0x5d000000, // Ambient alpha: 00
    0x5e000000, // Lightmode: 0 (single color)
    0x5f000000, // Light 0 type: directional, comp: diffuse
    0x60000000, // Light 1 type: directional, comp: diffuse
    0x61000000, // Light 2 type: directional, comp: diffuse
    0x62000000, // Light 3 type: directional, comp: diffuse
    0x63000000, // Light 0 X pos: 0.000000
    0x64000000, // Light 0 Y pos: 0.000000
    0x65000000, // Light 0 Z pos: 0.000000
    0x66000000, // Light 1 X pos: 0.000000
    0x67000000, // Light 1 Y pos: 0.000000
    0x68000000, // Light 1 Z pos: 0.000000
    0x69000000, // Light 2 X pos: 0.000000
    0x6a000000, // Light 2 Y pos: 0.000000
    0x6b000000, // Light 2 Z pos: 0.000000
    0x6c000000, // Light 3 X pos: 0.000000
    0x6d000000, // Light 3 Y pos: 0.000000
    0x6e000000, // Light 3 Z pos: 0.000000
    0x6f000000, // Light 0 X dir: 0.000000
    0x70000000, // Light 0 Y dir: 0.000000
    0x71000000, // Light 0 Z dir: 0.000000
    0x72000000, // Light 1 X dir: 0.000000
    0x73000000, // Light 1 Y dir: 0.000000
    0x74000000, // Light 1 Z dir: 0.000000
    0x75000000, // Light 2 X dir: 0.000000
    0x76000000, // Light 2 Y dir: 0.000000
    0x77000000, // Light 2 Z dir: 0.000000
    0x78000000, // Light 3 X dir: 0.000000
    0x79000000, // Light 3 Y dir: 0.000000
    0x7a000000, // Light 3 Z dir: 0.000000
    0x7b000000, // Light 0 X att: 0.000000
    0x7c000000, // Light 0 Y att: 0.000000
    0x7d000000, // Light 0 Z att: 0.000000
    0x7e000000, // Light 1 X att: 0.000000
    0x7f000000, // Light 1 Y att: 0.000000
    0x80000000, // Light 1 Z att: 0.000000
    0x81000000, // Light 2 X att: 0.000000
    0x82000000, // Light 2 Y att: 0.000000
    0x83000000, // Light 2 Z att: 0.000000
    0x84000000, // Light 3 X att: 0.000000
    0x85000000, // Light 3 Y att: 0.000000
    0x86000000, // Light 3 Z att: 0.000000
    0x87000000, // Light 0 spot exponent: 0.000000
    0x88000000, // Light 1 spot exponent: 0.000000
    0x89000000, // Light 2 spot exponent: 0.000000
    0x8a000000, // Light 3 spot exponent: 0.000000
    0x8b000000, // Light 0 spot cutoff: 0.000000
    0x8c000000, // Light 1 spot cutoff: 0.000000
    0x8d000000, // Light 2 spot cutoff: 0.000000
    0x8e000000, // Light 3 spot cutoff: 0.000000
    0x8f000000, // Light 0 ambient color: 0.000000 0.000000 0.000000
    0x90000000, // Light 0 diffuse color: 0.000000 0.000000 0.000000
    0x91000000, // Light 0 specular color: 0.000000 0.000000 0.000000
    0x92000000, // Light 1 ambient color: 0.000000 0.000000 0.000000
    0x93000000, // Light 1 diffuse color: 0.000000 0.000000 0.000000
    0x94000000, // Light 1 specular color: 0.000000 0.000000 0.000000
    0x95000000, // Light 2 ambient color: 0.000000 0.000000 0.000000
    0x96000000, // Light 2 diffuse color: 0.000000 0.000000 0.000000
    0x97000000, // Light 2 specular color: 0.000000 0.000000 0.000000
    0x98000000, // Light 3 ambient color: 0.000000 0.000000 0.000000
    0x99000000, // Light 3 diffuse color: 0.000000 0.000000 0.000000
    0x9a000000, // Light 3 specular color: 0.000000 0.000000 0.000000
    0x9b000000, // Cull: front (CW)
    0x9c000000, // Framebuf ptr: 0x04000000
    0x9d000000, // Framebuf stride: 0000
    0x9e000000, // Zbuf ptr: 000000
    0x9f000000, // Zbuf stride: 0000
    0xa0000000, // Texture address 0: low=000000
    0xa1000000, // Texture address 1: low=000000
    0xa2000000, // Texture address 2: low=000000
    0xa3000000, // Texture address 3: low=000000
    0xa4000000, // Texture address 4: low=000000
    0xa5000000, // Texture address 5: low=000000
    0xa6000000, // Texture address 6: low=000000
    0xa7000000, // Texture address 7: low=000000
    0xa8000000, // Texture stride 0: 0x0000, address high=00
    0xa9000000, // Texture stride 1: 0x0000, address high=00
    0xaa000000, // Texture stride 2: 0x0000, address high=00
    0xab000000, // Texture stride 3: 0x0000, address high=00
    0xac000000, // Texture stride 4: 0x0000, address high=00
    0xad000000, // Texture stride 5: 0x0000, address high=00
    0xae000000, // Texture stride 6: 0x0000, address high=00
    0xaf000000, // Texture stride 7: 0x0000, address high=00
    0xb0000000, // CLUT addr: low=000000
    0xb1000000, // CLUT addr: high=00
    0xb2000000, // Block transfer src: low=000000
    0xb3000000, // Block transfer src: high=00, w=0 (addr 00000000)
    0xb4000000, // Block transfer dst: low=000000
    0xb5000000, // Block transfer dst: high=00, w=0 (addr 00000000)
    0xb8000000, // Texture size 0: 1x1
    0xb9000000, // Texture size 1: 1x1
    0xba000000, // Texture size 2: 1x1
    0xbb000000, // Texture size 3: 1x1
    0xbc000000, // Texture size 4: 1x1
    0xbd000000, // Texture size 5: 1x1
    0xbe000000, // Texture size 6: 1x1
    0xbf000000, // Texture size 7: 1x1
    0xc0000000, // Tex map mode: uvgen=texcoords, uvproj=pos
    0xc1000000, // Tex shade light sources: 0, 0
    0xc2000000, // TexMode no swizzle, 0 levels, shared clut
    0xc3000000, // TexFormat 5650
    0xc4000000, // Clut load: 00000000, 0 bytes
    0xc5000000, // Clut format: 000000 (BGR 5650)
    0xc6000000, // TexFilter min: nearest, mag: nearest
    0xc7000000, // TexWrap wrap s, wrap t
    0xc8000000, // TexLevel mode: auto + bias Offset: 0.000000 / 0
    0xc9000000, // TexFunc 0 RGB modulate
    0xca000000, // TexEnvColor 000000
    0xcb000000, // TexFlush
    0xcc000000, // TexSync
    0xcd000000, // Fog end 0.000000
    0xce000000, // Fog slope 0.000000
    0xcf000000, // FogColor 000000
    0xd0000000, // TexLodSlope 0.000000
    0xd2000001, // Framebuf PixelFormat: 5551
    0xd3000000, // Clear mode: off
    0xd4000000, // Scissor TL: 0, 0
    0xd5043ddf, // Scissor BR: 479, 271
    0xd6000000, // MinZ: 0x0000 / 0.000000
    0xd700ffff, // MaxZ: 0xffff / 1.000000
    0xd8000000, // ColorTest: (src.rgb & cmask) NEVER (dst.rgb & cmask)
    0xd9000000, // ColorRef: 000000
    0xda000000, // ColorTestMask: 000000
    0xdb000000, // Alpha test: (src.a & 00) NEVER 00
    0xdc000000, // Stencil test: 00 NEVER (dst.a & 00)
    0xdd000000, // Stencil op: fail=KEEP, pass/depthfail=KEEP, pass=KEEP
    0xde000000, // Z test mode: NEVER
    0xdf000000, // Blend mode: add dst, src
    0xe0000000, // Blend fix A: 000000
    0xe1000000, // Blend fix B: 000000
    0xe2001d0c, // DitherMatrix 0 = 001d0c
    0xe300f3e2, // DitherMatrix 1 = 00f3e2
    0xe4000c1d, // DitherMatrix 2 = 000c1d
    0xe500e2f3, // DitherMatrix 3 = 00e2f3
    0xe6000000, // LogicOp: clear
    0xe7000000, // ZMask: allow write
    0xe8000000, // MaskRGB: 000000 (bits not to write)
    0xe9000000, // MaskAlpha: 00 (bits not to write)
    0xeb000000, // Block transfer src rect TL: 0, 0
    0xec000000, // Block transfer dst rect TL: 0, 0
    0xee000000, // Block transfer rect size: 1 x 1
    0xf0000000, // Vertex screen X: 0000 / 0 with sub 0
    0xf1000000, // Vertex screen Y: 0000 / 0 with sub 0
    0xf2000000, // Vertex Z: 0000
    0xf3000000, // Vertex tex S: 0.000000
    0xf4000000, // Vertex tex T: 0.000000
    0xf5000000, // Vertex tex Q: 0.000000
    0xf6000000, // Vertex color: 000000
    0xf7000000, // Vertex draw: alpha=00, prim=POINTS
    0xf8000000, // Vertex fog: 00 / 0.000000
    0xf9000000, // Vertex secondary color: 000000
    0x0f000000, // FINISH 000000
    0x0c000000, // END
};

void ge_finish_callback(int, void*);

void Ge::initialize() {
  sceDisplaySetMode(0,0x1e0,0x110);
  sceGeEdramSetAddrTranslation(0x400);

  active_buffer = 0;
  clear_display_list();
  sceGeListSync(sceGeListEnQueue(INITIALIZE_GE_DISPLAY_LIST,(void *)0x0,-1,(PspGeListArgs *)0x0),0);

  PspGeCallbackData cb;
  cb.finish_func = ge_finish_callback;
  cb.signal_func = (PspGeCallback)0x0;
  cb.signal_arg = (void *)0x0;
  cb.finish_arg = (void *)0x0;
  ge_callback_id = sceGeSetCallback(&cb);

  swap_buffers();
  clear_display_list();
  GE_END_REACHED = 1;
  waiting_for_ge = 0;
}

INCLUDE_ASM("asm/eboot/nonmatchings/ge", func_eboot_08858FD0);

INCLUDE_ASM("asm/eboot/nonmatchings/ge", func_eboot_08859094);

INCLUDE_ASM("asm/eboot/nonmatchings/ge", func_eboot_088590A8);

INCLUDE_ASM("asm/eboot/nonmatchings/ge", func_eboot_08859138);

void Ge::swap_buffers() {
  active_buffer ^= 1;
  sceGeListSync(sceGeListEnQueue(BLANK_BUFFER_DISPLAY_LISTS[active_buffer],(void *)0x0,-1,(PspGeListArgs *)0x0),0);
  active_write_head = slab[active_buffer];
  clear_display_list();
}

INCLUDE_ASM("asm/eboot/nonmatchings/ge", func_eboot_088591C8);

static ge_command FINISH_END_DISPLAY_LIST[2] = {
    0xf000000, // FINISH 000000
    0xc000000, // END
};

#pragma opt_unroll_loops on
void Ge::clear_display_list() {
    display_list cur = &root_display_lists[active_buffer][0];
    display_list temp, target;
    int i;
    for (i = 0; i < 19; ++i) {
        temp = cur;
        target = cur + 2;
        jump(cur, target);
        active_branches[i] = temp;
    }
    temp = cur;
    jump(cur, FINISH_END_DISPLAY_LIST);
    active_branches[i] = temp;
}
#pragma opt_unroll_loops reset

INCLUDE_ASM("asm/eboot/nonmatchings/ge", method_088593A0__2GeFPUiii);

INCLUDE_ASM("asm/eboot/nonmatchings/ge", func_eboot_088595E8);

void Ge::render() {
    GE_END_REACHED = 0;
    sceGeListEnQueue(root_display_lists[active_buffer ^ 1], 0, ge_callback_id, 0);
}

void ge_finish_callback(int, void*) {
#ifdef __MWERKS__
    __asm__ volatile (
        "sync"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
        "nop"
    );
#endif
    GE_END_REACHED = 1;
}

void Ge::spinlock_until_ge_end() {
    while (!GE_END_REACHED) {
#ifdef __MWERKS__
        asm __volatile__ (
            "nop"
            "nop"
        );
#endif
    }
}

ge_command *Ge::write_head() {
    return active_write_head;
}

void Ge::set_write_head(ge_command *value) {
    active_write_head = value;
}

INCLUDE_ASM("asm/eboot/nonmatchings/ge", method_0885973C__2GeFUi);

INCLUDE_ASM("asm/eboot/nonmatchings/ge", method_08859768__2GeFP10tmh_headeriUiUiP9GeTexture);
