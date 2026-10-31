#include "cmre.h"

/* fn_0069c7e0: stdcall with three stack arguments (ret 0xc), no ECX read before write.
   Preferred-image VA 0x0069c7e0, image base 0x00400000.
   SEH frame {prev, handler 0x0095ccbb, state} is linked first; state is written as 0 after the
   first object setup. The 0xe4-byte scratch area L is the local object (its address +4 is the
   receiver of the 0x00785790 calls) and also holds the one-byte result of 0x007858c0 at L+0.
   Only the low byte of L is read back, as the callee takes a char argument.
   The final 0x00785d40 call uses the global at 0x00b7431c as its receiver. */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern u32 CM_THISCALL cm_fn_007858c0(int param_1, char param_2, char *param_3, char *param_4);
extern unsigned long long CM_THISCALL cm_fn_00785d40(void *self, u32 arg1, u32 arg2, u32 arg3);

void CM_STDCALL cm_fn_0069c7e0(u32 *param_1, u32 *param_2, u32 *param_3)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe4];
    u8 *obj;
    u32 v;
    u32 p;
    s32 g;

    seh.state = -1;
    seh.handler = 0x0095ccbbu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    obj = scratch + 4;
    cm_fn_0077bbe0(obj, 0x1390u, 0u);
    seh.state = 0;

    v = *(volatile u32 *)param_1;
    cm_fn_00785790(obj, 0u, v);
    v = *(volatile u32 *)param_2;
    cm_fn_00785790(obj, 1u, v);
    v = *(volatile u32 *)param_3;
    cm_fn_00785790(obj, 2u, v);
    p = *(volatile u32 *)(param_1 + 1);
    v = *(volatile u32 *)((u8 *)p + 0x33);
    cm_fn_00785790(obj, 3u, v);
    p = *(volatile u32 *)(param_1 + 2);
    v = *(volatile u32 *)((u8 *)p + 0x33);
    cm_fn_00785790(obj, 4u, v);
    p = *(volatile u32 *)(param_1 + 3);
    v = *(volatile u32 *)((u8 *)p + 0x33);
    cm_fn_00785790(obj, 5u, v);

    g = CM_GLOBAL(s32, 0x00ae2364);
    v = *(volatile u32 *)param_1;
    if ((s32)v >= g - 0x10) {
        p = *(volatile u32 *)(param_1 + 1);
        scratch[0] = 7u;
        cm_fn_007858c0((int)obj, (char)7, (char *)p, (char *)scratch);
        cm_fn_00785790(obj, 6u, (u32)(s32)(s8)scratch[0]);
        p = *(volatile u32 *)(param_1 + 2);
        cm_fn_007858c0((int)obj, (char)scratch[0], (char *)p, (char *)scratch);
    }

    cm_fn_00785d40((void *)CM_GLOBAL(u32, 0x00b7431c), (u32)obj, (u32)param_1, 0u);

    CM_FS(u32, 0) = seh.prev;
}
