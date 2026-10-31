#include "cmre.h"

/* Cdecl with two stack arguments (ret 0): a and b are object pointers. Each
   record field is read through the record pointer at +0x61 or the pointers at
   +4, +8 and +0xc, then passed to the 0x00785790 family with the scratch base as
   ECX. The 0xe0-byte scratch area is the original's frame below the SEH record. */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32 self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32 self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32 self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_007875e0(u32 self, u32 arg);

void CM_CDECL cm_fn_005888a0(u32 a, u32 b)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe0];
    u32 scratch_base;
    u32 p;
    u32 v;

    seh.state = -1;
    seh.handler = 0x00958bdbu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    scratch_base = (u32)scratch;

    if (a == 0) {
        goto done;
    }
    if (*(volatile u32 *)(a + 0x61u) == 0) {
        goto done;
    }
    if (b == 0) {
        goto done;
    }
    if (*(volatile u32 *)(b + 0x61u) == 0) {
        goto done;
    }
    v = *(volatile u32 *)(a + 0x39u);
    if (v != 0) {
        if (cm_fn_007875e0(CM_GLOBAL(volatile u32, 0x00b7431cu), v) == 0) {
            goto done;
        }
    }

    cm_fn_0077bbe0(scratch_base, 0x0bcau, 0u);

    v = *(volatile u32 *)a;
    seh.state = 0;
    cm_fn_00785790(scratch_base, 0u, v);

    p = *(volatile u32 *)(a + 4u);
    v = *(volatile u32 *)(p + 0x33u);
    cm_fn_00785790(scratch_base, 1u, v);

    p = *(volatile u32 *)(a + 8u);
    v = *(volatile u32 *)(p + 0x33u);
    cm_fn_00785790(scratch_base, 2u, v);

    p = *(volatile u32 *)(a + 0xcu);
    v = *(volatile u32 *)(p + 0x33u);
    cm_fn_00785790(scratch_base, 3u, v);

    v = *(volatile u32 *)b;
    cm_fn_00785790(scratch_base, 4u, v);

    p = *(volatile u32 *)(b + 4u);
    v = *(volatile u32 *)(p + 0x33u);
    cm_fn_00785790(scratch_base, 5u, v);

    p = *(volatile u32 *)(b + 8u);
    v = *(volatile u32 *)(p + 0x33u);
    cm_fn_00785790(scratch_base, 6u, v);

    p = *(volatile u32 *)(b + 0xcu);
    v = *(volatile u32 *)(p + 0x33u);
    cm_fn_00785790(scratch_base, 7u, v);

    v = *(volatile u32 *)(a + 0x39u);
    cm_fn_007861e0(CM_GLOBAL(volatile u32, 0x00b7431cu), scratch_base, v);

done:
    CM_FS(u32, 0) = seh.prev;
}
