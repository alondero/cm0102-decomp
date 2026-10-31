/* Preferred-image VA 0x007df1c0 (image base 0x00400000). Two stack arguments
   (`ret 8`). The function never reads ECX itself, but the call at 0x007df1fc
   reaches 0x007e82e0 with the incoming ECX unchanged, and that callee reads
   ECX as `this`. The entry ECX is therefore kept as an explicit `self`
   parameter (CM_THISCALL) and passed through at that boundary.

   Frame: SEH record {prev, handler 0x009601a8, state} at S+0x20..S+0x2b, where
   S = esp after `sub esp, 0x20` (less the two saved registers). The state is
   -1 on entry and 0 from the 0x007417c10 path onwards. The scratch area is
   S+0x00..S+0x1f: byte 3 is the flag (0xff, 1 or 2), bytes 4..7 the pointer
   passed to 0x00417c10, bytes 0x0c..0x0f the four bytes {bl, 0, 0xff, 0xff}
   passed through to it, 0x10..0x1f its object, and bytes 0x18..0x1b are
   written after that call. The low byte of the first stack argument slot
   (E+4) is reassigned as a local `arg1_slot`. */
#include "cmre.h"

extern int CM_THISCALL cm_fn_007e82e0(int param_1,u32 param_2,int param_3);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1,u32 *param_2,u16 *param_3);
extern void CM_THISCALL cm_fn_00556940(int param_1,int param_2,char *param_3);
extern void CM_STDCALL cm_fn_00557280(int param_1, char *param_2, int param_3);

void CM_THISCALL cm_fn_007df1c0(u32 self, u32 arg1, u32 arg2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } frame;
    volatile u8 scratch[0x20];
    u32 prev_value;
    u32 arg1_slot;
    u32 p;
    u32 r;
    u32 v0;
    u32 g7;
    u32 g8;
    u32 rec;
    u32 p1;
    u32 a2;
    u8 bl;
    u8 cl;
    u8 dl;
    u8 al;
    u8 flag_cl;
    u16 ax;

    prev_value = CM_FS(u32, 0);
    frame.state = -1;
    frame.handler = 0x009601a8u;
    frame.prev = prev_value;
    CM_FS(u32, 0) = (u32)&frame;

    p = arg1;
    bl = 0;
    scratch[3] = 0xff;
    arg1_slot = arg1;

    if (*(volatile u8 *)(p + 0x2cu) == 0) {
        if (*(volatile u8 *)(p + 0x2du) == 0) {
            goto epilogue;
        }
    }

    v0 = *(volatile u32 *)p;
    r = (u32)cm_fn_007e82e0((int)self, v0, 1);

    dl = *(volatile u8 *)(p + 0x2du);
    cl = 2;
    if (dl == 1) {
        bl = 1;
        scratch[3] = cl;
        arg1_slot = (arg1_slot & 0xffffff00u) | bl;
    }

    dl = *(volatile u8 *)(p + 0x2cu);
    if (dl != 0) {
        dl = *(volatile u8 *)(r + 4u);
        dl = (u8)(dl + 1u);
        *(volatile u8 *)(r + 4u) = dl;
        if ((dl & 1u) == 0) {
            bl = (u8)(bl + 1u);
            scratch[3] = 1;
            arg1_slot = (arg1_slot & 0xffffff00u) | cl;
        }
    }

    if (bl == 0) {
        goto epilogue;
    }

    (void)cm_fn_0054a110((u16 *)&scratch[0x10]);

    frame.state = 0;
    scratch[0x0c] = bl;
    *(volatile u16 *)&scratch[0x0e] = 0xffffu;
    scratch[0x0d] = 0;
    cm_fn_00417c10((u32 *)&scratch[0x10], (u32 *)&scratch[4], (u16 *)0x00ae2c90u);

    ax = (u16)(s16)(s8)arg1_slot;
    cl = *(volatile u8 *)(p + 0x48u);
    *(volatile u16 *)&scratch[0x1d] = ax;
    al = scratch[0x1f];
    dl = scratch[3];
    al = (u8)(al & 0xfdu);
    scratch[0x1c] = dl;
    flag_cl = (cl == 0) ? 1u : 0u;
    al = (u8)(al & 0xfbu);
    cl = (u8)((flag_cl << 2) | al);

    v0 = *(volatile u32 *)p;
    scratch[0x1f] = cl;
    g7 = CM_GLOBAL(volatile u32, 0x00ae28f0u);
    scratch[0x18] = 0xff;
    scratch[0x19] = 0xff;
    scratch[0x1a] = 0xff;
    scratch[0x1b] = 0;
    cm_fn_00556940((int)g7, (int)v0, (char *)&scratch[0x0c]);

    p1 = *(volatile u32 *)p;
    a2 = arg2;
    g8 = CM_GLOBAL(volatile u32, 0x00ae23bcu);
    rec = g8 + 110u * p1;
    /* mov ecx, [0xae28f0]: 0x00557280 does not read ECX (ret 0xc, stdcall). */
    (void)CM_GLOBAL(volatile u32, 0x00ae28f0u);
    cm_fn_00557280((int)rec, (char *)&scratch[0x0c], (int)a2);

epilogue:
    CM_FS(u32, 0) = frame.prev;
}
