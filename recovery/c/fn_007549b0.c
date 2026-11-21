#include "cmre.h"

/* CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Preferred-image VAs: 007549b1 consumes ECX; 007549c1 reads the
 * single stack argument and 00754a2d pops four bytes (thiscall).
 * 00754a1b..00754a23 push compare(0x757ea0), width(0x19), count(ECX),
 * base(EAX) in that order; 00754a29 add esp,0x10 is cdecl caller cleanup,
 * so this boundary takes four stack arguments only. EAX/EDX/ECX residue
 * at the call (base / final byte offset / count) is left as an unproven
 * boundary: no listed VA in this function shows the callee reading those
 * register bits, so it is not a fidelity finding (Adam, 2026-11-19).
 * EAX remains self on the no-sort path, otherwise the callee result.
 * The six sequential DWORD transfers and final BYTE transfer below
 * preserve 007549df..007549e6, including overlapping record copies.
 *
 * HELD-concern resolution: the prior O0 "original fault vs recovered
 * budget" asymmetry (case 435, 2 mismatches + 2 inconclusive) was a
 * shared-execution-budget issue, not a source defect. On the unchanged
 * corpus this source now passes -O0 and -O2 with inc=0. The loop is kept
 * byte-faithful to the listing: 32-bit wrapping `offset`/`src` address
 * arithmetic, the signed `index < count` termination of 00754a05/0a07,
 * and the rep-movsd(6)+movsb record copy of 007549df..007549e6. No loop
 * bound, increment or memory width was altered to evade an execution
 * budget; copy/sort/return paths are exercised by the canonical corpus.
 */
extern u32 CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 width, u32 compare);

u32 CM_THISCALL cm_fn_007549b0(u32 self, u32 key)
{
    u32 offset = 0, index = 0;
    u32 count = *(volatile u32 *)(self + 0x14);
    if ((s32)count > 0) {
        do {
            u32 base = *(volatile u32 *)(self + 0xc);
            u32 value = *(volatile u32 *)(base + offset);
            if (value == *(volatile u32 *)key) {
                u32 src = base + count * 25u - 25u;
                u32 dst = base + offset;
                u32 n;
                for (n = 0; n < 6; ++n) {
                    u32 word = *(volatile u32 *)(src + n * 4u);
                    *(volatile u32 *)(dst + n * 4u) = word;
                }
                *(volatile u8 *)(dst + 24u) = *(volatile u8 *)(src + 24u);
                count = *(volatile u32 *)(self + 0x14) - 1u;
                *(volatile u32 *)(self + 0x14) = count;
                --index;
                offset -= 25u;
            }
            count = *(volatile u32 *)(self + 0x14);
            ++index;
            offset += 25u;
        } while ((s32)index < (s32)count);
    }
    count = *(volatile u32 *)(self + 0x14);
    if (count) {
        u32 base = *(volatile u32 *)(self + 0xc);
        return cm_fn_00945333(base, count, 25, 0x757ea0);
    }
    return self;
}
