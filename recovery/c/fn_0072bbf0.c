#include "cmre.h"

/* Stdcall with three stack arguments (ret 0xc): a and b are object pointers, c is
   a word-sized selector (sign-extended). Selector values 0..0x23 index a byte
   map at 0x0072bd2c whose value selects a DWORD jump target at 0x0072bd24; both
   tables are read as the original does (target 0x72bce0 calls 0x0090cfc0(4)).
   The result is passed on as the seventh call of the sequence. */

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32 self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32 self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32 self, u32 arg1, u32 arg2);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 size);

void CM_STDCALL cm_fn_0072bbf0(u32 a, u32 b, u32 c)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xe0];
    u32 scratch_base;
    u32 p;
    u32 v;
    u32 selector;
    u32 result;

    seh.state = -1;
    seh.handler = 0x0095d65bu;
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

    cm_fn_0077bbe0(scratch_base, 0x0bdbu, 0u);

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
    if (p != 0) {
        v = *(volatile u32 *)(p + 0x33u);
    } else {
        v = 0xffffffffu;
    }
    cm_fn_00785790(scratch_base, 3u, v);

    v = *(volatile u32 *)b;
    cm_fn_00785790(scratch_base, 4u, v);

    selector = (u32)(s32)(s16)c;
    cm_fn_00785790(scratch_base, 5u, selector);
    cm_fn_00785790(scratch_base, 6u, 0u);

    if (selector > 0x23u) {
        result = 0u;
    } else {
        /* Byte map at 0x0072bd2c, then the DWORD target at 0x0072bd24 + idx*4:
           0x0072bce0 -> 0x90cfc0(4), 0x0072bcec -> 0. */
        u32 idx = CM_GLOBAL(volatile u8, 0x0072bd2cu + selector);
        u32 target = CM_GLOBAL(volatile u32, 0x0072bd24u + idx * 4u);
        if (target == 0x0072bce0u)
            result = (u32)cm_fn_0090cfc0(4);
        else
            result = 0u;
    }
    cm_fn_00785790(scratch_base, 7u, result);

    cm_fn_007861e0(CM_GLOBAL(volatile u32, 0x00b7431cu), scratch_base, b);

done:
    CM_FS(u32, 0) = seh.prev;
}
