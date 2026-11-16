#include "cmre.h"

/*
 * Preferred-image VAs below refer to CM0102 3.9.68, SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * 0046e26c pushes the final argument of 007ff270; 0046e26e pushes
 * the sole stack argument of 007ff010. ECX is B74450 at all calls.
 * 0046e27a supplies 00803320's index; 0046e285/286 supply the other
 * two 007ff270 arguments. 0046e304 and 0046e346..34b confirm the
 * one-argument getter and three-argument setter call sites.
 * 0046e35b/35f RET without cleanup: entry is cdecl, result is EAX.
 *
 * The held FindNextFileA/FILETIME OUT-policy discrepancy is downstream
 * of these explicit original-code boundaries. This routine has no file
 * search buffer or time-conversion output to initialize. Do not mask
 * that harness discrepancy by manufacturing defaults or stack layout.
 */
extern u32 CM_THISCALL cm_fn_007ff010(u32 self, u32 index);
extern u32 CM_THISCALL cm_fn_00803320(u32 self, u32 index);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 index,
                                     u32 value, u32 last);

u32 CM_CDECL cm_fn_0046e260(s16 arg)
{
    u32 value = 0;
    u32 selected;
    u32 current;
    u32 base;
    u32 row;
    u32 index;

    if (arg != -1) {
        index = cm_fn_007ff010(0x00b74450u, 0x12u);
        value = cm_fn_00803320(0x00b74450u, index);
        cm_fn_007ff270(0x00b74450u, 0x11u, value, 0);
        row = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
        base = CM_GLOBAL(volatile u32, 0x00b74c4cu + row * 0x300u);
        value = CM_GLOBAL(volatile u32,
                          base + (u32)(s32)arg * 0x18cu + 0xba966u);
    }
    if ((u16)value == 0)
        value = CM_GLOBAL(volatile u16, 0x00dd6bde);
    if ((u16)value != 9)
        return 0;

    selected = 0;
    if (arg != -1) {
        row = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
        base = CM_GLOBAL(volatile u32, 0x00b74c4cu + row * 0x300u);
        selected = CM_GLOBAL(volatile u32,
                             base + (u32)(s32)arg * 0x18cu + 0xba9a6u);
    }
    current = cm_fn_007ff010(0x00b74450u, 0x10u);
    if (selected == current)
        return 0;

    value = 0;
    if (arg != -1) {
        row = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
        base = CM_GLOBAL(volatile u32, 0x00b74c4cu + row * 0x300u);
        value = CM_GLOBAL(volatile u32,
                          base + (u32)(s32)arg * 0x18cu + 0xba9a6u);
    }
    cm_fn_007ff270(0x00b74450u, 0x10u, value, 0);
    return 0xfffffff5u;
}
