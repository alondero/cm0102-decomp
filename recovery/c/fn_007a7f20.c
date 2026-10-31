#include "cmre.h"

/* fn_007a7f20: thiscall (ECX = object, read before written) with two stack arguments (ret 8).
   Preferred-image VA 0x007a7f20, image base 0x00400000.
   Object fields: 0x04 table base, 0x08 signed record count; table records are 0x1a bytes and
   start with a dword key compared with *param_2. 0x39 pointer inside *param_2 whose first
   dword (or -1 when null) is passed on.
   SEH frame {prev, handler 0x0095f1f0, state} is linked first; state is written as 0, then 1.
   Scratch (0x2c bytes below the frame, with explicit offsets): +0x00 8-byte output of 0x0054c5b0,
   +0x08 8-byte output of the first 0x00417c10 call, +0x10 key, +0x14 value, +0x18 and +0x20
   receivers of 0x0054a110 and 0x00417c10, +0x28 byte 8, +0x29 byte 0. */

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_THISCALL cm_fn_0054c5b0(short *param_1, u32 *param_2, int param_3);
extern void CM_THISCALL cm_fn_007a8510(int param_1, u32 *param_2);
extern void CM_THISCALL cm_fn_007a8690(int param_1, u32 param_2);
extern int cm_fn_0090cfc0(int param_1);

void CM_THISCALL cm_fn_007a7f20(u8 *self, u32 *param_2, s16 param_3)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0x2c];
    s32 n;
    s32 i;
    u8 *base;
    u32 k;
    u32 p;
    u32 v;
    s32 r;
    u32 ret;
    s32 found;

    seh.state = -1;
    seh.handler = 0x0095f1f0u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0054a110((u16 *)(scratch + 0x18));
    seh.state = 0;
    cm_fn_0054a110((u16 *)(scratch + 0x20));

    seh.state = 1;
    if (param_2 != 0) {
        n = *(volatile s32 *)(self + 0x08);
        if (n > 0) {
            base = *(u8 **)(self + 0x04);
            k = *(volatile u32 *)param_2;
            found = -1;
            for (i = 0; i < n; i++) {
                if (*(volatile u32 *)(base + (u32)i * 0x1au) == k) {
                    found = i;
                    break;
                }
            }
            if (found >= 0) {
                n = *(volatile s32 *)(self + 0x08);
                if (n > 0) {
                    for (i = 0; i < n; i++) {
                        if (*(volatile u32 *)(base + (u32)i * 0x1au) == k) {
                            cm_fn_007a8690((int)self, (u32)(base + (u32)i * 0x1au));
                            break;
                        }
                    }
                }
            }
        }
    }

    p = *(volatile u32 *)((u8 *)param_2 + 0x39);
    *(volatile u32 *)(scratch + 0x10) = *(volatile u32 *)param_2;
    if (p == 0u) {
        *(volatile u32 *)(scratch + 0x14) = 0xffffffffu;
    } else {
        v = *(volatile u32 *)p;
        *(volatile u32 *)(scratch + 0x14) = v;
    }

    r = cm_fn_0090cfc0((int)((s32)param_3 / 2));
    ret = cm_fn_0054c5b0((short *)0x00ae2c90u, (u32 *)scratch, r + 1);

    cm_fn_00417c10((u32 *)(scratch + 0x18), (u32 *)(scratch + 0x08), (u16 *)(u32)ret);
    cm_fn_00417c10((u32 *)(scratch + 0x20), (u32 *)(scratch + 0x08), (u16 *)(scratch + 0x18));

    *(volatile u8 *)(scratch + 0x28) = 8u;
    *(volatile u8 *)(scratch + 0x29) = 0u;
    cm_fn_007a8510((int)self, (u32 *)(scratch + 0x10));

    CM_FS(u32, 0) = seh.prev;
}
