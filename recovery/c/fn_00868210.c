#include "cmre.h"

/* Original 3.9.68 VA 0x00868210. Convention: thiscall (ECX is copied to EDI on
   entry) with three stack arguments: [S+4] (never read by the listing),
   [S+8] = arg2 (u32 *, its dword is cleared, or set to 1 at the end) and
   [S+0xc] = arg3 (u8 *, receives a byte flag). `ret 0xc`.

   Frame: one volatile record. scratch[0..0x17] is the local buffer L (passed
   as ECX to 0x0054a110 and as the `this` of 0x00417c10, and addressed at L+0,
   L+2, L+8, L+0xa, L+0xc and L+0x10); the SEH registration sits right above it:
   prev (L+0x18), handler (L+0x1c), state (L+0x20).

   Object (this) fields: dword +0x00 passed to 0x0053ab90 (scaled by 110),
   the byte-window at esi = this+0x25: word +0x00 and +0x02, dwords +0x00 and
   +0x04 passed to 0x0054cc30. Globals: 0x00ae2330 dword, 0x00ae2334 dword,
   0x00ae233c byte flag, 0x00ae23bc dword base for records of 110 bytes,
   0x00ae2c90 is the `this` for 0x0054cc30. */

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern short cm_fn_0053ab90(int param_1, short *param_2);
extern u32 * CM_THISCALL cm_fn_0054cc30(short *param_1, u32 *param_2, u32 param_3, u32 param_4);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);

u32 CM_THISCALL cm_fn_00868210(void *self, u32 stack_arg_04, u32 *arg2, u8 *arg3)
{
    volatile struct {
        u8 scratch[0x18];
        u32 prev;
        u32 handler;
        s32 state;
    } frame;
    u32 L;
    u32 esi;
    u32 a;
    u32 b;
    u32 hi;
    u32 w;
    u32 x;
    u32 g;
    s16 s;
    u8 bl;
    u32 *r;
    u32 t_lo;
    u32 t_hi;

    (void)stack_arg_04;

    frame.state = -1;
    frame.handler = 0x00962b08u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame.prev;

    L = (u32)&frame.scratch[0];
    cm_fn_0054a110((u16 *)L);

    frame.state = 0;
    if (arg2 != (u32 *)0) {
        *arg2 = 0u;
    }

    if (CM_GLOBAL(u8, 0x00ae233cu) != 1u) {
        CM_FS(u32, 0) = frame.prev;
        return 1u;
    }

    a = CM_GLOBAL(u32, 0x00ae2330u);
    b = CM_GLOBAL(u32, 0x00ae2334u);
    esi = (u32)self + 0x25u;
    *(volatile u32 *)(L + 0x08u) = a;
    hi = *(volatile u16 *)(L + 0x0au);
    *(volatile u32 *)(L + 0x0cu) = b;

    if (hi > *(u16 *)(esi + 0x02u)
        || (hi == *(u16 *)(esi + 0x02u)
            && (s16)(u16)(a & 0xffffu) > (s16)*(u16 *)esi)) {
        CM_FS(u32, 0) = frame.prev;
        return 1u;
    }

    /* 0x008682ba: first comparison call, then the 0x00417c10 update. */
    t_hi = *(u32 *)(esi + 0x04u);
    t_lo = *(u32 *)esi;
    r = cm_fn_0054cc30((short *)0x00ae2c90u, (u32 *)(L + 0x08u), t_lo, t_hi);
    cm_fn_00417c10((u32 *)L, (u32 *)(L + 0x10u), (u16 *)r);

    x = *(u32 *)self;
    g = CM_GLOBAL(u32, 0x00ae23bcu);
    s = cm_fn_0053ab90((int)(g + x * 0x6eu), (short *)esi);

    bl = (u8)(2u + (((s8)(u8)s < 0x1c) ? 1u : 0u));
    if (arg3 != (u8 *)0) {
        *arg3 = bl;
    }

    t_hi = *(u32 *)(esi + 0x04u);
    t_lo = *(u32 *)esi;
    r = cm_fn_0054cc30((short *)0x00ae2c90u, (u32 *)(L + 0x10u), t_lo, t_hi);
    cm_fn_00417c10((u32 *)L, (u32 *)(L + 0x08u), (u16 *)r);

    w = *(volatile u16 *)(L + 0x02u);
    if ((s32)w < (s32)(s8)bl) {
        CM_FS(u32, 0) = frame.prev;
        return 1u;
    }

    if (arg2 != (u32 *)0 && w == (u32)(s32)(s8)bl && *(u16 *)L == 0u) {
        *arg2 = 1u;
    }

    CM_FS(u32, 0) = frame.prev;
    return 0u;
}
