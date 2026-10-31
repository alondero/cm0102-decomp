#include "cmre.h"

/* fn_00789860: cdecl with two stack arguments (ret 0), no ECX read before write.
   Preferred-image VA 0x00789860, image base 0x00400000.
   SEH frame {prev, handler 0x0095e2bb, state} is linked first; state is written as 0 after the
   first setup call and never changed again. The 0xe0-byte scratch area L is the receiver of the
   first three 0x0077bbe0/0x00785830 calls and holds the 0x00785830 index results.
   The shared receiver for the 0x007ff270 and 0x007ff010 calls is 0x00b74450. */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786b70(int *param_1, int *param_2, int param_3, int param_4);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
extern u32 CM_THISCALL cm_fn_007ff010(int param_1, int param_2);
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

void cm_fn_00789860(u32 param_1, u32 param_2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe0];
    u32 i1;
    u32 i2;
    u32 val;
    u32 pv;
    u32 q;

    seh.state = -1;
    seh.handler = 0x0095e2bbu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0077bbe0(scratch, 0u, 0u);
    seh.state = 0;

    cm_fn_00786b70((int *)CM_GLOBAL(u32, 0x00b7431c), (int *)param_1, (int)param_2, (int)scratch);

    i1 = (u32)cm_fn_00785830(scratch, 0u);
    i2 = (u32)cm_fn_00785830(scratch, 1u);

    if (i2 == 0u) {
        if (cm_fn_007fe600(0x00b74450u, 0x00789990u, 0x00789bb0u, 1u, 0u, 0u) == 0u) {
            CM_FS(u32, 0) = seh.prev;
            return;
        }
        val = 0u;
    } else {
        val = CM_GLOBAL(u32, 0x00ae23bc) + i1 * 0x6eu;
        if (cm_fn_007fe600(0x00b74450u, 0x00789990u, 0x00789bb0u, 1u, 0u, 0u) == 0u) {
            CM_FS(u32, 0) = seh.prev;
            return;
        }
    }

    cm_fn_007ff270(0x00b74450, 0, (char *)val, 0);
    pv = (u32)cm_fn_00944cf1(0x65u);
    cm_fn_007ff270(0x00b74450, 1, (char *)pv, 0);
    cm_fn_007ff270(0x00b74450, 2, (char *)param_2, 0);
    q = cm_fn_007ff010(0x00b74450, 1);
    *(volatile u8 *)q = 0u;

    CM_FS(u32, 0) = seh.prev;
}
