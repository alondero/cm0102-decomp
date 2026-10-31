#include "cmre.h"

/* Memory access helpers. Volatile keeps the listing's load/store order and
   the byte, word and dword widths for every access. */
#define P8(a) (*(volatile u8 *)(u32)(a))
#define P16(a) (*(volatile u16 *)(u32)(a))
#define P32(a) (*(volatile u32 *)(u32)(a))

/* Thiscall sub-object copies: this in ECX, one stack argument popped by the
   callee (listing shows no add esp after each call). Return values unused. */
extern u32 CM_THISCALL cm_fn_00448110(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_00448170(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_00448490(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_004a1fb0(u32 self, u32 src);

/* Copies the record at the stack argument (src) into this (ECX), field by
   field, calling sub-object copy routines between the field groups. Returns
   this. Offsets are byte offsets from the record base. */
u32 CM_THISCALL cm_fn_004ab5f0(u32 self, u32 src)
{
    u32 esi;
    u32 edi;

    esi = self;
    edi = src;

    P32(esi) = P32(edi);
    P8(esi + 4) = P8(edi + 4);
    P32(esi + 5) = P32(edi + 5);
    P32(esi + 9) = P32(edi + 9);
    P32(esi + 0xd) = P32(edi + 0xd);
    P32(esi + 0x11) = P32(edi + 0x11);
    P32(esi + 0x15) = P32(edi + 0x15);
    P32(esi + 0x19) = P32(edi + 0x19);
    P32(esi + 0x1d) = P32(edi + 0x1d);
    P32(esi + 0x21) = P32(edi + 0x21);
    P32(esi + 0x25) = P32(edi + 0x25);
    cm_fn_00448110(esi + 0x29, edi + 0x29);
    P32(esi + 0x48) = P32(edi + 0x48);
    cm_fn_00448110(esi + 0x4c, edi + 0x4c);

    /* 0x4ab65a: 10-byte block (dword, dword, word) copied directly. */
    P32(esi + 0x6b) = P32(edi + 0x6b);
    P32(esi + 0x6b + 4) = P32(edi + 0x6b + 4);
    P16(esi + 0x6b + 8) = P16(edi + 0x6b + 8);
    cm_fn_00448110(esi + 0x75, edi + 0x75);
    cm_fn_00448110(esi + 0x94, edi + 0x94);

    cm_fn_00448170(esi + 0xb3, edi + 0xb3);
    P32(esi + 0xb3 + 0x14) = P32(edi + 0xb3 + 0x14);
    cm_fn_00448170(esi + 0xcb, edi + 0xcb);
    P32(esi + 0xcb + 0x14) = P32(edi + 0xcb + 0x14);
    cm_fn_00448170(esi + 0xe3, edi + 0xe3);
    P32(esi + 0xe3 + 0x14) = P32(edi + 0xe3 + 0x14);
    cm_fn_00448170(esi + 0xfb, edi + 0xfb);
    P32(esi + 0xfb + 0x14) = P32(edi + 0xfb + 0x14);

    /* Byte at +0x113 is stored before the call at +0x114. */
    P8(esi + 0x113) = P8(edi + 0x113);
    cm_fn_00448490(esi + 0x114, edi + 0x114);

    P32(esi + 0x12c) = P32(edi + 0x12c);
    P16(esi + 0x130) = P16(edi + 0x130);
    P8(esi + 0x132) = P8(edi + 0x132);
    cm_fn_00448490(esi + 0x133, edi + 0x133);

    cm_fn_00448110(esi + 0x14b, edi + 0x14b);

    P8(esi + 0x16a) = P8(edi + 0x16a);
    cm_fn_00448490(esi + 0x16b, edi + 0x16b);

    P32(esi + 0x183) = P32(edi + 0x183);
    P16(esi + 0x187) = P16(edi + 0x187);
    P32(esi + 0x189) = P32(edi + 0x189);
    cm_fn_00448490(esi + 0x18d, edi + 0x18d);

    P8(esi + 0x1a5) = P8(edi + 0x1a5);
    P32(esi + 0x1a6) = P32(edi + 0x1a6);
    P16(esi + 0x1aa) = P16(edi + 0x1aa);
    P8(esi + 0x1ac) = P8(edi + 0x1ac);
    cm_fn_00448490(esi + 0x1ad, edi + 0x1ad);

    P32(esi + 0x1c5) = P32(edi + 0x1c5);
    P16(esi + 0x1c9) = P16(edi + 0x1c9);
    P8(esi + 0x1cb) = P8(edi + 0x1cb);
    P8(esi + 0x1cc) = P8(edi + 0x1cc);
    cm_fn_00448490(esi + 0x1cd, edi + 0x1cd);

    P32(esi + 0x1e5) = P32(edi + 0x1e5);
    P16(esi + 0x1e9) = P16(edi + 0x1e9);
    cm_fn_004a1fb0(esi + 0x1eb, edi + 0x1eb);

    edi = edi + 0x22a;
    cm_fn_004a1fb0(esi + 0x22a, edi);

    return esi;
}
