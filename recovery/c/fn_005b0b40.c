#include "cmre.h"

/* 0x005b1940: original thiscall boundary, no stack arguments. Its EAX is
 * overwritten by the subsequent field load. */
extern void cm_fn_005b1940(void *self) CM_THISCALL;

/* 0x00944c94: original cdecl bridge, one pointer argument. Its semantic
 * return is unknown; this routine forwards its actual EAX. */
extern u32 cm_fn_00944c94(void *p) CM_CDECL;

/* Preferred-image VA 0x005b0b40, image base 0x00400000.
 * Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * No direct callers are indexed; indirect EAX use is unknown.
 * Preserve the EAX result forwarded from the final original callee.
 *   push esi
 *   mov  esi, ecx              ; ESI = this
 *   call 0x005b1940            ; sub(this)
 *   mov  eax, dword ptr [esi]  ; EAX = *this  (reloaded after the call)
 *   push eax
 *   call 0x00944c94            ; free(*this)
 *   add  esp, 4
 *   pop  esi
 *   ret
 */
u32 CM_THISCALL cm_fn_005b0b40(void **self)
{
    cm_fn_005b1940(self);
    return cm_fn_00944c94(*self);
}

