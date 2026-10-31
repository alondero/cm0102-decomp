#include "cmre.h"

/* fn_00808540: cdecl with one stack argument (ret 0). ECX is pushed and never used as a value.
   Preferred-image VA 0x00808540, image base 0x00400000.
   SEH frame {prev, handler 0x009611ab, state} is linked first. State is written as -1, then 0
   after the 0x3638-byte allocation test, then -1 again just before the 0x007f6af0 call; the
   unlink follows the last 0x007ff270 call (or the early exit after 0x007fe600 returns zero).
   The 0x007fe600 receiver is the constant 0x00b74450; 0x007f6af0 takes its receiver from the
   global at 0x00b74448. */

extern u32 CM_THISCALL cm_fn_007f6af0(int param_1, u32 param_2, char *param_3);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);
extern u32 CM_THISCALL cm_fn_008041c0(u32 self, u32 arg_dword);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

void cm_fn_00808540(u32 param_1)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 iv;
    u32 pv;
    u32 esi;
    u32 g;

    seh.state = -1;
    seh.handler = 0x009611abu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    iv = cm_fn_007fe600(0x00b74450u, 0x00808690u, 0x0080eb00u, 1u, 0u, 0x008131c0u);
    if (iv == 0u) {
        CM_FS(u32, 0) = seh.prev;
        return;
    }

    pv = (u32)cm_fn_00944cf1(0x3638u);
    seh.state = 0;
    if (pv == 0u) {
        esi = 0u;
    } else {
        esi = cm_fn_008041c0(pv, param_1);
    }

    g = CM_GLOBAL(u32, 0x00b74448);
    seh.state = -1;
    cm_fn_007f6af0((int)g, esi, 0);

    cm_fn_007ff270(0x00b74450, 0, (char *)esi, 0);
    cm_fn_007ff270(0x00b74450, 1, 0, 0);
    cm_fn_007ff270(0x00b74450, 2, 0, 0);
    cm_fn_007ff270(0x00b74450, 3, 0, 0);
    cm_fn_007ff270(0x00b74450, 4, 0, 0);
    cm_fn_007ff270(0x00b74450, 5, 0, 0);
    cm_fn_007ff270(0x00b74450, 6, 0, 0);
    cm_fn_007ff270(0x00b74450, 7, (char *)1, 0);
    cm_fn_007ff270(0x00b74450, 8, (char *)0xffffffffu, 0);
    cm_fn_007ff270(0x00b74450, 9, (char *)0xffffffffu, 0);
    cm_fn_007ff270(0x00b74450, 0xa, (char *)0xffffffffu, 0);
    cm_fn_007ff270(0x00b74450, 0xb, 0, 0);

    CM_FS(u32, 0) = seh.prev;
}
