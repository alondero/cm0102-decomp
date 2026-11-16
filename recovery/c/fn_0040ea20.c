/* Preferred-image VA 0x0040ea20, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   `ret 4` pops one stack argument and 0040ea21 reads ECX before writing it, so
   the entry is thiscall: ECX is the object (saved to ESI at 0040ea21) and the
   single stack argument is read only through `test byte ptr [esp+8], 1` at
   0040ea28, i.e. a BYTE read of that argument's caller-owned home after the
   first call. 0040ea40 receives the unchanged ECX (no instruction reloads the
   receiver: ESI/ECX are never rewritten), and 00944c94 receives the same object
   pushed as its single cdecl argument (cleanup `add esp, 4` at 0040ea35). EAX
   is the saved object on both paths (0040ea38), so u32 is the real width. Both
   callees stay original-code boundaries; neither result is used.

   Held concern (0x40ea40 returns in only 1/600 sweep cases): that gap is in the
   fixture corpus, not in this body. Nothing here depends on the destructor's
   outcome. If 0x40ea40 does not return, the original's 0040ea28/0040ea2f/0040ea30
   are never reached either, and the C body stops at the same boundary call. If it
   returns, the tail is reproduced literally: the BYTE test of the flag home
   happens after the call (so a callee that rewrote the home would be observed),
   the delete is called only when bit 0 is set, and the saved object is returned.
   Flag 0 and flag 1 are each expressed by the one branch at 0040ea2d; extra
   fixtures reaching that branch with 0x40ea40 returning are needed as evidence. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_0040ea40(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_0040ea20(u32 self, u8 flags)
{
    cm_fn_0040ea40(self);
    if (flags & 1u)
        cm_fn_00944c94((void *)self);
    return self;
}
