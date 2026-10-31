#include "cmre.h"

/* fn_00545880: cdecl, one stack argument (ret 0). ECX is only saved, never used as a value.
   Preferred-image VA 0x00545880, image base 0x00400000.
   SEH frame {prev, handler 0x009579fb, state} is linked first and unlinked on every return.
   Globals: 0x009b5984 mode (0 and 1 return param_1 + 1), 0x00ae2408 lazily allocated
   0x40-byte object (fields at +0x20..+0x3c zeroed, then 0x00544fc0 is called with it),
   0x00ae23fc table of indices (scaled by 79 and offset by the object's +0x20 field). */

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_FASTCALL cm_fn_00544fc0(u32 *param_1);

int *cm_fn_00545880(int *param_1)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 esi;
    u32 edx;
    u32 eax;
    u32 idx;
    u32 v;
    s32 m;
    s32 cnt;
    u32 result;

    seh.state = -1;
    seh.handler = 0x009579fbu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    m = CM_GLOBAL(s32, 0x009b5984);
    if (m == 0 || m == 1) {
        CM_FS(u32, 0) = seh.prev;
        return param_1 + 1;
    }

    edx = CM_GLOBAL(u32, 0x00ae2408);
    if (edx == 0u) {
        esi = (u32)cm_fn_00944cf1(0x40u);
        seh.state = 0;
        if (esi != 0u) {
            *(volatile u32 *)(esi + 0x20u) = 0u;
            *(volatile u32 *)(esi + 0x24u) = 0u;
            *(volatile u32 *)(esi + 0x28u) = 0u;
            *(volatile u32 *)(esi + 0x2cu) = 0u;
            *(volatile u32 *)(esi + 0x30u) = 0u;
            *(volatile u32 *)(esi + 0x34u) = 0u;
            *(volatile u32 *)(esi + 0x38u) = 0u;
            *(volatile u32 *)(esi + 0x3cu) = 0u;
            cm_fn_00544fc0((u32 *)esi);
            edx = esi;
        } else {
            edx = 0u;
        }
        CM_GLOBAL(u32, 0x00ae2408) = edx;
        if (edx == 0u) {
            CM_FS(u32, 0) = seh.prev;
            return (int *)0;
        }
    }

    eax = CM_GLOBAL(u32, 0x00ae23fc);
    if (eax != 0u) {
        idx = *(volatile u32 *)param_1;
        v = *(volatile u32 *)(eax + idx * 4u);
        result = v * 79u + *(volatile u32 *)(edx + 0x20u);
        CM_FS(u32, 0) = seh.prev;
        return (int *)result;
    }

    idx = *(volatile u32 *)param_1;
    cnt = *(volatile s32 *)edx;
    if ((s32)idx >= cnt) {
        CM_FS(u32, 0) = seh.prev;
        return (int *)0;
    }
    result = idx * 79u + *(volatile u32 *)(edx + 0x20u);
    CM_FS(u32, 0) = seh.prev;
    return (int *)result;
}
