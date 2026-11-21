#include "cmre.h"

/* CM0102 3.9.68 SHA-256:
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VAs 0079bf40..0079bf59: ECX receiver, RET 0. EAX holds
   the terminating loop count (or 0), returned as u32.

   Listing order (all dword field reloads retained, signed count test/cmp):
     0079bf40 edx = [ecx+4] (count), 0079bf43 eax = 0,
     0079bf45 test/jle -> return if count <= 0,
     0079bf49 edx = [ecx+8] (base), 0079bf4c eax++,
     0079bf4d byte[eax + edx - 1] = 0  => byte[base + (old eax)],
     0079bf52 edx = [ecx+4] reload, 0079bf55 cmp eax,edx / jl loop.

   Both field loads are volatile: the byte store may alias self+4/self+8,
   so count and base must be re-read each iteration exactly as the hardware
   does (no caching under C aliasing rules).

   HELD budget concern (O0 budget exhaustion vs original return/fault):
   resolved by folding the single-use base temporary (original EDX at
   0079bf49) directly into the byte-store address expression. This removes
   redundant O0 stack traffic for that temporary without caching either
   field or batching stores, so per-iteration cost stays minimal at -O0
   while every original memory access and its order are preserved. */
u32 CM_THISCALL cm_fn_0079bf40(u32 self)
{
    u32 i = 0;
    if (*(volatile s32 *)(self + 4) > 0) {
        do {
            *(volatile u8 *)(*(volatile u32 *)(self + 8) + i++) = 0;
        } while ((s32)i < *(volatile s32 *)(self + 4));
    }
    return i;
}
