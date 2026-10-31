/* Preferred-image VA 0x005b25a0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: ECX is the pointer (read before written), so fastcall with one
   argument and ret 0. Byte-swaps four consecutive dwords at [ecx], [ecx+4],
   [ecx+8] and [ecx+0xc] in place. The load of each dword happens before the
   store to the dword before it, in the listing's order: read 0, read 1,
   store 0, read 2, store 1, read 3, store 2, store 3. Returns 1 in EAX. */
#include "cmre.h"

static u32 cm_bswap32_step(u32 x)
{
    return ((((x & 0xff0000u) | (x >> 0x10)) >> 8) | (((x << 0x10) | (x & 0xff00u)) << 8));
}

u32 CM_FASTCALL cm_fn_005b25a0(u32 *param_1)
{
    u32 uVar1;
    u32 uVar2;

    uVar1 = param_1[0];
    uVar2 = param_1[1];
    param_1[0] = cm_bswap32_step(uVar1);
    uVar1 = param_1[2];
    param_1[1] = cm_bswap32_step(uVar2);
    uVar2 = param_1[3];
    param_1[2] = cm_bswap32_step(uVar1);
    param_1[3] = cm_bswap32_step(uVar2);
    return 1;
}
