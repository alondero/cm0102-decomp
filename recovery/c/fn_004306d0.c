/* Preferred-image VA 0x004306d0, image base 0x00400000.
   Listing:
     004306d0  push esi
     004306d1  mov esi, ecx
     004306d3  call 0x4306f0
     004306d8  test byte ptr [esp + 8], 1
     004306dd  je 0x4306e8
     004306df  push esi
     004306e0  call 0x944c94
     004306e5  add esp, 4
     004306e8  mov eax, esi
     004306ea  pop esi
     004306eb  ret 4
   `ret 4` pops one stack argument; ECX is read before written and forwarded
   as the receiver of 0x4306f0, so the entry is thiscall. After `push esi`,
   [esp + 8] is that stack byte argument (saved esi at [esp], return address
   at [esp + 4]). EAX returns the ECX value (esi). */
#include "cmre.h"

extern void CM_THISCALL cm_fn_004306f0(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_004306d0(u32 param_1, u8 param_2)

{
  cm_fn_004306f0(param_1);
  if ((param_2 & 1) != 0) {
    cm_fn_00944c94((void *)(param_1));
  }
  return param_1;
}
