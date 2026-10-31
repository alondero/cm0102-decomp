#include "cmre.h"

/* fn_00587220: cdecl with two stack arguments (ret 0).
   Preferred-image VA 0x00587220, image base 0x00400000.
   Object (param_1) fields: 0x00 dword passed as the value of record 0, 0x04/0x08/0x0c pointers
   whose +0x33 dwords feed records 1-3, 0x39 pointer (its target's first dword is record 5,
   the pointer itself is passed to 0x007861e0), 0x61 dword that must be nonzero.
   SEH frame {prev, handler 0x00958b3b, state} is linked first and unlinked on every return.
   Its state is written as 0 after the first object read and before the record calls, and is
   never reset to -1. The 0xe0-byte scratch area is the receiver of the 0x0077bbe0 and
   0x00785790 calls and the first argument of 0x007861e0. */

extern u32 CM_CDECL cm_fn_00547770(u32 a, u32 b);
extern int CM_THISCALL cm_fn_0062ed70(int *param_1, int *param_2);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_008972c0(int param_1, int param_2, int *param_3, u32 param_4);
extern int cm_fn_0090cfc0(int param_1);

void cm_fn_00587220(u32 *param_1, u32 *param_2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe0];
    u8 *p1;
    u32 r;
    u32 v;
    u32 ret;
    u32 g;
    u32 q;
    s8 cv;

    seh.state = -1;
    seh.handler = 0x00958b3bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    p1 = (u8 *)param_1;
    if (p1 == 0 || *(volatile u32 *)(p1 + 0x61u) == 0u || *(volatile u32 *)(p1 + 0x39u) == 0u ||
        param_2 == 0) {
        CM_FS(u32, 0) = seh.prev;
        return;
    }

    r = (u32)cm_fn_0090cfc0(3);
    cm_fn_0077bbe0(scratch, 0xfbdu, 0u);
    v = *(volatile u32 *)p1;
    seh.state = 0;
    cm_fn_00785790(scratch, 0u, v);
    v = *(volatile u32 *)(*(volatile u32 *)(p1 + 0x04u) + 0x33u);
    cm_fn_00785790(scratch, 1u, v);
    v = *(volatile u32 *)(*(volatile u32 *)(p1 + 0x08u) + 0x33u);
    cm_fn_00785790(scratch, 2u, v);
    v = *(volatile u32 *)(*(volatile u32 *)(p1 + 0x0cu) + 0x33u);
    cm_fn_00785790(scratch, 3u, v);
    v = *(volatile u32 *)param_2;
    cm_fn_00785790(scratch, 4u, v);
    q = *(volatile u32 *)(p1 + 0x39u);
    v = *(volatile u32 *)q;
    cm_fn_00785790(scratch, 5u, v);
    cm_fn_00785790(scratch, 6u, r);

    ret = cm_fn_00547770((u32)param_2, 0x00ae2c90u);
    if (ret == 0u) {
        CM_FS(u32, 0) = seh.prev;
        return;
    }

    g = CM_GLOBAL(u32, 0x00b63d28);
    if (g != 0u) {
        if (cm_fn_0062ed70((int *)g, (int *)p1) != 0) {
            CM_FS(u32, 0) = seh.prev;
            return;
        }
    }

    cv = (s8)cm_fn_008972c0((int)CM_GLOBAL(u32, 0x00de1d08), (int)param_2, (int *)p1, 0u);
    if (cv >= 0) {
        q = *(volatile u32 *)(p1 + 0x39u);
        cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431c), (u32)scratch, q);
    }

    CM_FS(u32, 0) = seh.prev;
}
