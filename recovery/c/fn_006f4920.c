#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 param_1);

/* Thiscall (ECX = self), four stack arguments (`ret 0x10`), no return value.
   Byte arguments: arg_04, arg_08 (subtracted from arg_0c and arg_10 to form
   two differences), arg_0c (bl), arg_10 (dl). Field offsets from self: 0x105
   and 0x10c are signed bytes multiplied together; 0x19a is a u16 result word;
   0x6d is a pointer whose +0x30 and +0x3b bytes are signed. The table at
   0x00b67c30 is indexed by (int8)diff_c + 24 * (int8)diff_a (u16 entries).
   The listing stores the second byte into the arg_0c stack slot (not
   modelled as a field, since only its value is read back).
   Word arithmetic is done at 16 bits; the final word is clamped to 0..0x167
   after the range checks (the negative-to-positive and over-range adjustments). */
void CM_THISCALL cm_fn_006f4920(void *self, u32 arg_04, u32 arg_08, u32 arg_0c, u32 arg_10)
{
    volatile u8 *base = (volatile u8 *)self;
    u8 al;
    u8 cl;
    u8 dl;
    u8 bl;
    u8 cl_new;
    u8 bl_new;
    u16 word;
    u16 w16;
    s32 eax_v;
    s32 edx_v;
    s32 r1;
    s32 r2;
    s32 edi_v;
    u32 ptr;
    s32 d;
    s32 a;

    bl = (u8)arg_0c;
    dl = (u8)arg_10;
    al = (u8)(bl - (u8)arg_04);
    cl = (u8)(dl - (u8)arg_08);

    if (al == 0 && cl == 0 && dl == 0x0b && bl == 4) {
        *(volatile u16 *)(base + 0x19a) = 0;
    } else if (al == 0 && cl == 0 && dl == 0 && bl == 4) {
        *(volatile u16 *)(base + 0x19a) = 0xb4;
    } else {
        if (al != 0 || cl != 0) {
            s32 idx;
            idx = (s32)(s8)cl + 24 * (s32)(s8)al;
            word = *(volatile u16 *)((u32)0x00b67c30 + (u32)idx * 2u);
        } else {
            eax_v = cm_fn_0090cfc0(0x168);
            word = (u16)eax_v;
        }
        *(volatile u16 *)(base + 0x19a) = word;
    }

    cl_new = *(volatile u8 *)(base + 0x10c);
    bl_new = *(volatile u8 *)(base + 0x105);
    eax_v = cm_fn_0090cfc0(0x78);
    edx_v = (s32)(s8)cl_new * (s32)(s8)bl_new;
    if (edx_v < eax_v) {
        r1 = cm_fn_0090cfc0(0x1e);
        edi_v = r1;
        r2 = cm_fn_0090cfc0(0x1e);
        edi_v = edi_v - r2;
        *(volatile u16 *)(base + 0x19a) = (u16)(*(volatile u16 *)(base + 0x19a) + (u16)edi_v);
    }

    ptr = *(volatile u32 *)(base + 0x6d);
    d = (s32)*(volatile s8 *)(ptr + 0x3b);
    a = (s32)*(volatile s8 *)(ptr + 0x30);
    word = (u16)((u16)((u16)d - (u16)a) << 1);
    *(volatile u16 *)(base + 0x19a) = (u16)(*(volatile u16 *)(base + 0x19a) + word);

    w16 = *(volatile u16 *)(base + 0x19a);
    if ((s16)w16 < 0) {
        *(volatile u16 *)(base + 0x19a) = (u16)(w16 + 0x168u);
    } else if ((s16)w16 > 0x167) {
        *(volatile u16 *)(base + 0x19a) = (u16)(w16 - 0x168u);
    }
}
