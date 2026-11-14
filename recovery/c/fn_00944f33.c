/* Tier-2 C recovery for preferred-image VA 0x00944f33 (stdcall, ret 12).
   Listing:
     00944f33  pop eax
     00944f34  pop ecx
     00944f35  xchg dword ptr [esp], eax
     00944f38  jmp eax
   Tail-call trampoline. The caller pushes three words right to left, so at
   entry the stack is
     [esp+0x00] return address
     [esp+0x04] C   -> becomes ECX (the receiver of the target)
     [esp+0x08] B   -> the target address
     [esp+0x0c] A   -> the target's stack argument
   pop eax takes the return address, pop ecx takes C, and the xchg puts the
   return address back at [esp+0x08] while eax takes B. The jmp therefore
   enters the target with ECX = C, its stack argument at [esp+0x0c] and the
   target's own ret 4 returning to the original caller: 12 bytes consumed. */
#include "cmre.h"

typedef u32 (CM_THISCALL *cm_target_fn)(void *self, u32 arg);

u32
CM_STDCALL cm_fn_00944f33(u32 self, u32 target, u32 arg)
{
  return ((cm_target_fn)target)((void *)self, arg);
}
