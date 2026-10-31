#include "cmre.h"

/* Cdecl with two stack arguments (ret 0): p points at a u32 holding a pointer
   (p0), q at a u32 holding a pointer (q0). Two records in the scratch area are
   initialised by 0x0054a110 and filled from p0+0x10 and q0+0x10 by 0x00417c10,
   compared (unsigned word, then signed word), and, when equal, the non-null
   sources are converted by 0x0053dba0 into a 0x7d0-byte buffer, followed by
   0x0090c1d0. The 0xfb8-byte scratch is the frame below the SEH record; the
   SEH state is 0 after the first initialisation and 1 before the conversions. */

extern void CM_FASTCALL cm_fn_0054a110(u32 self);
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 out, u32 src);
extern u32 CM_CDECL cm_fn_0053dba0(u32 value, u8 *dst, u32 size, u32 type);
extern s32 CM_CDECL cm_fn_0090c1d0(volatile u8 *left, volatile u8 *right,
                                   s32 compare_mode, u32 max_count);

s32 CM_CDECL cm_fn_00540130(u32 p, u32 q)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0xfb8];
    u32 p0;
    u32 q0;
    u16 ax;
    u16 cx;
    u32 eax_dword;
    u32 ecx_dword;
    s32 result;

    seh.state = -1;
    seh.handler = 0x00957946u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0054a110((u32)(scratch + 8));

    seh.state = 0;
    cm_fn_0054a110((u32)scratch);

    *(volatile u8 *)&seh.state = 1;

    p0 = *(volatile u32 *)p;
    q0 = *(volatile u32 *)q;

    if (p0 != 0) {
        cm_fn_00417c10((u32)(scratch + 8), (u32)(scratch + 0x10), p0 + 0x10u);
    }
    if (q0 != 0) {
        cm_fn_00417c10((u32)scratch, (u32)(scratch + 0x10), q0 + 0x10u);
    }

    ax = *(volatile u16 *)(scratch + 2);
    cx = *(volatile u16 *)(scratch + 0xa);
    if (ax > cx) {
        result = -1;
        CM_FS(u32, 0) = seh.prev;
        return result;
    }
    if (ax < cx) {
        result = 1;
        CM_FS(u32, 0) = seh.prev;
        return result;
    }

    eax_dword = *(volatile u32 *)scratch;
    ecx_dword = *(volatile u32 *)(scratch + 8);
    if ((s16)(u16)eax_dword > (s16)(u16)ecx_dword) {
        result = -1;
        CM_FS(u32, 0) = seh.prev;
        return result;
    }
    if ((s16)(u16)eax_dword < (s16)(u16)ecx_dword) {
        result = 1;
        CM_FS(u32, 0) = seh.prev;
        return result;
    }

    p0 = *(volatile u32 *)p;
    q0 = *(volatile u32 *)q;
    *(volatile u8 *)(scratch + 0x7e8u) = 0;
    if (p0 != 0) {
        cm_fn_0053dba0(p0, scratch + 0x7e8u, 0x7d0u, 4u);
    }
    *(volatile u8 *)(scratch + 0x18u) = 0;
    if (q0 != 0) {
        cm_fn_0053dba0(q0, scratch + 0x18u, 0x7d0u, 4u);
    }

    result = cm_fn_0090c1d0((volatile u8 *)(scratch + 0x7e8u),
                            (volatile u8 *)(scratch + 0x18u), 1, 0u);

    CM_FS(u32, 0) = seh.prev;
    return result;
}
