#include "cmre.h"

/* Stdcall: the three stack arguments are object_a (listing [esp+0x178c] after the
   two pushes), flag ([esp+0x1788]) and object_c ([esp+0x178c] at the common tail).
   The scratch area is the original's 0x1774-byte frame below the SEH record:
   scratch+0 holds the first 0x00785830 result, scratch+4 is the buffer written
   by 0x00441c10, scratch+0x3ec is the formatted text. */

extern u32 CM_THISCALL cm_fn_00785830(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_0043f460(u32 self, u32 arg);
extern void CM_CDECL cm_fn_00441c10(u8 *buffer, u32 limit, u32 low, u32 high,
                                    u32 flag);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 dest, u32 format, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg);

void CM_STDCALL cm_fn_0059feb0(u32 object_a, u32 flag, u32 object_c)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scratch[0x1774];
    u32 r1;
    unsigned long long r;

    seh.prev = CM_FS(u32, 0);
    seh.state = -1;
    seh.handler = 0x0095914bu;
    CM_FS(u32, 0) = (u32)&seh;

    if (flag == 0) {
        cm_fn_0066f4e0((u32)(scratch + 0x3ec), 0x009c9720u);
    } else {
        r1 = cm_fn_00785830(object_a, 1u);
        cm_fn_00785830(object_a, 2u);
        *(volatile u32 *)scratch = r1;
        seh.state = 0;
        r = cm_fn_0043f460((u32)scratch, 4u);
        cm_fn_00441c10(scratch + 4, 0x3e8u, (u32)r, (u32)(r >> 32), 4u);
        cm_fn_0066f4e0((u32)(scratch + 0x3ec), 0x009c96b8u, (u32)(scratch + 4));
        seh.state = -1;
    }

    cm_fn_004ae660(object_c, (u32)(scratch + 0x3ec));
    CM_FS(u32, 0) = seh.prev;
}
