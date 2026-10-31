#include "cmre.h"

/* Original 3.9.68 VA 0x007ff630. Convention: cdecl with one stack argument
   arg1 at [S+4] (`ret`, no pops). The entry `push ecx` is a scratch slot whose
   contents are never read back, so ECX is not modelled.

   Table words: T[si] at 0x00b77c5a + 2*si (a count, 0..15).
   Records: 16 slots of 0x300 bytes at 0x00b74c24 + 0x300*si. Each slot holds
   48-byte entries (0x30 each) at 0x00b74c24 + 0x300*si + 0x30*k.
   Within a slot, the first word is the key (compared with arg1 low word).
   Slot offsets used by the insert path: +0x00 word key, +0x02, +0x06, +0x0a,
   +0x0e dwords (zeroed), +0x12 and +0x14 words (zeroed), +0x16 dword (1, then 0
   on the copy path), +0x1a dword (0, then 1 on the copy path), +0x22 word
   (0xffff), +0x24 dword (0), +0x28 dword (0x00b77c94 when key is 0, else
   0x00ca72f5), +0x2c dword (-1).

   Globals: 0x00b77c90 and 0x00b77c80 dwords gate the whole routine;
   0x00b77c7a word holds the current slot index and is saved/restored in BP;
   0x00b77c7c word receives the slot index after the callback; 0x00dd6bf0 is a
   callback pointer (cdecl, one argument: arg1 as a dword). */

extern void CM_CDECL cm_fn_0090ba90(void *arg1, void *arg2, u32 count);

typedef void (CM_CDECL *cm_fn_007ff630_callback)(u32 arg);

void CM_CDECL cm_fn_007ff630(u32 arg1)
{
    s16 si;
    u16 cx;
    u16 dx;
    u16 saved_global;
    u32 rec;
    u32 edi;
    cm_fn_007ff630_callback callback;

    if (CM_GLOBAL(u32, 0x00b77c90u) != 0u) {
        if (CM_GLOBAL(u32, 0x00b77c80u) == 0u) {
            return;
        }
    }

    dx = (u16)arg1;

    for (si = 0; si < 0x10; si++) {
        if (CM_GLOBAL(u16, 0x00b77c5au + (u32)(s32)si * 2u) == 0u) {
            goto insert;
        }
        rec = 0x00b74c24u + (u32)(s32)si * 0x300u;
        if (CM_GLOBAL(u16, rec) == dx) {
            goto found;
        }
    }
    return;

found:
    cx = CM_GLOBAL(u16, 0x00b77c5au + (u32)(s32)si * 2u);
    if ((s16)cx >= 0x10) {
        return;
    }
    if ((s16)cx > 0) {
        s32 remaining = (s32)(s16)cx;
        u32 p = 0x00b74c24u + (u32)(s32)si * 0x300u + (u32)remaining * 0x30u;
        do {
            u32 next = p - 0x30u;
            cm_fn_0090ba90((void *)p, (void *)next, 0x30u);
            p = next;
            remaining = remaining - 1;
        } while (remaining != 0);
    }

insert:
    if (si >= 0x10) {
        return;
    }
    CM_GLOBAL(u16, 0x00b77c5au + (u32)(s32)si * 2u) += 1u;
    rec = 0x00b74c24u + (u32)(s32)si * 0x300u;
    CM_GLOBAL(u16, rec + 0x00u) = dx;
    CM_GLOBAL(u32, rec + 0x02u) = 0u;
    CM_GLOBAL(u32, rec + 0x06u) = 0u;
    CM_GLOBAL(u32, rec + 0x0au) = 0u;
    CM_GLOBAL(u32, rec + 0x0eu) = 0u;
    CM_GLOBAL(u16, rec + 0x12u) = 0u;
    CM_GLOBAL(u16, rec + 0x14u) = 0u;
    CM_GLOBAL(u32, rec + 0x16u) = 1u;
    edi = (dx == 0u) ? 0x00b77c94u : 0x00ca72f5u;
    CM_GLOBAL(u32, rec + 0x28u) = edi;
    edi = 0xffffffffu;
    CM_GLOBAL(u32, rec + 0x1au) = 0u;
    CM_GLOBAL(u16, rec + 0x22u) = (u16)edi;
    CM_GLOBAL(u32, rec + 0x24u) = 0u;
    CM_GLOBAL(u32, rec + 0x2cu) = edi;

    callback = CM_GLOBAL(volatile cm_fn_007ff630_callback, 0x00dd6bf0u);
    saved_global = CM_GLOBAL(u16, 0x00b77c7au);
    CM_GLOBAL(u16, 0x00b77c7au) = (u16)si;
    if (callback != (cm_fn_007ff630_callback)0) {
        callback(arg1);
        CM_GLOBAL(u16, 0x00b77c7cu) = (u16)si;
        CM_GLOBAL(u16, 0x00b77c7au) = saved_global;
        return;
    }

    CM_GLOBAL(u32, rec + 0x16u) = 0u;
    CM_GLOBAL(u32, rec + 0x1au) = 1u;
    cm_fn_0090ba90((void *)rec, (void *)(rec + 0x30u), 0x30u);
    CM_GLOBAL(u16, 0x00b77c7au) = saved_global;
}
