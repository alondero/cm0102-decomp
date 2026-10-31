/* Preferred-image VA 0x009441A0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (one stack argument, pointer `out`, ret 0), pointer result in EAX.
   - *out = 0. Allocate via cm_fn_00944e46(0xa4) into esi.
   - esi == 0 -> *out = 0x2718 and esi stays 0.
   - Otherwise write dwords +0x00 = 0xab1500ff, +0x9c = 0xaa005501,
     +0xa0 = 0xa5a55a5a, +0x04 = 0, +0x08 = 0x1f5, +0x98 = 0; allocate
     cm_fn_00944e46(0x7d4) and store it at +0x0c.
     - That allocation is 0 -> call cm_fn_009452ca(esi), esi = 0, *out = 0x2718.
     - Else for i = 1 .. [esi+8] (only when [esi+8] is positive, signed): dword
       ([esi+0xc] + i*4 - 4) = 0, re-reading [esi+0xc] and [esi+8] each step.
   - Always afterwards, six times with p = esi + 0x68 + 4k: dword p-0x18 = 0,
     dword p = 0, dword p+0x18 = 1. With esi == 0 these stores are the original's
     low-address writes (kept faithful, not guarded).
   - Return esi.
   Field meanings remain unknown. Object accesses are volatile so -O2 keeps the
   listing's store order and fault addresses. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

u8 *CM_CDECL cm_fn_009441a0(u32 *out)
{
    u32 esi;
    u32 base;
    u32 count;
    u32 i;
    u32 k;

    *out = 0;
    esi = (u32)cm_fn_00944e46(0xa4);
    if (esi == 0) {
        *out = 0x2718;
    } else {
        *(volatile u32 *)(esi + 0x00) = 0xab1500ffu;
        *(volatile u32 *)(esi + 0x9c) = 0xaa005501u;
        *(volatile u32 *)(esi + 0xa0) = 0xa5a55a5au;
        *(volatile u32 *)(esi + 0x04) = 0;
        *(volatile u32 *)(esi + 0x08) = 0x1f5;
        *(volatile u32 *)(esi + 0x98) = 0;
        base = (u32)cm_fn_00944e46(0x7d4);
        *(volatile u32 *)(esi + 0x0c) = base;
        if (base == 0) {
            cm_fn_009452ca(esi);
            esi = 0;
            *out = 0x2718;
        } else {
            count = *(volatile u32 *)(esi + 0x08);
            if ((s32)count > 0) {
                i = 0;
                do {
                    base = *(volatile u32 *)(esi + 0x0c);
                    i++;
                    *(volatile u32 *)(base + i * 4u - 4u) = 0;
                    count = *(volatile u32 *)(esi + 0x08);
                } while ((s32)i < (s32)count);
            }
        }
    }

    for (k = 0; k < 6; k++) {
        *(volatile u32 *)(esi + 0x68u + k * 4u - 0x18u) = 0;
        *(volatile u32 *)(esi + 0x68u + k * 4u) = 0;
        *(volatile u32 *)(esi + 0x68u + k * 4u + 0x18u) = 1;
    }
    return (u8 *)esi;
}
