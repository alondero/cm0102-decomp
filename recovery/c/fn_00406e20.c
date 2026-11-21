/* fn_00406e20 - preferred-image VA 0x00406e20, image base 0x00400000.

   Listing (32-bit x86):
     00406e20  push esi
     00406e21  mov esi, ecx        ; this (ECX receiver) kept in ESI
     00406e23  call 0x406e40       ; thiscall, ECX = this, no stack args
     00406e28  test byte ptr [esp + 8], 1   ; first stack argument, bit 0
     00406e2d  je 0x406e38
     00406e2f  push esi
     00406e30  call 0x944c94       ; cdecl, one stack argument = this
     00406e35  add esp, 4
     00406e38  mov eax, esi        ; return this
     00406e3a  pop esi
     00406e3b  ret 4               ; callee pops the 4-byte stack argument

   One ECX receiver plus one DWORD stack argument, cleaned by `ret 4`.
   The `& 1` guard tests only bit 0 of that argument's low byte. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00406e40(u8 *param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_00406e20(u32 param_1, u32 param_2)

{
  cm_fn_00406e40((u8 *)(param_1));
  if ((param_2 & 1) != 0) {
    cm_fn_00944c94((void *)(param_1));
  }
  return param_1;
}
