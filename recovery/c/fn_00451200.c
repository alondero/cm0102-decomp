/* Plain-C recovery of the record copy routine at preferred-image VA 00451200
   (image base 00400000). Thiscall: ECX is the destination record (`self`),
   one stack argument is the source record (`src`), callee pops 4 bytes.
   Returns the destination pointer (EAX = ESI = self at 0045155a).

   Every field is copied at its listed width, read then written, in listing
   order. Sub-records are copied through the thiscall helpers at 00448110,
   00448170, 00448490, 00448550 and 004a1fb0, which stay external boundaries.
   Field offsets are relative to the record base only. */
#include "cmre.h"

#define RD_U8(base, off)  (*(volatile u8 *)((base) + (off)))
#define RD_U16(base, off) (*(volatile u16 *)((base) + (off)))
#define RD_U32(base, off) (*(volatile u32 *)((base) + (off)))

extern u32 CM_THISCALL cm_fn_00448110(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_00448170(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_00448490(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_00448550(u32 self, u32 src);
extern u32 CM_THISCALL cm_fn_004a1fb0(u32 self, u32 src);

u32 CM_THISCALL cm_fn_00451200(u32 self, u32 src)
{
    u32 dst = self;

    /* 0045120b-0045121d: header fields, dword, word, byte */
    RD_U32(dst, 0x00) = RD_U32(src, 0x00);
    RD_U16(dst, 0x04) = RD_U16(src, 0x04);
    RD_U8(dst, 0x06) = RD_U8(src, 0x06);

    /* 00451223-0045122c: dword at +7, word at +0xb (source via ECX = src+7) */
    RD_U32(dst, 0x07) = RD_U32(src, 0x07);
    RD_U16(dst, 0x0b) = RD_U16(src, 0x0b);

    /* 00451233-00451237: byte at +0xd; dword at +0xe and call on +0x12 */
    RD_U8(dst, 0x0d) = RD_U8(src, 0x0d);
    RD_U32(dst, 0x0e) = RD_U32(src, 0x0e);
    cm_fn_00448110(dst + 0x12, src + 0x12);

    RD_U32(dst, 0x31) = RD_U32(src, 0x31);
    cm_fn_00448110(dst + 0x35, src + 0x35);

    RD_U32(dst, 0x54) = RD_U32(src, 0x54);
    cm_fn_00448110(dst + 0x58, src + 0x58);

    /* 00451272-0045127d: dword at +0x77, word at +0x7b */
    RD_U32(dst, 0x77) = RD_U32(src, 0x77);
    RD_U16(dst, 0x7b) = RD_U16(src, 0x7b);
    cm_fn_00448110(dst + 0x7d, src + 0x7d);

    cm_fn_00448110(dst + 0x9c, src + 0x9c);
    cm_fn_00448110(dst + 0xbb, src + 0xbb);
    cm_fn_00448110(dst + 0xda, src + 0xda);
    cm_fn_00448110(dst + 0xf9, src + 0xf9);

    cm_fn_00448170(dst + 0x118, src + 0x118);
    cm_fn_00448170(dst + 0x12c, src + 0x12c);
    cm_fn_00448170(dst + 0x140, src + 0x140);
    cm_fn_00448170(dst + 0x154, src + 0x154);

    /* 0045131a-0045135f: byte at +0x168 then helper at +0x169 */
    RD_U8(dst, 0x168) = RD_U8(src, 0x168);
    cm_fn_00448490(dst + 0x169, src + 0x169);

    /* 00451338-0045135f: word at +0x181, byte at +0x183, helper at +0x184 */
    RD_U16(dst, 0x181) = RD_U16(src, 0x181);
    RD_U8(dst, 0x183) = RD_U8(src, 0x183);
    cm_fn_00448490(dst + 0x184, src + 0x184);

    /* 00451364-0045138b: word at +0x19c, byte at +0x19e, helper at +0x19f */
    RD_U16(dst, 0x19c) = RD_U16(src, 0x19c);
    RD_U8(dst, 0x19e) = RD_U8(src, 0x19e);
    cm_fn_00448490(dst + 0x19f, src + 0x19f);

    cm_fn_00448110(dst + 0x1b7, src + 0x1b7);

    /* 004513a2-004513bb: byte at +0x1d6, helper at +0x1d7 */
    RD_U8(dst, 0x1d6) = RD_U8(src, 0x1d6);
    cm_fn_00448490(dst + 0x1d7, src + 0x1d7);

    /* 004513c0-004513e7: word at +0x1ef, dword at +0x1f1, helper at +0x1f5 */
    RD_U16(dst, 0x1ef) = RD_U16(src, 0x1ef);
    RD_U32(dst, 0x1f1) = RD_U32(src, 0x1f1);
    cm_fn_00448490(dst + 0x1f5, src + 0x1f5);

    /* 004513ec-0045141f: byte +0x20d, word +0x20e, byte +0x210, helper +0x211 */
    RD_U8(dst, 0x20d) = RD_U8(src, 0x20d);
    RD_U16(dst, 0x20e) = RD_U16(src, 0x20e);
    RD_U8(dst, 0x210) = RD_U8(src, 0x210);
    cm_fn_00448490(dst + 0x211, src + 0x211);

    /* 00451424-00451457: word +0x229, byte +0x22b, byte +0x22c, helper +0x22d */
    RD_U16(dst, 0x229) = RD_U16(src, 0x229);
    RD_U8(dst, 0x22b) = RD_U8(src, 0x22b);
    RD_U8(dst, 0x22c) = RD_U8(src, 0x22c);
    cm_fn_00448490(dst + 0x22d, src + 0x22d);

    /* 0045145c-00451485: word +0x245, word +0x247, helper +0x249 */
    RD_U16(dst, 0x245) = RD_U16(src, 0x245);
    RD_U16(dst, 0x247) = RD_U16(src, 0x247);
    cm_fn_00448490(dst + 0x249, src + 0x249);

    /* 0045148a-004514a5: word +0x261, helper +0x263 */
    RD_U16(dst, 0x261) = RD_U16(src, 0x261);
    cm_fn_00448490(dst + 0x263, src + 0x263);

    cm_fn_004a1fb0(dst + 0x27b, src + 0x27b);
    cm_fn_004a1fb0(dst + 0x2ba, src + 0x2ba);

    /* 004514ce-004514e9: word +0x2f9 then helper +0x2fb */
    RD_U16(dst, 0x2f9) = RD_U16(src, 0x2f9);
    cm_fn_00448490(dst + 0x2fb, src + 0x2fb);

    cm_fn_00448550(dst + 0x313, src + 0x313);
    cm_fn_00448550(dst + 0x33c, src + 0x33c);

    /* 00451524-0045154f: dword +0x365, word +0x369, dword +0x36b, word +0x36f */
    RD_U32(dst, 0x365) = RD_U32(src, 0x365);
    RD_U16(dst, 0x369) = RD_U16(src, 0x369);
    RD_U32(dst, 0x36b) = RD_U32(src, 0x36b);
    RD_U16(dst, 0x36f) = RD_U16(src, 0x36f);
    cm_fn_00448490(dst + 0x371, src + 0x371);

    /* 00451556-0045155d: after the call, word at source +0x389 (source is
       src+0x371 here) copied to destination +0x389. */
    RD_U16(dst, 0x389) = RD_U16(src, 0x389);

    return dst;
}
