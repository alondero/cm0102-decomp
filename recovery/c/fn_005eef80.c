#include "cmre.h"

/* Thiscall with two stack arguments (ret 8). flag (arg1) selects the 565 masks
   (non-zero: red 0xf800, green 0x7e0) or the 555 masks (zero: 0x7c00, 0x3e0);
   value (arg2) selects which of three channels are converted by bits 0, 1 and 2.
   The 0x20-byte scratch area holds the output dword at +0, the output pointer
   area at +4, and the masks at +0x14 (red), +0x18 (green) and +0x1c (blue, 0x1f).
   Each channel uses a 16-bit field of the object (+0x72, +0x74, +0x78); the packed
   first argument to 0x005e49f0 keeps the high half of the argument slot address,
   as the original's `mov cx, word` does. 0x005e49f0 writes bytes into the flag and
   value argument slots, so the channel bits are taken at entry and the later
   calls read the slots again. */

extern void CM_CDECL cm_fn_005e49f0(u32 packed, u8 *table, u32 *a, u32 *b, u32 *out);
extern u32 CM_CDECL cm_fn_005e4800(u32 arg1, u32 arg2, u32 arg3, u32 *arg4);

u32 CM_THISCALL cm_fn_005eef80(u32 self, u32 flag, u32 value)
{
    u8 scratch[0x20];
    u32 mask_red;
    u32 mask_green;
    u32 packed;
    u16 word;
    u32 *out;
    u32 arg1_addr;
    u32 arg2_addr;
    u32 table_base;
    u32 result;
    u32 r_on;
    u32 g_on;
    u32 b_on;

    out = (u32 *)(scratch + 4);
    arg1_addr = (u32)&flag;
    arg2_addr = (u32)&value;
    table_base = (u32)scratch;
    result = flag;
    r_on = value & 1u;
    g_on = value & 2u;
    b_on = value & 4u;

    if (flag != 0) {
        mask_red = 0xf800u;
        mask_green = 0x7e0u;
    } else {
        mask_red = 0x7c00u;
        mask_green = 0x3e0u;
    }
    *(volatile u32 *)(scratch + 0x14) = mask_red;
    *(volatile u32 *)(scratch + 0x18) = mask_green;
    *(volatile u32 *)(scratch + 0x1c) = 0x1fu;

    if (r_on != 0) {
        word = *(volatile u16 *)(self + 0x72u);
        packed = (arg1_addr & 0xffff0000u) | word;
        cm_fn_005e49f0(packed, (u8 *)table_base, (u32 *)arg2_addr, (u32 *)arg1_addr, out);
    }
    if (g_on != 0) {
        word = *(volatile u16 *)(self + 0x74u);
        packed = (arg1_addr & 0xffff0000u) | word;
        cm_fn_005e49f0(packed, (u8 *)table_base, (u32 *)arg2_addr, (u32 *)arg1_addr, out);
    }
    if (b_on != 0) {
        word = *(volatile u16 *)(self + 0x78u);
        packed = (arg1_addr & 0xffff0000u) | word;
        cm_fn_005e49f0(packed, (u8 *)table_base, (u32 *)arg2_addr, (u32 *)arg1_addr, out);
    }

    if (r_on != 0) {
        result = cm_fn_005e4800(*(volatile u32 *)scratch, value, flag, 0);
        word = (u16)result;
        *(volatile u16 *)(self + 0x72u) = word;
    }
    if (g_on != 0) {
        result = cm_fn_005e4800(*(volatile u32 *)scratch, value, flag, 0);
        word = (u16)result;
        *(volatile u16 *)(self + 0x74u) = word;
    }
    if (b_on != 0) {
        result = cm_fn_005e4800(*(volatile u32 *)scratch, value, flag, 0);
        word = (u16)result;
        *(volatile u16 *)(self + 0x78u) = word;
    }
    return result;
}
