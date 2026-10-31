/* WIP: detail mismatch, case 5 (also 6 and 9), -O2 only. Register boundary at
   0x005873c1: the original calls 0x00785830 with EAX still holding the previous
   call's result (r3), and that callee's returned EAX depends on incoming EAX
   upper bytes (see recovery/c/fn_00785830.c). Plain C cannot carry EAX into a
   call, so r4 differs at -O2 (fault at 0x0058741c). -O0 passes by codegen
   coincidence. No codegen workaround attempted. */
#include "cmre.h"

/* Incoming cdecl arguments read from the entry stack frame:
   arg1 (E+4) is the object passed as ECX to every thiscall in this routine,
   arg2 (E+8) selects the path (zero takes the second path),
   arg3 (E+0xc) is used as ECX for the thiscalls to 0x004ae660 and 0x004ae8a0.
   Global 0x00ae23b4 is the first table base (element stride 581 bytes, scaled
   index in the path with a nonzero flag); global 0x00ae23bc is the second table
   base (element stride 110 bytes per index value, used with 0x0053e990).
   buffer is the 0x190-byte scratch area at the frame offset the listing
   addresses with lea (locals 0..7 are held in C variables). */

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 arg2, s32 arg3, s32 arg4);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_CDECL cm_fn_0053e140(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
extern s32 CM_CDECL cm_fn_0053e990(u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
/* cdecl variadic sprintf-like: destination, format, then arguments. */
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);

void CM_CDECL cm_fn_00587370(u32 object, u32 flag, u32 target)
{
    u8 buffer[0x190];
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r4;
    u32 r6;
    u32 base1;
    u32 scaled;
    u32 base2;
    u32 table_ofs;
    u32 fmt;
    s32 value;
    s32 result;

    if (flag != 0) {
        r0 = (u32)cm_fn_00785830((void *)object, 0u);
        r1 = (u32)cm_fn_00785830((void *)object, 1u);
        r2 = (u32)cm_fn_00785830((void *)object, 2u);
        r3 = (u32)cm_fn_00785830((void *)object, 3u);
        r4 = (u32)cm_fn_00785830((void *)object, 4u);
        (void)cm_fn_00785830((void *)object, 5u);
        r6 = (u32)cm_fn_00785830((void *)object, 6u);

        (void)cm_fn_0053e140(r1, r2, r3, (u32)&buffer[0], 0x190u, 1u, r0);

        if (r6 == 0) {
            fmt = 0x009c4b04u;
        } else if (r6 == 1) {
            fmt = 0x009c4b9cu;
        } else {
            fmt = 0x009c4c50u;
        }
        base1 = CM_GLOBAL(volatile u32, 0x00ae23b4);
        scaled = r4 * 581u;
        value = *(volatile s8 *)(base1 + scaled + 0x52u);
        (void)cm_fn_0066f4e0(0x00de1f64u, fmt, (u32)value, (u32)value, 0x64u,
                             base1 + scaled + 0x38u, (u32)&buffer[0]);
        (void)cm_fn_004ae660((void *)target, 0x00de1f64u);

        table_ofs = r0 * 110u;
        base2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
        result = cm_fn_0053e990(table_ofs + base2, r1, r2, r3, 0u);
        if (result != 0) {
            base2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
            table_ofs = table_ofs + base2;
            (void)cm_fn_004ae8a0(target, (u32)&buffer[0], 0x7d7, (s32)table_ofs);
        }

        base1 = CM_GLOBAL(volatile u32, 0x00ae23b4);
        scaled = scaled + base1;
        (void)cm_fn_004ae8a0(target, scaled + 0x38u, 0x7d5, (s32)scaled);
    } else {
        r0 = (u32)cm_fn_00785830((void *)object, 0u);
        r1 = (u32)cm_fn_00785830((void *)object, 1u);
        r2 = (u32)cm_fn_00785830((void *)object, 2u);
        r3 = (u32)cm_fn_00785830((void *)object, 3u);
        (void)cm_fn_00785830((void *)object, 4u);
        (void)cm_fn_00785830((void *)object, 5u);
        (void)cm_fn_00785830((void *)object, 6u);

        (void)cm_fn_0053e140(r1, r2, r3, (u32)&buffer[0], 0x190u, 5u, r0);

        (void)cm_fn_0066f4e0(0x00de1f64u, 0x009c4ad4u, (u32)&buffer[0]);
        (void)cm_fn_004ae660((void *)target, 0x00de1f64u);

        table_ofs = r0 * 110u;
        base2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
        result = cm_fn_0053e990(table_ofs + base2, r1, r2, r3, 0u);
        if (result != 0) {
            base2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
            table_ofs = table_ofs + base2;
            (void)cm_fn_004ae8a0(target, (u32)&buffer[0], 0x7da, (s32)table_ofs);
        }
    }
}
