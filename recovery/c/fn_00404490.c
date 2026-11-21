/* fn_00404490, preferred-image VA 0x00404490, image base 0x00400000.
   Listing (VA, instruction):
     00404490  push esi
     00404491  mov esi, ecx            ; receiver = entry ECX
     00404493  call 0x4044b0           ; cm_fn_004044b0(this)
     00404498  test byte ptr [esp + 8], 1   ; stack arg low byte (saved esi, ret, arg)
     0040449d  je 0x4044a8
     0040449f  push esi                ; argument = receiver
     004044a0  call 0x944c94           ; cm_fn_00944c94(this), cdecl, caller cleans
     004044a5  add esp, 4
     004044a8  mov eax, esi           ; return receiver
     004044aa  pop esi
     004044ab  ret 4                  ; pops return address + 4 stack bytes
   Plain C reconstruction. Unvalidated until `cmre test --c` passes. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_004044b0(u8 *param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_00404490(u32 param_1, u8 param_2)
{
  cm_fn_004044b0((u8 *)(param_1));
  if ((param_2 & 1) != 0) {
    cm_fn_00944c94((void *)(param_1));
  }
  return param_1;
}
