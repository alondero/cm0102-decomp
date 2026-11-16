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

   0040ea20 saves ESI, making [esp+8] at 0040ea28 the incoming
   four-byte argument home. Keep a volatile, word-sized formal so the BYTE
   lvalue below reads that home after 0040ea23, rather than a narrowed copy
   or a value cached before the boundary call. Only its low byte is read;
   the other three bytes are not examined by the original.
   Callee non-return cases do not exercise this reload or the delete branch. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_0040ea40(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_0040ea20(u32 self, volatile u32 flags)
{
    cm_fn_0040ea40(self);
    if (*(volatile u8 *)&flags & 1u)
        cm_fn_00944c94((void *)self);
    return self;
}
