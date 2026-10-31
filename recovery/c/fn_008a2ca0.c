#include "cmre.h"

/* fn_008a2ca0: thiscall (ECX = object, read before written), no stack arguments (ret 0).
   Preferred-image VA 0x008a2ca0, image base 0x00400000.
   SEH frame {prev, handler 0x0096364f, state} is linked first and unlinked last. The state
   is set to 0 as a dword, then updated by byte writes of 3, 4 and 5 before each of the first
   three calls, and set to -1 (dword) before the last call. The two-dword scratch area at the
   frame's lower neighbour receives this and the per-call value, as the listing does.
   Callee 0x009462bb is stdcall with four stack arguments (ret 0x10). */

extern void CM_STDCALL cm_fn_009462bb(u32 param_1, u32 param_2, int param_3, void *param_4);

void CM_THISCALL cm_fn_008a2ca0(u8 *self)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    volatile u32 scratch[2];
    u32 eax;

    seh.state = -1;
    seh.handler = 0x0096364fu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    scratch[0] = (u32)self;
    seh.state = 0;

    eax = (u32)(self + 0x8682u);
    scratch[1] = eax;
    *(volatile u8 *)&seh.state = 3u;
    cm_fn_009462bb((u32)(self + 0x9f73u), 0x1beu, 0x14, (void *)0x006ebd60u);

    eax = (u32)(self + 0x4a63u);
    scratch[1] = eax;
    *(volatile u8 *)&seh.state = 4u;
    cm_fn_009462bb((u32)(self + 0x6354u), 0x1beu, 0x14, (void *)0x006ebd60u);

    eax = (u32)(self + 0x0e44u);
    scratch[1] = eax;
    *(volatile u8 *)&seh.state = 5u;
    cm_fn_009462bb((u32)(self + 0x2735u), 0x1beu, 0x14, (void *)0x006ebd60u);

    seh.state = -1;
    cm_fn_009462bb((u32)(self + 0x0137u), 0x625u, 2, (void *)0x00453c10u);

    (void)scratch;
    CM_FS(u32, 0) = seh.prev;
}
