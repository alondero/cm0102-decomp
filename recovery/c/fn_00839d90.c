/* Preferred-image VA 0x00839D90, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object), plain ret.
   If dword +0xdc is nonzero: for i = 0 while i < dword[0x00AE235C] (signed,
   re-read every pass), the dword at (dword +0xdc) + i*9 is passed to 009452CA
   without a null test and then cleared (+0xdc re-read after the call); then
   dword +0xdc itself is passed to 009452CA and cleared.
   Then, in two passes of sixteen, each nonzero dword from +0x18 through +0x94
   is passed to 009452CA and cleared. Finally dwords +0x00 and +0x04 are each
   passed to 009452CA and cleared when nonzero.
   Direct callers do not observe EAX or EDX. Field meanings remain unknown;
   009452CA is an explicit boundary. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void CM_THISCALL cm_fn_00839d90(u8 *self)
{
    u32 *slot;
    u32 offset;
    s32 i;
    s32 pass;
    s32 n;

    if (*(u32 *)(self + 0xdc) != 0) {
        i = 0;
        if (CM_GLOBAL(s32, 0x00ae235c) > 0) {
            offset = 0;
            /* +0xdc is re-read as the listing does: the zero store may alias it,
               and strict aliasing would otherwise let -O2 reuse a stale copy. */
            do {
                cm_fn_009452ca(*(u32 *)(*(u8 *volatile *)(self + 0xdc) + offset));
                i++;
                *(u32 *)(*(u8 *volatile *)(self + 0xdc) + offset) = 0;
                offset += 9;
            } while (i < CM_GLOBAL(s32, 0x00ae235c));
        }
        cm_fn_009452ca(*(u32 *)(self + 0xdc));
        *(u32 *)(self + 0xdc) = 0;
    }
    slot = (u32 *)(self + 0x18);
    pass = 2;
    do {
        n = 0x10;
        do {
            if (*slot != 0) {
                cm_fn_009452ca(*slot);
                *slot = 0;
            }
            slot++;
        } while (--n != 0);
    } while (--pass != 0);
    if (*(u32 *)(self + 0x00) != 0) {
        cm_fn_009452ca(*(u32 *)(self + 0x00));
        *(u32 *)(self + 0x00) = 0;
    }
    if (*(u32 *)(self + 0x04) != 0) {
        cm_fn_009452ca(*(u32 *)(self + 0x04));
        *(u32 *)(self + 0x04) = 0;
    }
}
