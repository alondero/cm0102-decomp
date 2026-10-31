#include "cmre.h"

/* Original routine 0x005c45a0 (preferred-image VA, image base 0x00400000).
   Convention: ret 8, two stack arguments: arg1 is an object pointer (edi in the
   listing), arg2 is a pointer to an 8-byte value read as two dwords (esi in the
   listing, also the ECX receiver of both thiscall callees).

   SEH frame: one volatile record at the frame base, linked through fs:[0] and
   unlinked on both return paths (state 0xffffffff, then 0, then low byte 1).

   Scratch area (listing [esp+N] relative to the frame base B, 24 bytes):
     scratch[0..3]   dword copy of low dword of *arg2   (B-0x18)
     scratch[4..7]   dword copy of high dword of *arg2  (B-0x14)
     scratch[8..15]  buffer filled by callee 0x0054c8e0 (B-0x10)
     scratch[16..23] buffer filled by callee 0x0054c5b0 (B-0x08)
   Word indexes compared inside the loop are the listing's own word offsets. */

struct cm_seh_frame {
    void *prev;
    void *handler;
    u32 state;
};

extern void CM_THISCALL cm_fn_0054c8e0(u32 self, u32 buf, u32 value);
extern void CM_THISCALL cm_fn_0054c5b0(u32 self, u32 buf, u32 value);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 value);

CM_STDCALL u32 cm_fn_005c45a0(u32 param_1, u32 param_2)
{
    volatile struct cm_seh_frame frame;
    u8 scratch[24];
    u32 base;
    u32 lo;
    u32 hi;
    u32 psVar3;
    u16 sVar1;
    u16 dx;
    u16 cx;

    /* push -1; push handler; push fs:[0]; mov fs:[0], esp */
    frame.state = 0xffffffffu;
    frame.handler = (void *)&CM_GLOBAL(u8, 0x0095a000);
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)(u32)&frame;

    base = (u32)scratch;

    cm_fn_0054c8e0(param_2, base + 8u, 1u);
    frame.state = 0u;
    cm_fn_0054c5b0(param_2, base + 16u, 1u);
    *(volatile u8 *)&frame.state = 1;

    /* 0x005c45eb loads [param_1+0x12] once and reuses it for the copy; the
       call receiver at 0x005c45f8 reloads it. */
    {
        u32 rec = *(volatile u32 *)(param_1 + 0x12u);
        if (rec == 0u) {
            goto fail;
        }
        lo = *(volatile u32 *)(rec + 2u);
        *(volatile u32 *)(rec + 0xau) = lo;
    }

    psVar3 = cm_fn_0068d1b0(*(volatile u32 *)(param_1 + 0x12u));
    if (psVar3 == 0u) {
        goto fail;
    }

    for (;;) {
        hi = *(volatile u32 *)(param_2 + 4u);
        lo = *(volatile u32 *)(param_2);
        *(volatile u32 *)(base + 4u) = hi;
        sVar1 = *(volatile u16 *)(psVar3 + 2u);
        *(volatile u32 *)(base + 0u) = lo;

        if (*(volatile u16 *)(base + 2u) == sVar1) {
            if ((u16)lo == *(volatile u16 *)(psVar3)) {
                goto found;
            }
        }
        if (*(volatile u16 *)(base + 10u) == sVar1) {
            dx = *(volatile u16 *)(base + 8u);
            if (dx == *(volatile u16 *)(psVar3)) {
                goto found;
            }
        }
        if (*(volatile u16 *)(base + 18u) == sVar1) {
            cx = *(volatile u16 *)(base + 16u);
            if (cx == *(volatile u16 *)(psVar3)) {
                goto found;
            }
        }
        psVar3 = cm_fn_0068d1b0(*(volatile u32 *)(param_1 + 0x12u));
        if (psVar3 == 0u) {
            goto fail;
        }
    }

found:
    CM_FS(void *, 0) = frame.prev;
    return 1u;

fail:
    CM_FS(void *, 0) = frame.prev;
    return 0u;
}
