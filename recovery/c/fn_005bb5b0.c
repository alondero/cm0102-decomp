/* Preferred-image VA 0x005BB5B0, image base 0x00400000.
   Listing: mov [ecx], 0x0096A904; jmp 0x0048C650. Thiscall: the receiver in ECX
   is stored a vtable-style constant at its offset 0, then control tail-jumps to
   0x0048C650 with the same ECX. No stack arguments. The only caller does not read
   EAX, so the return is void and the callee's result is not modelled. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0048c650(void *self_arg);

void CM_THISCALL cm_fn_005bb5b0(void *self)
{
  *(u32 *)self = 0x0096a904;
  (void)cm_fn_0048c650(self);
}
