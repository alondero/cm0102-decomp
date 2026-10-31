/* Preferred-image VA 0x0062c2f0, image base 0x00400000, CM0102 3.9.68.
   Thiscall (ECX = self) with one stack argument (`ret 4`): index. Field
   offset from self: 0x00 is a pointer to a table of 31-byte records (index *
   31, the 16-bit field at +0xc is updated). Global 0x00ae2364 is the signed
   bound (index >= bound returns index unchanged); global 0x00ae23bc is the base
   of a table of 110-byte rows (index * 110): signed byte at +0x18, and a dword
   pointer at +0x61 whose target byte at +0xf must equal 0x14.
   Callee 0x0090cfc0 is cdecl with one int argument (called with 25, then 13);
   it stays an original boundary. EAX at return is tracked per path: the index,
   the record address, the callee result, or the clamped 16-bit value. */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 param_1);

u32 CM_THISCALL cm_fn_0062c2f0(u32 self, u32 index)
{
    u32 ret;
    u32 ofs31;
    u32 ofs110;
    u32 base;
    u32 elem;
    u32 tab;
    u32 row;
    s32 bv;
    s32 c;
    u32 v;

    ret = index;
    if ((s32)index >= CM_GLOBAL(volatile s32, 0x00ae2364))
        goto L3c6;

    base = *(volatile u32 *)self;
    ofs31 = index * 31u;
    elem = base + ofs31 + 0xcu;
    v = *(volatile u16 *)elem;
    if ((s16)v < (s16)0xfc18) {
        v = (u16)(v - 1u);
        *(volatile u16 *)elem = (u16)v;
    }

    ofs110 = index * 110u;
    tab = CM_GLOBAL(volatile u32, 0x00ae23bc);
    ret = tab;
    bv = *(volatile s8 *)(tab + ofs110 + 0x18u);
    if (bv < 0x13) {
        base = *(volatile u32 *)self;
        elem = base + ofs31 + 0xcu;
        ret = elem;
        if ((s16)*(volatile u16 *)elem < 0) {
            *(volatile u16 *)elem = 0;
            goto L396;
        }
    }

    base = *(volatile u32 *)self;
    elem = base + ofs31 + 0xcu;
    if ((s16)*(volatile u16 *)elem < (s16)0xfe0c)
        goto L380;

    c = cm_fn_0090cfc0(0x19);
    ret = (u32)c;
    tab = CM_GLOBAL(volatile u32, 0x00ae23bc);
    bv = *(volatile s8 *)(tab + ofs110 + 0x18u);
    if (bv < c)
        goto L380;

    base = *(volatile u32 *)self;
    elem = base + ofs31 + 0xcu;
    v = *(volatile u16 *)elem;
    *(volatile u16 *)elem = (u16)(v - 13u);
    ret = elem;
    goto L396;

L380:
    base = *(volatile u32 *)self;
    elem = base + ofs31 + 0xcu;
    c = cm_fn_0090cfc0(0x0d);
    ret = (u32)c;
    v = *(volatile u16 *)elem;
    *(volatile u16 *)elem = (u16)(v - (u16)c);

L396:
    tab = CM_GLOBAL(volatile u32, 0x00ae23bc);
    row = *(volatile u32 *)(tab + ofs110 + 0x61u);
    if (row == 0)
        goto L3c6;
    if (*(volatile u8 *)(row + 0xf) != 0x14)
        goto L3c6;

    base = *(volatile u32 *)self;
    elem = base + ofs31 + 0xcu;
    v = *(volatile u16 *)elem;
    if ((s16)v < (s16)0x041a) {
        ret = (u32)(s32)(s16)v;
        *(volatile u16 *)elem = (u16)v;
    } else {
        ret = 0x041au;
        *(volatile u16 *)elem = 0x041au;
    }

L3c6:
    return ret;
}
