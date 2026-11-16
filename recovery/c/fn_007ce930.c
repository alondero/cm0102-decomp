/* Preferred-image VA 0x007ce930, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scalar deleting destructor: ret 4 with ECX read before written, so thiscall
   with one dword stack home (007ce938 tests only its low byte, bit 0).
   007ce950 (destructor body) and 00944c94 (delete) remain original boundaries.
   EAX is the incoming ECX on both paths (007ce948 mov eax, esi). */
#include "cmre.h"

extern void CM_THISCALL cm_fn_007ce950(u8 *self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *block);

u32 CM_THISCALL cm_fn_007ce930(u32 self, u32 flags)
{
    cm_fn_007ce950((u8 *)self);
    if (((u8)flags & 1u) != 0)
        (void)cm_fn_00944c94((void *)self);
    return self;
}
