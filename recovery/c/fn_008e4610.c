#include "cmre.h"

/* Original 3.9.68 VA 0x008e4610. Convention: stdcall (two stack arguments,
   `ret 8`, no ECX entry read). Result: EAX left by the last call, 0x007861e0.

   Frame: one volatile record holds the 0xe0-byte scratch area (the self pointer
   passed to the 0x00785790 helper calls is its base, L) followed by the SEH
   registration at +0xe0 (prev), +0xe4 (handler) and +0xe8 (state). The scratch
   area is only ever passed as L; its bytes are not initialised by the listing.

   Arguments: arg1 at [S+4] (read as a pointer, fields +0x08, +0x20, +0x24, the
   dword +0x20/+0x24 via the record), arg2 at [S+8] (fields +0x08, +0x0c, +0x20,
   +0x24, +0x2c). Global 0x00ae23bc is the base of 110-byte records indexed by
   arg2+0x0c; 0x00ae23b4 is the base of 581-byte records indexed by a dword. */

extern int CM_THISCALL cm_fn_004cf5a0(int *param_1, int *param_2);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

u32 CM_STDCALL cm_fn_008e4610(u32 arg1, u32 arg2)
{
    volatile struct {
        u8 scratch[0xe0];
        u32 prev;
        u32 handler;
        s32 state;
    } frame;
    void *L;
    u32 n;
    u32 record;
    u32 v;
    u32 node;
    u32 p;
    u32 x;
    u32 result;

    L = (void *)&frame.scratch[0];

    frame.state = -1;
    frame.handler = 0x0096488bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame.prev;

    n = *(u32 *)(arg2 + 0x0cu);
    record = CM_GLOBAL(u32, 0x00ae23bcu) + n * 0x6eu;

    cm_fn_0077bbe0(L, 0x0fc0u, 0u);

    v = *(u32 *)record;
    frame.state = 0;
    cm_fn_00785790(L, 0u, v);

    v = *(u32 *)(*(u32 *)(record + 0x04u) + 0x33u);
    cm_fn_00785790(L, 1u, v);

    v = *(u32 *)(*(u32 *)(record + 0x08u) + 0x33u);
    cm_fn_00785790(L, 2u, v);

    v = *(u32 *)(*(u32 *)(record + 0x0cu) + 0x33u);
    cm_fn_00785790(L, 3u, v);

    v = *(u32 *)(arg1 + 0x08u);
    cm_fn_00785790(L, 4u, v);

    node = (u32)cm_fn_004cf5a0((int *)CM_GLOBAL(u32, 0x00ae1050u), (int *)record);
    CM_GLOBAL(u32, 0x00ae1080u) = node;

    if (node == 0u || (*(u8 *)(node + 0x35u) & 0x3fu) == 5u) {
        p = 0u;
    } else {
        x = *(u32 *)(node + 0x04u);
        p = CM_GLOBAL(u32, 0x00ae23b4u) + x * 0x245u;
    }
    /* 0x8e46f6: with p == 0 the listing still loads [0] (xor eax, eax;
       mov edx, [eax]); volatile keeps that load instead of letting the
       compiler treat it as unreachable. */
    v = *(volatile u32 *)p;
    cm_fn_00785790(L, 5u, v);

    v = *(u32 *)(arg2 + 0x20u);
    cm_fn_00785790(L, 6u, v);

    v = *(u32 *)(arg2 + 0x24u);
    cm_fn_00785790(L, 7u, v);

    v = *(u32 *)(arg1 + 0x20u);
    cm_fn_00785790(L, 8u, v);

    v = *(u32 *)(arg1 + 0x24u);
    cm_fn_00785790(L, 9u, v);

    v = (u32)(s32)(s8)*(u8 *)(arg2 + 0x2cu);
    cm_fn_00785790(L, 0x0au, v);

    x = *(u32 *)(arg2 + 0x08u);
    p = CM_GLOBAL(u32, 0x00ae23b4u) + x * 0x245u;
    result = (u32)cm_fn_007861e0((void *)CM_GLOBAL(u32, 0x00b7431cu), (u32)L, p);

    CM_FS(u32, 0) = frame.prev;
    return result;
}
