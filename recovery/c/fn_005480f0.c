#include "cmre.h"

/* Cdecl, three stack arguments (caller cleans up):
     p1  object pointer; field +0x65 holds a record pointer X
     p2  dword array; written at element index = signed byte counter
     p3  byte counter pointer (set to 0 first; incremented by 1 per stored value)
   Record X fields: +0x28, +0x2c, +0x30 (dwords). A value of 0 is not stored.
   The record pointer is re-read from +0x65 before each of the three fields. */
void CM_CDECL cm_fn_005480f0(u32 p1, u32 p2, u32 p3)
{
    u32 x;
    u32 v;
    s8 idx;

    *(volatile u8 *)p3 = 0;
    if (p1 == 0) {
        return;
    }
    x = *(volatile u32 *)(p1 + 0x65);
    if (x == 0) {
        return;
    }
    if (p2 == 0) {
        return;
    }

    v = *(volatile u32 *)(x + 0x28);
    if (v != 0) {
        *(volatile u32 *)p2 = v;
        *(volatile u8 *)p3 = (u8)(*(volatile u8 *)p3 + 1u);
    }

    x = *(volatile u32 *)(p1 + 0x65);
    v = *(volatile u32 *)(x + 0x2c);
    if (v != 0) {
        idx = *(volatile s8 *)p3;
        *(volatile u32 *)(p2 + (u32)(s32)idx * 4u) = v;
        *(volatile u8 *)p3 = (u8)(*(volatile u8 *)p3 + 1u);
    }

    x = *(volatile u32 *)(p1 + 0x65);
    v = *(volatile u32 *)(x + 0x30);
    if (v != 0) {
        idx = *(volatile s8 *)p3;
        *(volatile u32 *)(p2 + (u32)(s32)idx * 4u) = v;
        *(volatile u8 *)p3 = (u8)(*(volatile u8 *)p3 + 1u);
    }
}
