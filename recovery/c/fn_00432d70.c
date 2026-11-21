/* Recovered from original instructions at preferred-image VA 0x00432d70
   (image base 0x00400000), CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Listing:
     00432d70  push esi
     00432d71  mov esi, ecx        ; esi = this
     00432d73  call 0x432d90       ; ECX still holds this
     00432d78  test byte ptr [esp + 8], 1   ; stack arg low byte, bit 0
     00432d7d  je 0x432d88
     00432d7f  push esi
     00432d80  call 0x944c94
     00432d85  add esp, 4
     00432d88  mov eax, esi        ; return this
     00432d8a  pop esi
     00432d8b  ret 4               ; pops the one dword stack argument

   ECX is read before written (this) and one dword stack argument is
   caller-popped by `ret 4`, so the entry is thiscall with an additional
   stack argument; only its low byte (bit 0) is observed. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00432d90(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_00432d70(u32 param_1, u8 param_2)
{
  cm_fn_00432d90(param_1);
  if ((param_2 & 1) != 0) {
    cm_fn_00944c94((void *)(param_1));
  }
  return param_1;
}
