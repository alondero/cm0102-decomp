/* Preferred-image VA 0x00717c70, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Shape: the object arrives in ECX and is kept in ESI. Twice, the routine
   reloads the dword at [self+0x19e]; when it is non-null it calls the
   thiscall member (0x006c38c0 first, 0x006c38f0 second) on it with self
   pushed as the single argument, otherwise it takes zero. Each result is
   masked with 0x880 and the 16-bit test of that mask decides whether to
   continue. Passing both tests sets AL = 1; failing either clears AL only,
   so EAX[31:8] keep the masked value of the last call (ret 0).

   ECX is read before written: ECX carries the object (esi).  The harness feeds
   the fixture's register state in EAX/EDX/ECX, so the entry contract is
   (incoming_eax, incoming_edx, self).  EAX is live input: the original never
   writes EAX before the first call, so 006c38c0 sees the caller's EAX, and
   the second call sees the masked first result.

   Both callees are entered with ECX = the dword loaded from [self+0x19e] and
   with one stack word (esi); the callee pops that word, so the caller's stack
   is unchanged (the C contributes no stack word beyond the original's single
   argument push).

   Stack aliasing (held concern) and how this C resolves it: the second load
   at 0x00717c91 is a *fresh* read of the same field, taken after the first
   call.  When the field aliases the stack (entry ESP = self+0x1a7), the
   original's `push esi` (ESP-8) and the CALL's return word 0x00717c83
   (ESP-12) overwrite the field, and the original's second test then sees the
   overwritten value (zero for that placement).  This C therefore re-reads the
   field with a volatile load at the original's second read site instead of
   reusing the value loaded before the call: the value the C tests is the
   value that stands in memory after the call, exactly as the original
   observes it, and nothing is cached across either call.  The C's own stack
   traffic is the original's single argument push (esi) with the callee
   popping it, so this function adds no stack word that could mask the
   overwrite the original sees.  Reproducing the placement itself still needs
   a harness fixture with a controlled entry ESP (ESP = self+0x1a7) and stack
   initialisation, because the emulator pins the entry ESP to STACK_TOP. */
#include "cmre.h"
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38c0(u32, u32, u32, u32);
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_006c38f0(u32, u32, u32, u32);
u32 CM_CDECL __attribute__((regparm(3))) cm_fn_00717c70(
    u32 incoming_eax, u32 incoming_edx, u32 self)
{
    /* 00717c73: mov ecx, [esi+0x19e] */
    u32 target = CM_GLOBAL(volatile u32, self + 0x19eu);
    /* 00717c7d/00717c7e: push esi; call 006c38c0 (ECX = target). */
    u32 result = target ? cm_fn_006c38c0(incoming_eax, incoming_edx, target, self) : 0;
    /* 00717c85/00717c87: xor eax,eax on the null path, then and eax,0x880. */
    result &= 0x880u;
    /* 00717c8f: test ax,ax - zero jumps to 0x717cb3 (xor al,al). */
    if (!result) return result & 0xffffff00u;
    /* 00717c91: the field is loaded again here, after the call; a value the
       call wrote over the field (stack aliasing) is what the original tests. */
    target = CM_GLOBAL(volatile u32, self + 0x19eu);
    /* 00717c9b/00717c9c: push esi; call 006c38f0 (ECX = target, EAX = the
       masked first result). */
    result = target ? cm_fn_006c38f0(result, incoming_edx, target, self) : 0;
    /* 00717ca3/00717ca5: xor eax,eax on the null path, then and eax,0x880. */
    result &= 0x880u;
    /* 00717cad: test ax,ax - zero jumps to 0x717cb3 (xor al,al). */
    if (!result) return result & 0xffffff00u;
    /* 00717caf: mov al,1 keeps bits 8..31 of EAX. */
    return (result & 0xffffff00u) | 1u;
}
