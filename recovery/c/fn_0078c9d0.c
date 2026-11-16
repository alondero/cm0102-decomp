/* Preferred-image VA 0x0078c9d0, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 4 with ECX read first: thiscall with one stack argument. Only the low
   byte of that argument is tested (0078c9d8). 0078c9f0 and 00944c94 remain
   original boundaries; EAX is the incoming ECX on both paths.
   This is a scalar deleting destructor: the low flag bit selects the
   operator-delete leg. 0078c9f0 first calls 00687970 (reads the word self+3e
   and the dword self+b1) and later 00682300 (reads the unaligned dword
   self+ea), so those fixture arena fields stay zeroed in
   recovery/hints/fn_0078c9d0.json; per the original instructions this wrapper
   adds nothing to that chain, it only forwards ECX and the flag. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_0078c9f0(u32 self);
extern void CM_CDECL cm_fn_00944c94(u32 block);

u32 CM_THISCALL cm_fn_0078c9d0(u32 self, u32 flags)
{
    cm_fn_0078c9f0(self);
    if ((u8)flags & 1u)
        cm_fn_00944c94(self);
    return self;
}
