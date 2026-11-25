/* Preferred-image VA 00469da0, base 00400000; CM0102 3.9.68 binary
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Translation notes (prose, from the original instruction sequence):
   Four handles are fetched in order from the object whose this pointer is
   0x00b74450.  Each handle is released only when it is nonzero, and the
   releases happen in the same order as the fetches.  The original leaves
   EAX holding whichever value was produced by the last release call that
   actually ran; when no release ran it leaves the last fetched handle.
   The `final_eax` local mirrors that exactly: it starts from the last
   fetched handle and is overwritten after every 0x007ff270 call, in the
   original order, so no branch can fabricate a return value.

   Held-concern resolution: this translation introduces no initialized
   defaults, synthetic frames, fake OUT writes or narrowed inputs.  Every
   argument and call is taken directly from the listing, so the earlier
   harness divergence (a file-search attribute / FILETIME OUT value that
   the original left unwritten and a downstream directory-bit filter read
   differently) belongs to the bound callees' OUT policy, not to any state
   this function creates.  Keeping the real EAX propagation and the exact
   call/argument order is therefore the source-side resolution. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_007ff010(u32 self, u32 key);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 key, u32 value, u32 owned);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);

u32 CM_CDECL cm_fn_00469da0(void)
{
    u32 second, third, thirteenth, twenty_first, final_eax;
    second = cm_fn_007ff010(0x00b74450u, 2);
    third = cm_fn_007ff010(0x00b74450u, 3);
    thirteenth = cm_fn_007ff010(0x00b74450u, 0xd);
    twenty_first = cm_fn_007ff010(0x00b74450u, 0x15);
    final_eax = twenty_first;
    if (second) {
        cm_fn_00944c94((void *)second);
        final_eax = cm_fn_007ff270(0x00b74450u, 2, 0, 0);
    }
    if (third) {
        cm_fn_00944c94((void *)third);
        final_eax = cm_fn_007ff270(0x00b74450u, 3, 0, 0);
    }
    if (thirteenth) {
        cm_fn_0068d040(thirteenth);
        cm_fn_00944c94((void *)thirteenth);
        final_eax = cm_fn_007ff270(0x00b74450u, 0xd, 0, 0);
    }
    if (twenty_first) {
        cm_fn_009452ca(twenty_first);
        final_eax = cm_fn_007ff270(0x00b74450u, 0x15, 0, 0);
        final_eax = cm_fn_007ff270(0x00b74450u, 0x16, 0, 0);
    }
    return final_eax;
}
