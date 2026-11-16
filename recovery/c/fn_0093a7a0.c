/* Preferred-image VA 0x0093a7a0, image base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Pops one dword from the pointer stack at object[self+0x20], dispatches it
   through the table object at object[self+0x28] (the slot at
   [[object+0x28]+0x58]+0x18, i.e. table+0x70), stores the callee's EAX into
   the popped slot and advances the stack pointer past it, returning the new
   pointer.

   Dispatch convention: the listing pushes three dwords (0, the popped value,
   object) and then does `add esp, 0xc` after the indirect call.  Caller
    cleanup of the full argument block proves caller stack cleanup, not a
    complete cdecl register contract. At preferred-image VA 0x0093a7b6,
    ECX receives the popped value; 0x0093a7b8 pushes that value, 0x0093a7b9
    pushes self, and 0x0093a7ba dispatches with ECX still holding the value.
    VA 0x0093a7c0 removes the three stack arguments. These instructions are
    from the pinned binary identified above.

    UNRESOLVED FIDELITY DEFECT: the cdecl pointer below does not establish
    this ECX input. A thiscall pointer would instead presume callee stack
    cleanup, contradicting the evidenced boundary. Do not treat a passing
    differential gate as resolution; a caller-cleaned ECX boundary needs an
    authorized representation before this candidate is accepted again.

    Evidence limitation: the supplied listing identifies no original callback
    target or mapped receiver fixture. Passing fault-only cases cannot
    validate dispatch completion or callback-dependent cursor reloads. */
#include "cmre.h"

typedef u32 (CM_CDECL *callback_fn)(u32 self, u32 value, u32 zero);

u32 CM_CDECL cm_fn_0093a7a0(u32 self)
{
    u32 cursor;
    u32 table;
    u32 value;
    u32 callback;
    u32 result;

    cursor = CM_GLOBAL(volatile u32, self + 0x20u);      /* mov ecx,[esi+0x20] */
    table = CM_GLOBAL(volatile u32, self + 0x28u);       /* mov eax,[esi+0x28] */
    cursor = cursor - 4u;                                /* add ecx,-4 */
    CM_GLOBAL(volatile u32, self + 0x20u) = cursor;      /* mov [esi+0x20],ecx */
    value = CM_GLOBAL(volatile u32, cursor);             /* mov ecx,[ecx] */
    callback = CM_GLOBAL(volatile u32, table + 0x70u);   /* call [eax+0x18] */
    result = ((callback_fn)callback)(self, value, 0u);
    cursor = CM_GLOBAL(volatile u32, self + 0x20u);      /* mov ecx,[esi+0x20] */
    CM_GLOBAL(volatile u32, cursor) = result;            /* mov [ecx],eax */
    cursor = CM_GLOBAL(volatile u32, self + 0x20u);      /* mov eax,[esi+0x20] */
    cursor = cursor + 4u;                                /* add eax,4 */
    CM_GLOBAL(volatile u32, self + 0x20u) = cursor;      /* mov [esi+0x20],eax */
    return cursor;
}
