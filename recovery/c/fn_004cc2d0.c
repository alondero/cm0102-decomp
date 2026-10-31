/* Preferred-image VA 0x004cc2d0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: mov eax, [esp+4]; mov ecx, [eax+0x61]; mov al, [ecx+0x45]; ret 4.
   One caller-supplied stack word (the pointer argument), callee-cleaned with
   RET 4. ECX is written before any read, so it is not an input. The return
   value is one byte (AL) at 32-bit pointer argument + 0x61 (dereferenced to a
   pointer), then + 0x45. Pointer arithmetic is 32-bit wraparound. Only AL is
   the result; callers read no EDX. The meanings of the fields at +0x61 and
   +0x45 remain unknown. */
#include "cmre.h"

u8 CM_STDCALL cm_fn_004cc2d0(u32 arg)
{
  u32 inner;

  inner = *(u32 *)(arg + 0x61u);
  return *(u8 *)(inner + 0x45u);
}
