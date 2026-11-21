/* fn_00436610, preferred-image VA 0x00436610 (image base 0x00400000),
   reconstructed from the original instructions:
     00436610 push esi
     00436611 mov esi, ecx
     00436613 call 0x436630          ; thiscall, receiver = entry ECX (this)
     00436618 test byte ptr [esp + 8], 1   ; caller stack argument, low byte
     0043661d je 0x436628
     0043661f push esi
     00436620 call 0x944c94          ; cdecl, arg = this
     00436625 add esp, 4
     00436628 mov eax, esi
     0043662a pop esi
     0043662b ret 4
   `ret 4` pops the caller's one DWORD stack argument; ECX read before written
   makes the entry CM_THISCALL. The returned value is ESI (the this pointer). */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00436630(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_00436610(u32 param_1, u8 param_2)
{
  cm_fn_00436630(param_1);
  if ((param_2 & 1) != 0) {
    cm_fn_00944c94((void *)(param_1));
  }
  return param_1;
}
