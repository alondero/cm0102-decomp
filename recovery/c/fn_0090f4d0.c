#include "cmre.h"

/* Preferred-image VAs; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0090f4d0-d3 saves ESI, keeps the entry ECX receiver and forwards it to
   0090f4f0 (ECX = this, 0090f4f7/0090f507 read it as the receiver).
   0090f4d8 tests only the low flag byte of the caller's argument slot
   ([esp+8] after the push at 0090f4d0 is the caller's [esp+4]).
   0090f4df-e5 pushes the full 32-bit saved receiver and calls 00944c94
   with caller cleanup (add esp,4).
   0090f4e8-eb returns the saved receiver, restores ESI and pops the four
   argument bytes (ret 4).
   Boundary: any difference in the callee's uninitialized deeper stack slots
   or in the compiler's callee-saved register choice is unproven consumption;
   the 30-byte listing has no instruction reading those bits. */
extern void CM_THISCALL cm_fn_0090f4f0(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_0090f4d0(u32 self, u8 flags)
{
    cm_fn_0090f4f0(self);
    if (flags & 1)
        cm_fn_00944c94((void *)self);
    return self;
}
