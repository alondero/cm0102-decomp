#include "cmre.h"

/* fn_00795550: thiscall (ECX = object, read before written) with six stack arguments
   (ret 0x18). Preferred-image VA 0x00795550, image base 0x00400000.
   SEH frame {prev, handler 0x0095ea23, state} is linked first; its state is set to 0 after the
   first 0x0054a110 call, to 1 before the index test, and to -1 by the unlink path. The
   0x11c-byte scratch area L lies directly below the frame and holds, with explicit offsets:
   L+0x00 (pointer passed as the second argument of 0x00417c10), L+0x08 (dword copy of *param_2),
   L+0x0b and L+0x13 (receivers for 0x0054a110 and 0x00417c10), L+0x0c (255-byte destination
   of 0x009466d0, with the NUL-free tail at L+0x10a), and L+0x13 (byte copy of param_6).
   Object table: 16-byte-stride index at this + idx*8 supplies the receiver of 0x0068d0d0. */

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(volatile u8 *self, const u8 *src, s16 count);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);

void CM_THISCALL cm_fn_00795550(u8 *self, int *param_1, u32 *param_2, char *param_3,
                                u32 param_4, u32 param_5, u8 param_6)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0x11c];
    u32 d;
    s32 iv;

    seh.state = -1;
    seh.handler = 0x0095ea23u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0054a110((u16 *)(scratch + 0x0b));
    seh.state = 0;
    cm_fn_0054a110((u16 *)(scratch + 0x13));

    seh.state = 1;
    d = CM_GLOBAL(u32, 0x00ae2364) - *(volatile u32 *)param_1;
    d = d - 1u;
    iv = (s32)d;
    if (iv >= 0 && iv < 0x10) {
        *(volatile u32 *)(scratch + 0x08) = *(volatile u32 *)param_2;
        cm_fn_009466d0((u32)(scratch + 0x0c), (u32)param_3, 0xffu);
        *(volatile u8 *)(scratch + 0x10a) = 0;
        cm_fn_00417c10((u32 *)(scratch + 0x0b), (u32 *)scratch, (u16 *)(u32)param_5);
        *(volatile u8 *)(scratch + 0x13) = param_6;
        cm_fn_00417c10((u32 *)(scratch + 0x13), (u32 *)scratch, (u16 *)(u32)param_4);
        cm_fn_0068d0d0((volatile u8 *)*(volatile u32 *)(self + (u32)iv * 8u),
                       (const u8 *)(scratch + 0x08), (s16)0x114);
    }

    CM_FS(u32, 0) = seh.prev;
}
